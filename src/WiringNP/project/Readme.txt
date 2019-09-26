compile I2C:
	gcc i2c.c -o i2c
compile uart:
	uart2tr: gcc -Wall -o uart2tr uart2tr.c -lwiringPi -lpthread
	uart2rc: gcc -Wall -o uart2rc uart2rc.c -lwiringPi -lpthread
setup: 
	uart: rx(npi)-tx(arduino); tx(npi)-rx(arduino); GND-GND
	I2C: SDA(npi)-SDA(arduino); SCL(npi)-SCL(arduino); GND-GND; resistor pull up SDA&&SCL
run:
	uart: sudo ./uart2tr  and sudo ./uart2rc. Ex transfer: echo IOT > /dev/ttyS2
	I2C
