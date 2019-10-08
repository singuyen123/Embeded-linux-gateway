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

#define MODE_AUTO 0
#define MODE_QUICK 1
#define MODE_READ 2
#define MODE MODE_AUTO
#define FIRST 0x03
#define LAST 0x77
#define I2CBUS 0

volatile int address = 0;
static const char *device = "/dev/i2c-0";
const char key[4] = {1, 9, 9, 7};
volatile int fd;
int wiringPiI2CSetupInterface(const char *device, int devId)
{
	int fd;

	if ((fd = open(device, O_RDWR)) < 0)
	{
		fprintf(stderr, "I2C: Failed to access %s\n", device);
		exit(1);
	}

	if (ioctl(fd, I2C_SLAVE, devId) < 0)
	{
		fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", devId);
		exit(1);
	}
	return fd;
}

int scan_i2c_bus(int mode, int first, int last, int file)
{
	int i, j;
	int cmd, res;
	int flag = 0;
	//char filename[20];
	//file = open_i2c_dev(I2CBUS, filename, sizeof(filename), 0);

	for (i = 0; i < 128; i += 16)
	{
		for (j = 0; j < 16; j++)
		{
			//printf("scan: %d %d\n",i,j);
			fflush(stdout);
			/* Select detection command for this address */
			switch (mode)
			{
			default:
				cmd = mode;
				break;
			case MODE_AUTO:
				if ((i + j >= 0x30 && i + j <= 0x37) || (i + j >= 0x50 && i + j <= 0x5F))
					cmd = MODE_READ;
				else
					cmd = MODE_QUICK;
				break;
			}

			/* Set slave address */
			if (ioctl(file, I2C_SLAVE, i + j) < 0)
			{
				//printf("access erro\n");
				if (errno == EBUSY)
				{
					printf("UU ");
					continue;
				}
				else
				{
					fprintf(stderr, "Error: Could not set "
									"address to 0x%02x: %s\n",
							i + j,
							strerror(errno));
					return -1;
				}
			}
			// else{
			// 	printf("set address success\n");
			// }

			/* Probe this address */
			switch (cmd)
			{
			default: /* MODE_QUICK */
				/* This is known to corrupt the Atmel AT24RF08
				   EEPROM */
				//printf("Mode default\n");
				res = i2c_smbus_write_quick(file,
											I2C_SMBUS_WRITE);

				break;
			case MODE_READ:
				/* This is known to lock SMBus on various
				   write-only chips (mainly clock chips) */
				//   printf("Mode read\n");
				res = i2c_smbus_read_byte(file);
				break;
			}

			if (res >= 0)
			{
				address = i + j;
				flag++;
					
			}
		}
	}
	if (flag == 0)
	{
		//	printf("don't see address\n");
		return 0;
	}
	else
	{
		printf("flag=%d\n", flag);
		return address;
	}
}

int main(int argc, char *argv[])
{
	while (1)
	{
		printf("Start i2c:\n");
		fd = open(device, O_RDWR);
		if (open(device, O_RDWR) < 0)
		{
			printf("Don't open gate\n");
			continue;
		}
		else
		{

			printf("open gate success\n");
			int sc = scan_i2c_bus(MODE, FIRST, LAST, fd);
			printf("Sc=%d\n", sc);
			if (sc < 0 || sc > 128)
			{
				printf("Don't scan address\n");
				continue;
			}
			else
			{

				printf("scan address success \n");
				// printf("address: %d\n", address);

				// int ac=ioctl(fd, I2C_SLAVE, address);
				// printf("ac=%d\n", ac);

				if (ioctl(fd, I2C_SLAVE, address) < 0)
				{
					fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
					continue;
				}
				else
				{
					printf("Acess success\n");
					write(fd, key, 4);
					sleep(1);
				}
			}
		}
	}

	//	close(fd);
	return (EXIT_SUCCESS);
}
