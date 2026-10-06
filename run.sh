make re && make clean && clear && clear && expect -c '
spawn ./my_blockchain
sleep 1
send "ls\r"
sleep 0.2
send "add node 1\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "add node 2\r"
sleep 0.2
send "add node 1\r"
sleep 0.2
send "add node a\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "add node 3\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "rm node 1\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "rm node 3\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "rm node 2\r"
sleep 0.2
send "ls\r"
sleep 0.2
send "quit\r"
interact
' && ./my_blockchain