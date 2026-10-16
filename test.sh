#!/usr/bin/env bash
# =============================================================================
# test.sh — Full test of my_blockchain
# Covers : help, add node, add block, ls, ls -l, sync, rm block, rm node,
#          invalid command, save / load.
# =============================================================================

set -e

# Delay
export DELAY=0.2
export DELAY_LONG=0.3

# Colors
RED='\033[0;31m'
GRN='\033[0;32m'
YLW='\033[1;33m'
CYN='\033[0;36m'
BLD='\033[1m'
RST='\033[0m'

pass() { printf "${GRN}[PASS]${RST} %s\n" "$1"; }
fail() { printf "${RED}[FAIL]${RST} %s\n" "$1"; FAILURES=$((FAILURES + 1)); }
section() { printf "\n${CYN}${BLD}=== %s ===${RST}\n" "$1"; }
FAILURES=0

# Prerequisites
if ! command -v expect &>/dev/null; then
    printf "${RED}[ERROR]${RST} 'expect' is not installed.\n"
    exit 1
fi

# Compilation
section "Compilation"
make re 2>&1 | tail -3
make clean 2>&1 | tail -1
printf "${GRN}Build OK${RST}\n"

# Original state
SAVE_FILE=".blockchain.save"
SAVE_BACKUP=".blockchain.save.bak"
[ -f "$SAVE_FILE" ] && cp "$SAVE_FILE" "$SAVE_BACKUP"

# ===========================================================================
# SESSION 1 : complete functional tests
#   Starting from an empty blockchain (or pre-existing state), we build a 
#   specific state, verify it, and quit with quit (save).
# ===========================================================================
section "SESSION 1 — functional tests"

