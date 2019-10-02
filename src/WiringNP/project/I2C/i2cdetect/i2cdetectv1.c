#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <smbus.h>
#include "i2cbusses.h"
#include "i2c.h"
#include <fcntl.h>

#define MODE_AUTO	0
#define MODE_QUICK	1
#define MODE_READ	2
#define MODE MODE_AUTO
#define FIRST 0x03
#define LAST  0x77
#define I2CBUS  0

volatile int address=0;
static const char *device = "/dev/i2c-0";
const char key[4]={1,9,9,7};
int wiringPiI2CSetupInterface (const char *device, int devId)
{
  int fd ;

  if ((fd = open (device, O_RDWR)) < 0)
  {
    fprintf(stderr, "I2C: Failed to access %d\n", device);
    exit(1);

  }
    

  if (ioctl (fd, I2C_SLAVE, devId) < 0)
  {
    fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", devId);
    exit(1);
  }
  return fd ;
}

static int scan_i2c_bus(int mode,int first, int last)
{
	int i, j;
	int cmd, res;
	int file;
	//char filename[20];
	//file = open_i2c_dev(I2CBUS, filename, sizeof(filename), 0);
	file=open (device, O_RDWR);
	if (file < 0) {
		exit(1);
	}

	for (i = 0; i < 128; i += 16) {
		for(j = 0; j < 16; j++) {
			fflush(stdout);
			/* Select detection command for this address */
			switch (mode) {
			default:
				cmd = mode;
				break;
			case MODE_AUTO:
				if ((i+j >= 0x30 && i+j <= 0x37)
				 || (i+j >= 0x50 && i+j <= 0x5F))
				 	cmd = MODE_READ;
				else
					cmd = MODE_QUICK;
				break;
			}

			/* Set slave address */
			if (ioctl(file, I2C_SLAVE, i+j) < 0) {
				if (errno == EBUSY) {
					printf("UU ");
					continue;
				} else {
					fprintf(stderr, "Error: Could not set "
						"address to 0x%02x: %s\n", i+j,
						strerror(errno));
					return -1;
				}
			}

			/* Probe this address */
			switch (cmd) {
			default: /* MODE_QUICK */
				/* This is known to corrupt the Atmel AT24RF08
				   EEPROM */
				res = i2c_smbus_write_quick(file,
				      I2C_SMBUS_WRITE);
				break;
			case MODE_READ:
				/* This is known to lock SMBus on various
				   write-only chips (mainly clock chips) */
				res = i2c_smbus_read_byte(file);
				break;
			}

			if(res>=0){
				address=i+j;
			}
		}
	}
	close(file);
	return 0;
}


int main(int argc, char *argv[])
{
	if(scan_i2c_bus(MODE, FIRST, LAST))
	{
		exit(1);
	}

   int fd=wiringPiI2CSetupInterface(device,address);
   if(fd<0)
   {
      printf("I2C Fail\n");
   }
  while(1)
  {
    write(fd, key, 4);
	break;
  }

  close(fd);
  return (EXIT_SUCCESS);
	// printf("value return d:%02x\n",address);
	// if(0x08==address)
	// {
	// 	printf("cho thi");
	// }

	// return 0;

}
