#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
	int i=0;
	char c[1] = {'X'};
	char chips[2] = {0xF,0xF};
	int descripteur = open("/dev/spi",O_RDWR);
	ioctl(descripteur,SET_AXE,'X');	
	sleep(1);

	if(descripteur>=0) {
		printf("SPI file opened\n");
		for (i=0;i<10;i++) {

			if(read(descripteur,chips,sizeof(chips))!=2) printf("error read\n");
			printf("%x%x\n",chips[1],chips[0]);

			sleep(0.5);
		}
		close(descripteur);
	} else return -1;

	return 0;
}
