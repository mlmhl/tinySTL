
OBJS = main.o vector_test.o list_test.o deque_test.o stack_test.o queue_test.o heap_test.o tree_test.o

main : ${OBJS}
	g++ -g -std=gnu++11 -o main ${OBJS}

main.o : main.cpp vector_test.h list_test.h deque_test.h stack_test.h queue_test.h heap_test.h tree_test.h
	g++ -c -g -std=gnu++11 -o main.o main.cpp

vector_test.o : vector_test.h vector_test.cpp ../ml_vector.h
	g++ -c -g -std=gnu++11 -o vector_test.o vector_test.cpp

list_test.o : list_test.h list_test.cpp ../ml_list.h
	g++ -c -g -std=gnu++11 -o list_test.o list_test.cpp

deque_test.o : deque_test.h deque_test.cpp ../ml_deque.h
	g++ -c -g -std=gnu++11 -o deque_test.o deque_test.cpp

stack_test.o : stack_test.h stack_test.cpp ../ml_stack.h
	g++ -c -g -std=gnu++11 -o stack_test.o stack_test.cpp

queue_test.o : queue_test.h queue_test.cpp ../ml_queue.h
	g++ -c -g -std=gnu++11 -o queue_test.o queue_test.cpp

heap_test.o : heap_test.h heap_test.cpp ../ml_heap.h
	g++ -c -g -std=gnu++11 -o heap_test.o heap_test.cpp

tree_test.o : tree_test.h tree_test.cpp ../ml_tree.h
	g++ -c -g -std=gnu++11 -o tree_test.o tree_test.cpp

clean : 
	rm -rf main ${OBJS}