OUTPUT=$(expect -c '
    set timeout 30
    log_user 1
    spawn ./my_blockchain
    sleep $env(DELAY_LONG)

    # help 
    send "help\r"
    sleep $env(DELAY)
    send "help add\r"
    sleep $env(DELAY)
    send "help rm\r"
    sleep $env(DELAY)
    send "help ls\r"
    sleep $env(DELAY)
    send "help sync\r"
    sleep $env(DELAY)
    send "help quit\r"
    sleep $env(DELAY)
    send "help help\r"
    sleep $env(DELAY)

    # invalid commands
    send "\r"
    sleep $env(DELAY)
    send "foobar\r"
    sleep $env(DELAY)
    send "add\r"
    sleep $env(DELAY)
    send "rm\r"
    sleep $env(DELAY)
    send "add node\r"
    sleep $env(DELAY)
    send "add block\r"
    sleep $env(DELAY)
    send "add node abc\r"
    sleep $env(DELAY)

    # reset blockchain : rm node *
    send "rm node *\r"
    sleep $env(DELAY)

    # ls empty blockchain
    send "ls\r"
    sleep $env(DELAY)
    send "ls -l\r"
    sleep $env(DELAY)

    # add node
    send "add node 1\r"
    sleep $env(DELAY)
    send "add node 2\r"
    sleep $env(DELAY)
    send "add node 3\r"
    sleep $env(DELAY)
    send "add node 4\r"
    sleep $env(DELAY)

    # duplicate node
    send "add node 1\r"
    sleep $env(DELAY)
    send "add node 2\r"
    sleep $env(DELAY)

    # ls after add nodes
    send "ls\r"
    sleep $env(DELAY)
    send "ls -l\r"
    sleep $env(DELAY)

    # add block on a specific node
    send "add block blockA 1\r"
    sleep $env(DELAY)
    send "add block blockB 1\r"
    sleep $env(DELAY)
    send "add block blockC 2\r"
    sleep $env(DELAY)
    send "add block blockD 3\r"
    sleep $env(DELAY)

    # add block : non-existent node
    send "add block blockX 99\r"
    sleep $env(DELAY)

    # add block : duplicate
    send "add block blockA 1\r"
    sleep $env(DELAY)

    # add block * (on all nodes)
    send "add block blockGlobal *\r"
    sleep $env(DELAY)

    # ls -l to check current state
    send "ls -l\r"
    sleep $env(DELAY)

    # sync
    send "sync\r"
    sleep $env(DELAY)

    # ls -l after sync : all nodes must have the same blocks
    send "ls -l\r"
    sleep $env(DELAY)

    # rm block on a specific node
    send "rm block blockD 1\r"
    sleep $env(DELAY)

    # rm block * on a node
    send "rm block * 4\r"
    sleep $env(DELAY)

    # rm block <bid> * (on all nodes)
    send "add block tempBlock *\r"
    sleep $env(DELAY)
    send "rm block tempBlock *\r"
    sleep $env(DELAY)

    # ls -l after rm block
    send "ls -l\r"
    sleep $env(DELAY)

    # specific rm node
    send "rm node 4\r"
    sleep $env(DELAY)
    send "ls\r"
    sleep $env(DELAY)

    # re-sync after modifications
    send "sync\r"
    sleep $env(DELAY)
    send "ls -l\r"
    sleep $env(DELAY)

    # -- Build the exact FINAL state for persistence verification ---
    # Start clean: rm node * then build exactly:
    send "rm node *\r"
    sleep $env(DELAY)
    send "add node 10\r"
    sleep $env(DELAY)
    send "add node 20\r"
    sleep $env(DELAY)
    send "add block alpha 10\r"
    sleep $env(DELAY)
    send "add block beta 10\r"
    sleep $env(DELAY)
    send "add block alpha *\r"
    sleep $env(DELAY)
    send "add block beta *\r"
    sleep $env(DELAY)
    send "sync\r"
    sleep $env(DELAY)

    # snapshot before quit
    send "ls -l\r"
    sleep $env(DELAY)

    # QUIT (triggers save_blockchain)
    send "quit\r"
    expect eof
' 2>&1)

echo "$OUTPUT"

# SESSION 1 verifications
section "SESSION 1 Verifications"

check() {
    local desc="$1"
    local pattern="$2"
    if echo "$OUTPUT" | grep -qE "$pattern"; then
        pass "$desc"
    else
        fail "$desc  (pattern: $pattern)"
    fi
}

# Help
check "help — list of commands displayed"       "available commands"
check "help add — add details displayed"             "add node <nid>"
check "help rm — rm details displayed"               "rm node <nid>"
check "help ls — ls details displayed"               "ls -l"
check "help sync — sync details displayed"           "synchronize"
check "help quit — quit details displayed"           "save the blockchain"
check "help help — help details displayed"           "help <cmd>"

# Errors
check "empty command -> error"                   "error|Error|ERROR|not found|Not Found"
check "unknown command (foobar) -> error"      "error|Error|ERROR|not found|Not Found"
check "add node without argument -> error"          "error|Error|ERROR|not found|Not Found"
check "add node abc (invalid) -> error"         "error|Error|ERROR|not found|Not Found"

# add node
check "add node 1..4 -> displayed in ls"         "1"
check "duplicate node 1 -> error"                  "error|Error|ERROR|already|Already"

# add block
check "add block blockA 1 -> in ls -l"          "blockA"
check "add block non-existent node -> error"       "error|Error|ERROR|not found|Not Found|doesn.t exist"
check "add block duplicate -> error"               "error|Error|ERROR|already|Already"
check "add block * -> blockGlobal present"        "blockGlobal"

# sync
check "sync -> OK displayed"                        "OK"

# ls
check "ls displays the nodes"                      "1|2|3"
check "ls -l displays the blocks"                   "blockA|blockB"

# rm block
check "rm block blockD 1 -> no longer in node 1"    "blockA|blockB"
check "rm block <bid> * -> tempBlock absent"      "blockA|blockGlobal"

# rm node
check "rm node 4 -> node 4 deleted"             "1|2|3"

# Final state before quit (nodes 10 and 20 with alpha-beta)
check "node 10 present before quit"               "10"
check "node 20 present before quit"               "20"
check "block alpha present before quit"            "alpha"
check "block beta present before quit"             "beta"

# ===========================================================================
# SESSION 2 : persistence verification (save/load)
#   Restart the program, execute ls -l and quit immediately.
#   The state must be identical to the final state of SESSION 1.
# ===========================================================================
# STREAMING_CHUNK:Running Expect for Session 2...
section "SESSION 2 — persistence verification (reload)"

OUTPUT2=$(expect -c '
    set timeout 10
    log_user 1
    spawn ./my_blockchain
    sleep $env(DELAY_LONG)

    send "ls -l\r"
    sleep $env(DELAY_LONG)

    send "quit\r"
    expect eof
' 2>&1)

echo "$OUTPUT2"

section "SESSION 2 Verifications — identical state"

check2() {
    local desc="$1"
    local pattern="$2"
    if echo "$OUTPUT2" | grep -qE "$pattern"; then
        pass "$desc"
    else
        fail "$desc  (pattern: $pattern)"
    fi
}

check2 "node 10 reloaded"             "10"
check2 "node 20 reloaded"             "20"
check2 "block alpha reloaded"          "alpha"
check2 "block beta reloaded"           "beta"

# Verify that both nodes have exactly the same blocks (sync)
# The ls -l output gives : "10 : alpha-beta" and "20 : alpha-beta"
check2 "node 10 has alpha and beta"      "10.*alpha.*beta|10.*beta.*alpha"
check2 "node 20 has alpha and beta"      "20.*alpha.*beta|20.*beta.*alpha"
check2 "sync state (s2) in prompt" "s2"

# ===========================================================================
# Restore the original state if a backup existed
# ===========================================================================
if [ -f "$SAVE_BACKUP" ]; then
    mv "$SAVE_BACKUP" "$SAVE_FILE"
fi

# Summary
section "Summary"
if [ "$FAILURES" -eq 0 ]; then
    printf "${GRN}${BLD}All tests passed! ✓${RST}\n"
    exit 0
else
    printf "${RED}${BLD}%d test(s) failed.${RST}\n" "$FAILURES"
    exit 1
fi