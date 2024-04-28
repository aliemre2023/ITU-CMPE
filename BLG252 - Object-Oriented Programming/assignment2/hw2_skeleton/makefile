# # # # OOP Assignment 2
# You can define any variable you want. I prefer to define
# the compiler flags and the list of all object files that 
# will be used to create the executable file. 

# # # This is a readible version of the makefile. See below for a more compact version.

CXXFLAGS := -Iinclude -std=c++17 -Wall -Werror -g
ALL_OBJECTS := bin/main.o bin/simulation.o bin/factory.o bin/machine.o bin/headworker.o bin/worker.o bin/unit.o


bin/main: $(ALL_OBJECTS)
	g++ $(ALL_OBJECTS) $(CXXFLAGS) -o bin/main

bin/unit.o: src/unit.cpp include/*.hpp
	g++ -c src/unit.cpp $(CXXFLAGS) -o bin/unit.o

bin/worker.o: src/worker.cpp include/*.hpp
	g++ -c src/worker.cpp $(CXXFLAGS) -o bin/worker.o

bin/headworker.o: src/headworker.cpp include/*.hpp
	g++ -c src/headworker.cpp $(CXXFLAGS) -o bin/headworker.o

bin/machine.o: src/machine.cpp include/*.hpp
	g++ -c src/machine.cpp $(CXXFLAGS) -o bin/machine.o

bin/factory.o: src/factory.cpp include/*.hpp
	g++ -c src/factory.cpp $(CXXFLAGS) -o bin/factory.o

bin/simulation.o: src/simulation.cpp include/*.hpp
	g++ -c src/simulation.cpp $(CXXFLAGS) -o bin/simulation.o

bin/main.o: src/main.cpp include/*.hpp
	g++ -c src/main.cpp $(CXXFLAGS) -o bin/main.o


valgrind: bin/main
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_log.txt ./bin/main 

test: bin/main
	python3 -m calico.cli hw2_tests.yaml

run: bin/main
	./bin/main

.PHONY: clean
clean:
	rm -f bin/*.o bin/main


# # # # This is a more compact and generic version of the makefile.
# INC := include
# BIN := bin
# SRC := src

# CXXFLAGS := -Iinclude -std=c++17 -Wall -Werror -g
# SRCS := $(wildcard $(SRC)/*.cpp)
# OBJS := $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SRCS))

# $(BIN)/main: $(OBJS)
# 	g++ $^ $(CXXFLAGS) -o $@

# $(BIN)/%.o: $(SRC)/%.cpp $(INC)/*.hpp
# 	g++ -c $< $(CXXFLAGS) -o $@

# valgrind: $(BIN)/main
# 	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_log.txt ./$(BIN)/main

# run: $(BIN)/main
# 	./$(BIN)/main

# .PHONY: clean
# clean:
# 	rm -f $(BIN)/*.o $(BIN)/main