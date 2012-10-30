#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	
	int fd;														
	char *fileName = "/dev/i2c-0";								
	int  address = 0x38;										
	char buf[1];
	buf[0] = 0x80;										
	char leds = 0xFF;
	int direction = 1;
	
	if ((fd = open(fileName, O_RDWR)) < 0) {					
		printf("Failed to open i2c port\n");
		exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, address) < 0) {					
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}
	
while(1){	
	char b[1];
	b[0] = buf[0] ^ leds;
	if ((write(fd, b, 1)) != 1) {								
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	printf("Value is %x\n", buf[0]);
	usleep(250000);	
	if(direction == 1) buf[0] = buf[0] >> 1;
	else buf[0] = buf[0] << 1;
	if(buf[0] == 0x00) {
		if(direction == 1) {	
			direction = 0;
			buf[0] = 0x01;
		}
		else if(direction == 0){
			direction = 1;
			buf[0] = 0x80;
		}	
	}	
}
}
