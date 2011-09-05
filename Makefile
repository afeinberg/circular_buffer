CXX = g++
CPPFLAGS = -I$(GTEST_DIR)/include
CXXFLAGS = -g -Wall -Wextra -pthread -std=c++0x

GTEST_DIR = ../gtest-1.6.0-svn
GTEST_LIBS = ${GTEST_DIR}/libgtest.a

HEADERS = circular_buffer.h trivial_allocator.h test_common.h
TESTS = circular_buffer_test trivial_allocator_test

all: ${TESTS}

clean:
	rm -f $(TESTS) *.o

circular_buffer_test.o:  circular_buffer_test.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c circular_buffer_test.cc

circular_buffer_test: circular_buffer_test.o $(GTEST_LIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

trivial_allocator_test.o: trivial_allocator_test.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c trivial_allocator_test.cc

trivial_allocator_test: trivial_allocator_test.o $(GTEST_LIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

test: ${TESTS}
	./trivial_allocator_test
	./circular_buffer_test
