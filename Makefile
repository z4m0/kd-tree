CXX=g++
RM=rm -f
FLAGS=-std=c++11


kdtree.o: kd_tree.hpp kd_tree.cpp
	$(CXX) $(FLAGS) -c kd_tree.cpp

test: kdtree.o
	$(CXX) $(FLAGS) -o test_kdtree test.cpp kd_tree.o
	time ./test_kdtree

clean:
	$(RM) *.o kd_tree
