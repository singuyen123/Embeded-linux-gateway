#source, executable, includes, library Defines
INCL	+=	../../common/data.h	

SRC		+=	clientSPI.c	
			
OBJ		=	$(SRC:.c=.o)
EXE		= 	clientSPI


#compiler, Linker Defines
CC		=	g++
CFLAGS  =  -ltr 
LIBPATH =  -pthread 
LDFLAGS	=	-o $(EXE) $(LIBPATH)


#compile and assemble C souurce Files into Object Files
$(OBJSDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $*.c
#Link all object files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -lwiringPi
#$(EXE2): $(OBJ)	
#	$(CC) $(LDFLAGS2) $(OBJ)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
.PHONY: clean
clean: 
	rm  $(OBJ) $(EXE)

