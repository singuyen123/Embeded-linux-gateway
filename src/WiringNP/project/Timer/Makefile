#Source, Executable, Includes, Library DefinessCC=gcc
INCL	=	curr_time.h itimerspec_from_str.h tlpi_hdr.h get_num.h error_functions.h
SRC		=	ptmr_sigev_thread.c curr_time.c itimerspec_from_str.c
OBJ		=	$(SRC:.c=.o)
EXE		=	ptmr_sigev_thread 

#compiler, Linker Defines
CC	=	gcc
LIBPATH = -L.
LDFLAGS	=	-o $(EXE) $(LIBPATH)
CFLAGS	=	-pthread -lrt -Wall -ldl -lacl -lcrypt -lcap

#compile and assemble C source Files 
%.o: %.c
	$(CC) -c $(CFLAGS) $*.c
#link all Object Files with external libraries into binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	$(CC) $(CFDEBUG) $(SRC)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
clean: 
	rm $(OBJ)