src = $(wildcard *.cpp)
obj = $(src:.o)
CXX = g++
LDFLAGS= -Wall -pedantic -std=c++11


sudoku_solver: $(obj)
	$(CXX) $(src) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) sudoku_solver
