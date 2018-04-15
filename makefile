# Makefile to compile
# Variables with the options for compilation
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# List of the objects to compile
OBJECTS = state.o pda.o main.o
# Variables with rule names
PDA = pda

# Default rule
all: $(PDA)

# Rule to compile object files
# Commented because it is the same as an explicit rule in Make
#   https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_10.html
#%.o: %.c
	#$(CXX) -c -o $@ $< $(CXXFLAGS)

# Rule to compile the program
$(PDA): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	-rm -f *.o

.PHONY: all clean
