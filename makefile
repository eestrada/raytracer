BIN = ./test.bin
INPUT = ./scenes/diffuse.rayTracing
OUTPUT = ./tmp.ppm
ARGS = $(INPUT) $(OUTPUT)
DISPLAY = display

SRCDIR = ./src
INCDIR = ./inc
OBJDIR = ./obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
INCS = $(wildcard $(INCDIR)/*.h*) $(wildcard $(INCDIR)/cg/*.h*) $(wildcard $(INCDIR)/utils/*.h*) $(wildcard $(INCDIR)/cg/*.inl) 

INCLUDES= -I$(INCDIR)/
CXX ?= c++
CXXFLAGS = -std=c++0x -O0 -g -Wall -pedantic $(INCLUDES)

.PHONY : run bin test clean memcheck

run : $(BIN)
	@ echo "Testing executable"
	$(BIN) $(ARGS)

bin : $(BIN)

test : clean memcheck

clean :
	@ echo "Removing generated files"
	@rm -vf $(BIN)
	@rm -vf $(OUTPUT)
	@rm -vrf $(OBJDIR)

memcheck : $(BIN) 
	@ echo "Running valgrind to check for memory leaks"
	valgrind --tool=memcheck --leak-check=yes --max-stackframe=5000000 \
	--show-reachable=yes $(BIN) $(ARGS)
	@ echo

$(BIN) : $(OBJS) $(INCS)
	@ echo "Compiling binary"
	$(CXX) -o $(BIN) $(OBJS) $(CXXFLAGS) $(INCLUDES)
	@ echo

obj/%.o : src/%.cpp $(INCS)
	@- mkdir -p $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@ echo
