
#CC=gcc	#compiler
#TARGET=mqtt_client.o	#target file name
#COMMONDIR := ../common
 
#all:	mqtt_client.o
#	$(CC) mqtt_client.c -I$(COMMONDIR) -o $(TARGET) -lpthread -lpaho-mqtt3c
#
#clean:
#	rm $(TARGET)

CXX = gcc
CXXFLAGS = -g -Wall -Wextra -std=c++1y 
.PHONY: mqtt_client 

COMMONDIR := ../common
COMMONOBJS := $(wildcard $(COMMONDIR)/*.o)

INCDIRS := -I$(COMMONDIR) 
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

 %.o: %.c
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@ 

mqtt_client: $(OBJECTS) 	
	@echo "Building mqtt_client..." 	
	$(CXX) $(CXXFLAGS) $(COMMONOBJS) $(INCDIRS) $^ -o $@ -lpthread -lpaho-mqtt3c
clean: 	
	@rm -f mqtt_client 	
	@rm -f $(OBJECTS) 
all: mqtt_client