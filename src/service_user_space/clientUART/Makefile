OBJSDIR	=	./src
#source, executable, includes, library Defines
INCL	+=	./src/uart.h \
			../../common/data.h  \
      ./src/wiringSerial1.h		\
	  ./src/timer.h
SRC		+=	./src/uart.c \
			ClientUART.c  \
			./src/timer.c \
      ./src/wiringSerial1.c
OBJ		=	$(SRC:.c=.o)
EXE		= 	clientUART #bluetooth
#bluetooth.c	

#compiler, Linker Defines
CC		=	gcc
CFLAGS  =  -ltr 
LIBPATH =  -pthread 
LDFLAGS	=	 -o $(EXE) $(LIBPATH)


#compile and assemble C souurce Files into Object Files
$(OBJSDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $*.c 
#Link all object files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -ljson-c -lpaho-mqtt3c 
#$(EXE2): $(OBJ)	
#	$(CC) $(LDFLAGS2) $(OBJ)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
.PHONY: clean
clean: 
	rm  $(OBJ) $(EXE)
