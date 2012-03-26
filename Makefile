#C++
GCC=g++

VPATH=src

OBJ=obj

#All relevant -I options here
INCLUDES= -I src

#Add other options here
CFLAGS=-O5 $(INCLUDES)

#There must be one line per program
PROGRAM_OBJS=types.o utils.o parameters.o graph_builder.o graph_builder_txt.o vertex.o graph.o graph_social.o connectedcomponent.o community.o log.o solution_lanci.o main.o

#move must be at the end
all: commugp 

commugp: $(PROGRAM_OBJS)
	@echo Linking $@...
	@$(GCC) -o $@ $(CFLAGS) $+

#Common rules
.SUFFIXES: .cpp .o
.cpp.o:
	@echo Compiling $<
	@$(GCC) $(CFLAGS) -c $<

#Clean up
clean:
	@rm -f src/*.o
	@rm -f *.o
	@rm -f src/*~
	@rm -f *~

