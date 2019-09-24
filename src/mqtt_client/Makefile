CC=gcc	#compiler
TARGET=mqtt_client.o	#target file name
 
all:	mqtt_client.o
	$(CC) mqtt_client.c -o $(TARGET) -lpthread -lpaho-mqtt3c
 
clean:
	rm $(TARGET)