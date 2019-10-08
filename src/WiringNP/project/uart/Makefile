#source, executable, includes, library Defines
INCL	=	uart.h
SRC		=	uart.c  uart2tr.c 
SRC2 	=   uart2rc.c uart.c
OBJ		=	$(SRC:.c=.o)
EXE1		= uart2tr 
EXE2		= uart2rc

#compiler, Linker Defines
CC		=	gcc
CFLAGS  = -shared -Wall -fPIC -ltr -lpthread
LDFLAGS1	=	-o $(EXE1)
LDFLAGS2	=	-o $(EXE2)

#compile and assemble C souurce Files into Object Files
%.o: %.c
	$(CC) -c $(CFLAGS) $*.c 
#Link all object files with external Libraries into Binaries
$(EXE1): $(OBJ)
	$(CC) $(LDFLAGS1) $(OBJ)
#$(EXE2): $(OBJ)	
#	$(CC) $(LDFLAGS2) $(OBJ)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
clean: 
	rm $(OBJ)
