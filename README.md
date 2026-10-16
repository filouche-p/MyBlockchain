# MyBlockchain

> A lightweight blockchain simulator written in C, operated through an interactive CLI.

MyBlockchain is a personal project built around the manipulation of **linked lists of linked lists**  each node holds its own chain of blocks. The entire program runs with a minimal set of allowed C functions: `malloc`, `free`, `printf`, `write`, `open`, `read`, `close`.

---

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation & Build](#installation--build)
- [Usage](#usage)
- [Commands](#commands)
- [Project Structure](#project-structure)
- [License](#license)

---

## Features

- Interactive CLI with a live status prompt
- Add / remove nodes and blocks with wildcard (`*`) support
- Sync the entire blockchain so every node holds identical blocks
- Automatic save & restore via a `.blockchain.save` file

---

## Requirements

| Dependency | Notes |
|---|---|
| C compiler | `gcc` or `clang` |
| `make` | Optional but recommended |
| Read/write permissions | Required for the save file |

> **Memory usage (worst case):** 10 nodes × 10 blocks each after a full sync ≈ **5.75 MB RAM**, **0.01s CPU**.
> **Disk usage (total):** .
> **Disk usage (utils):** .

---

## Installation & Build

```bash
# Clone the repository
git clone https://github.com/filouche-p/MyBlockchain.git
cd MyBlockchain
```

### With Make *(recommended)*

```bash
# Edit line 2 of the Makefile to point to your compiler if needed
make        # build
make clean  # remove object files
make fclean # full clean (binary included)
make re     # clean rebuild
```

### Without Make

```bash
gcc -o my_blockchain my_blockchain.c libs/src/*.c
```

---

## Usage

```bash
./my_blockchain        # Linux / macOS
```

### The prompt

```
[s3]> 
```

| Character | Meaning |
|---|---|
| `s` | Blockchain is **synced** |
| `-` | Blockchain is **not synced** |
| `3` | Number of nodes currently in the chain |

### Save file

On quit (command), the blockchain is saved to `.blockchain.save` in the working directory and automatically reloaded on the next launch.

> **Warning:** The save file is human-readable but the program does not handle a corrupted or manually edited file. Rename, move, or delete it if you want to start fresh.

---

## Commands

| Command | Syntax | Description |
|---|---|---|
| `help` | `help` \| `help <cmd>` | List all commands, or get help for a specific one |
| `add` | `add node <nid>` \| `add block <nid> <bid>` | Add a node or a block. Supports `*` as a wildcard |
| `rm` | `rm node <nid>` \| `rm block <nid> <bid>` | Remove a node or a block. Supports `*` as a wildcard |
| `ls` | `ls` \| `ls -l` | List nodes. `-l` also displays each node's blocks |
| `sync` | `sync` | Distribute all blocks to every node so they are identical |
| `quit` | `quit` | Save the blockchain and exit cleanly |

### Identifiers

- **nid** — node ID, a **unique integer** across the blockchain
- **bid** — block ID, a **unique string** within a node's block list

### `ls -l` output format

Nodes are listed vertically, their blocks horizontally:

```
node 1 : block-a -> block-b -> block-c
node 2 : block-c -> block-a -> block-b
node 3 : (empty)
```

### `sync` explained

After syncing, every node holds the **same set of blocks** (order may vary):

```
Before sync:          After sync:
  node 1: A B C        node 1: A B C
  node 2: C            node 2: C A B
  node 3: (empty)      node 3: A B C
```

### Error messages

| Message | Likely cause |
|---|---|
| `nok: no more resources available` | Out of memory |
| `nok: this node already exists or the nid is not valid` | Duplicate nid or non-numeric nid |
| `nok: this block already exists` | Duplicate bid on that node |
| `nok: node doesn't exist` | Wrong nid |
| `nok: block doesn't exist` | Wrong bid |
| `nok: command not found` | Typo or unsupported command |

If an error persists after checking the syntax (`help <cmd>`), verify available RAM and file permissions before opening an issue.

---

## Project Structure

```
MyBlockchain/
├── libs/
│   ├── headers/
│   │   ├── backup_manager.h   # Save & load logic
│   │   ├── block_manager.h    # Block add / remove
│   │   ├── command_handler.h  # Command dispatch
│   │   ├── command_sup.h      # Command helpers
│   │   ├── node_manager.h     # Node add / remove
│   │   └── utils.h            # Shared types & utilities
│   └── src/                   # Corresponding .c implementations
├── my_blockchain.c            # Entry point
├── Makefile
└── compile.sh
```

---

## License

MIT — free to use, modify, and distribute.