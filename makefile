BIN = ./test.bin
INPUT = ./scenes/diffuse.rayTracing
OUTPUT = ./diffuse.ppm
ARGS ?= $(INPUT) $(OUTPUT)
ARGS1 ?= $(INPUT) $(OUTPUT)
ARGS2 ?= ./scenes/SceneII.rayTracing ./SceneII.ppm
DISPLAY = display

SRCDIR = ./src
INCDIR = ./inc
OBJDIR = ./obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
INCS = $(wildcard $(INCDIR)/*.h*) $(wildcard $(INCDIR)/rt/*.*) $(wildcard $(INCDIR)/cg/*.*) $(wildcard $(INCDIR)/utils/*.*) 

INCLUDES= -I$(INCDIR)/
CXX ?= c++
CXXFLAGS = -std=c++0x -O0 -g -Wall -pedantic $(INCLUDES)

.PHONY : run bin test clean memcheck

run : $(BIN)
	@ echo "Testing executable"
	$(BIN) $(ARGS1)
	$(BIN) $(ARGS2)

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
