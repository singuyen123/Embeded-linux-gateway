#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

void readdata(int serial_port )
{
  // Allocate memory for read buffer, set size according to your needs
char read_buf [256];
memset(&read_buf, '\0', sizeof(read_buf));

// Read bytes. The behaviour of read() (e.g. does it block?,
// how long does it block for?) depends on the configuration
// settings above, specifically VMIN and VTIME
int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
if (num_bytes < 0) {
    printf("Error reading: %s", strerror(errno));
}

// Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
// print it to the screen like this!)
printf("Read %i bytes. Received message: %s\n", num_bytes, read_buf);
}
int main ()
{
  int serial_port ;
  
  if ((serial_port = serialOpen ("/dev/ttyS2", 9600)) < 0)	/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while(1){
	 // serialPuts(serial_port,"NamBang");
      
	if(serialDataAvail (serial_port) )
	{ 
		readdata(serial_port);
	}

	}
}
