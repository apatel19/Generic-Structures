OBJS = integer.o sll.o dll.o stack.o queue.o real.o string.o bst.o heap.o heapsort.o scanner.o bst-0-0.o heap-0-0.o 
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : test-bst test-heap heapsort test-sll test-dll test-stack test-queue

test-bst	: bst-0-0.o integer.o real.o string.o bst.o queue.o 
	gcc $(LOPTS) bst-0-0.o integer.o real.o string.o bst.o queue.o -o test-bst

test-heap	: heap-0-0.o integer.o real.o string.o heap.o bst.o queue.o stack.o
	gcc $(LOPTS) heap-0-0.o integer.o real.o string.o heap.o bst.o queue.o stack.o -o test-heap

heapsort	: heapsort.o integer.o real.o string.o heap.o scanner.o bst.o queue.o stack.o
	 gcc -Wall -Wextra -g heapsort.c integer.c real.c string.c scanner.c heap.c bst.c stack.c queue.c -o heapsort

test-sll : test-sll.o integer.o sll.o
	gcc $(LOPTS) test-sll.o integer.o sll.o  -o test-sll

test-dll : test-dll.o integer.o dll.o
	gcc $(LOPTS) test-dll.o integer.o dll.o -o test-dll

test-stack : test-stack.o integer.o dll.o stack.o
	gcc $(LOPTS) test-stack.o integer.o dll.o stack.o -o test-stack

test-queue : test-queue.o integer.o sll.o queue.o
	gcc $(LOPTS) test-queue.o integer.o sll.o queue.o -o test-queue

bst-0-0.o : bst-0-0.c integer.h real.h string.h bst.h queue.h stack.h
	gcc $(OOPTS) bst-0-0.c

heap-0-0.o : heap-0-0.c integer.h real.h string.h heap.h bst.h queue.h stack.h
	gcc $(OOPTS) heap-0-0.c

heapsort.o : heapsort.c integer.h real.h string.h scanner.h queue.h stack.h bst.h heap.h
	gcc $(OOPTS) heapsort.c

test-sll.o : test-sll.c sll.h integer.h
	gcc $(OOPTS) test-sll.c

test-dll.o : test-dll.c dll.h integer.h
	gcc $(OOPTS) test-dll.c

test-stack.o : test-stack.c stack.h integer.h
	gcc $(OOPTS) test-stack.c

test-queue.o : test-queue.c queue.h integer.h
	gcc $(OOPTS) test-queue.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

heap.o : heap.c heap.h
	gcc $(OOPTS) heap.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

hqueue.o : hqueue.c hqueue.h
	gcc $(OOPTS) hqueue.c

btree.o : btree.c btree.h
	gcc $(OOPTS) btree.c

stack.o : stack.c stack.h
	gcc $(OOPTS) stack.c

queue.o : queue.c queue.h dll.h
	gcc $(OOPTS) queue.c

sll.o : sll.c sll.h
	gcc $(OOPTS) sll.c

dll.o : dll.c dll.h
	gcc $(OOPTS) dll.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

valgrind  : all
	echo testing singly-linked list
	valgrind ./test-sll
	echo
	echo testing doubly-linked list
	valgrind ./test-dll
	echo
	echo testing stack
	valgrind ./test-stack
	echo
	echo testing queue
	valgrind ./test-queue
	echo
	echo testing bst
	valgrind ./test-bst
	echo
	echo testing heap
	valgrind ./test-heap
	echo

test	: all
	echo testing singly-linked list
	./test-sll
	echo
	echo testing doubly-linked list
	./test-dll
	echo
	echo testing stack
	./test-stack
	echo
	echo testing queue
	./test-queue
	echo
	echo testing bst
	./test-bst
	echo
	echo testing heap
	./test-heap
	echo testing heapsort
	./heapsort hs-0-0.data
	echo

clean    :
	rm -f $(OBJS) test-*.o test-stack test-queue test-sll test-dll test-heap test-bst heapsort   
