#Forder chua .o sau khi build
OBJSDIR	=	./src
# Source, Executable, Includes, Library DefinesCC=gcc
INCL   +=  ./src/i2c.h \
			./src/i2cbusses.h  \
			./src/smbus.h	\
			./src/timer.h

SRC    += 	./src/smbus.c  \
			./src/i2cbusses.c	\
			./src/timer.c	\
			 clientI2C.c 
			 
OBJ = $(SRC:.c=.o)
EXE = clientI2C
# Compiler, Linker Defines
CC = gcc
LIBPATH =  -pthread 
LDFLAGS =  -o  $(EXE) $(LIBPATH)

# Compile and Assemble C Source Files into Object Files
$(OBJSDIR)/%.o: %.c 
	$(CC) -c  $*.c

# Link all Object Files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
#debug:
#	$(CC) $(CFDEBUG) $(SRC)

# Objects depend on these Libraries
$(OBJ):$(INCL)

# Clean Up Objects, Exectuables, Dumps out of source directory
.PHONY: clean
clean: 
	rm  $(OBJ) $(EXE)
