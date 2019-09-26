compile I2C:
	gcc i2c.c -o i2c
compile uart:
	uart2tr: gcc -Wall -o uart2tr uart2tr.c -lwiringPi -lpthread
	uart2rc: gcc -Wall -o uart2rc uart2rc.c -lwiringPi -lpthread
