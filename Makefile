CXX = g++
CFLAGS = -std=c++11 -Wall

all: mapreduce sortfile

mapreduce: file_generate.o is_file_exist.o is_sorted.o main.o partition.o reduce.o run_process.o
	$(CXX) file_generate.o is_file_exist.o is_sorted.o main.o partition.o reduce.o run_process.o -o mapreduce
	
file_generate.o: file_generate.hpp file_generate.cpp
	$(CXX) -c $(CFLAGS) file_generate.cpp -o file_generate.o
	
is_file_exist.o: is_file_exist.cpp is_file_exist.h
	$(CXX) -c $(CFLAGS) is_file_exist.cpp -o is_file_exist.o
	
is_sorted.o: is_sorted.cpp is_sorted.hpp
	$(CXX) -c $(CFLAGS) is_sorted.cpp -o is_sorted.o
	
main.o: main.cpp run_process.h partition.h file_generate.hpp is_file_exist.h is_sorted.hpp reduce.hpp
	$(CXX) -c $(CFLAGS) main.cpp -o main.o
	
partition.o: partition.cpp partition.h
	$(CXX) -c $(CFLAGS) partition.cpp -o partition.o
	
reduce.o: reduce.cpp reduce.hpp 
	$(CXX) -c $(CFLAGS) reduce.cpp -o reduce.o
	
run_process.o: run_process.cpp run_process.h
	$(CXX) -c $(CFLAGS) run_process.cpp -o run_process.o
	
sortfile: sortfile.cpp
	$(CXX) $(CFLAGS) sortfile.cpp -o sortfile
	
clean:
	rm -f file_generate.o is_file_exist.o is_sorted.o main.o partition.o reduce.o run_process.o sortfile mapreduce
