
# Source, Executable, Includes, Library DefinesCC=gcc
INCL   = ../common/data.h i2c.h i2cbusses.h smbus.h
SRC    = clientI2C.c i2cbusses.c smbus.c   
OBJ = $(SRC:.c=.o)
#LIBS   = -lgen
EXE = clientI2C
# Compiler, Linker Defines
CC = gcc
CFLAGS  = -Wall  -ltr  
LIBPATH = -L. -pthread 
LDFLAGS = -o $(EXE) $(LIBPATH)
#CFDEBUG = -ansi -pedantic -Wall -g -DDEBUG $(LDFLAGS)

# Compile and Assemble C Source Files into Object Files

%.o: %.c 
	$(CC) -c  $(CFLAGS) $*.c 
# Link all Object Files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
# Create a gdb/dbx Capable Executable with DEBUG flags turned on
#debug:
#	$(CC) $(CFDEBUG) $(SRC)
# Objects depend on these Libraries
$(OBJ): $(INCL)
# Clean Up Objects, Exectuables, Dumps out of source directory
clean: 
	rm $(OBJ)
