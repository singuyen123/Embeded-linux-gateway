#source, executable, includes, library Defines
INCL	+=	../../common/data.h	\
			../../library/RF24/RF24.h	\
			../../library/RF24/nRF24L01.h	\
			../../library/RF24/RF24_config.h	\
			../../library/RF24/printf.h	

SRC		+=	clientSPI.cpp	\
			../../RF24.cpp
OBJ		=	$(SRC:.cpp=.o)
EXE		= 	clientSPI


#compiler, Linker Defines
CC		=	g++
CFLAGS  =  -ltr 
LIBPATH =  -pthread 
LDFLAGS	=	 -o $(EXE) $(LIBPATH)


#compile and assemble C souurce Files into Object Files
$(OBJSDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp 
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

