CXX = gcc
CXXFLAGS = -g -Wall -Wextra -std=c++1y 
.PHONY: client 

COMMONDIR := ../common
COMMONOBJS := $(wildcard $(COMMONDIR)/*.o)

INCDIRS := -I$(COMMONDIR) 
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

 %.o: %.c
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@ 

client: $(OBJECTS) 	
	@echo "Building client..." 	
	$(CXX) $(CXXFLAGS) $(COMMONOBJS) $(INCDIRS) $^ -o $@ 
clean: 	
	@rm -f client 	
	@rm -f $(OBJECTS) 
all: client