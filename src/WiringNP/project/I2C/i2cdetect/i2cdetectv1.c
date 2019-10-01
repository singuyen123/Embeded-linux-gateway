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
#include "version.h"
#include "i2c.h"

#define MODE_AUTO	0
#define MODE_QUICK	1
#define MODE_READ	2
#define MODE_FUNC	3
#define MODE MODE_AUTO
#define FIRST 0x03
#define LAST  0x77
#define I2CBUS  0
volatile int address=0;
volatile int address1=0;


static int scan_i2c_bus(int file, int mode, unsigned long funcs,
			int first, int last)
{
	int i, j;
	int cmd, res;

//	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");

	for (i = 0; i < 128; i += 16) {
	//	printf("%02x: ", i);
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

			//if (res < 0)
			//	printf("-- ");
			//else{
			if(res>=0){
				//printf("%02x ", i+j);
				address=i;
				address1=j;
			}
		}
	//	printf("\n");
	}

	return 0;
}

struct func
{
	long value;
	const char* name;
};

static const struct func all_func[] = {
	{ .value = I2C_FUNC_I2C,
	  .name = "I2C" },
	{ .value = I2C_FUNC_SMBUS_QUICK,
	  .name = "SMBus Quick Command" },
	{ .value = I2C_FUNC_SMBUS_WRITE_BYTE,
	  .name = "SMBus Send Byte" },
	{ .value = I2C_FUNC_SMBUS_READ_BYTE,
	  .name = "SMBus Receive Byte" },
	{ .value = I2C_FUNC_SMBUS_WRITE_BYTE_DATA,
	  .name = "SMBus Write Byte" },
	{ .value = I2C_FUNC_SMBUS_READ_BYTE_DATA,
	  .name = "SMBus Read Byte" },
	{ .value = I2C_FUNC_SMBUS_WRITE_WORD_DATA,
	  .name = "SMBus Write Word" },
	{ .value = I2C_FUNC_SMBUS_READ_WORD_DATA,
	  .name = "SMBus Read Word" },
	{ .value = I2C_FUNC_SMBUS_PROC_CALL,
	  .name = "SMBus Process Call" },
	{ .value = I2C_FUNC_SMBUS_WRITE_BLOCK_DATA,
	  .name = "SMBus Block Write" },
	{ .value = I2C_FUNC_SMBUS_READ_BLOCK_DATA,
	  .name = "SMBus Block Read" },
	{ .value = I2C_FUNC_SMBUS_BLOCK_PROC_CALL,
	  .name = "SMBus Block Process Call" },
	{ .value = I2C_FUNC_SMBUS_PEC,
	  .name = "SMBus PEC" },
	{ .value = I2C_FUNC_SMBUS_WRITE_I2C_BLOCK,
	  .name = "I2C Block Write" },
	{ .value = I2C_FUNC_SMBUS_READ_I2C_BLOCK,
	  .name = "I2C Block Read" },
	{ .value = 0, .name = "" }
};



int main(int argc, char *argv[])
{

	int file, res;
	char filename[20];
	unsigned long funcs;



	file = open_i2c_dev(I2CBUS, filename, sizeof(filename), 0);
	if (file < 0) {
		exit(1);
	}

	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		close(file);
		exit(1);
	}


	res = scan_i2c_bus(file, MODE, funcs, FIRST, LAST);
	if(res)
	{
		exit(1);
	}
	printf("value return d:%02x\n",address+address1);
	close(file);
	return 0;

}
