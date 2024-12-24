all:derle bagla calistir
derle:
	g++ -c -I "./include" ./src/main.cpp -o ./lib/main.o
	g++ -c -I "./include" ./src/LinkedList.cpp -o ./lib/LinkedList.o
	g++ -c -I "./include" ./src/ListNode.cpp -o ./lib/ListNode.o
	g++ -c -I "./include" ./src/Tree.cpp -o ./lib/Tree.o
	g++ -c -I "./include" ./src/TreeNode.cpp -o ./lib/TreeNode.o
bagla:
	g++ ./lib/main.o ./lib/LinkedList.o ./lib/ListNode.o ./lib/Tree.o ./lib/TreeNode.o  -o ./bin/program
calistir:
	./bin/program