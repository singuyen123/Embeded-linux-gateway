OBJSDIR	=	./src
#source, executable, includes, library Defines
INCL	=	./src/uart.h
SRC		+=	./src/uart.c \
			ClientUART.c
OBJ		=	$(SRC:.c=.o)
EXE		= 	clientUART


#compiler, Linker Defines
CC		=	gcc
#CFLAGS  = -shared -Wall -fPIC -ltr -lpthread
LIBPATH =  -pthread 
LDFLAGS	=	-o $(EXE) $(LIBPATH)


#compile and assemble C souurce Files into Object Files
$(OBJSDIR)/%.o: %.c
	$(CC) -c $*.c 
#Link all object files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
#$(EXE2): $(OBJ)	
#	$(CC) $(LDFLAGS2) $(OBJ)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
.PHONY: clean
clean: 
	rm  $(OBJ) $(EXE)
