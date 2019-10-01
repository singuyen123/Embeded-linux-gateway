#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVID 0x08

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

// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0

 int main() {
   int fd=wiringPiI2CSetupInterface(device,DEVID);
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
}
