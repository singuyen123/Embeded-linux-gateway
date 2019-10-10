#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "uart.h"

int main ()
{
  int serial_port ;
  char dat;
  if ((serial_port = serialOpen ("/dev/ttyS2", BAUD)) < 0)	/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while(1){
	 // serialPuts(serial_port,"NamBang");
      
	if(serialDataAvail (serial_port) )
	{ 

		dat = serialGetchar (serial_port);		/* receive character serially*/	
		printf ("%c", dat) ;
		fflush (stdout) ;
		serialPutchar(serial_port, dat);		/* transmit character serially on port */
	//	serial_port=-1;
	break;	
	 }
}
}
