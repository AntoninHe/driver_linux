#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int i=0;
//	char c[2] = {0x00,0x0C} ;
	char c[1] = {'X'};
	char chips[2] = {0xF,0xF};
	int descripteur = open("/dev/spi",O_RDWR);

	write(descripteur,c,sizeof(c));
	sleep(1);

	if(descripteur>=0) {
		printf("SPI file opened\n");
		for (i=0;i<10;i++) {

//			if(write(descripteur,c,sizeof(c))!=2) printf("error write\n");

			if(read(descripteur,chips,sizeof(chips))!=2) printf("error read\n");
			printf("%x%x\n",chips[1],chips[0]);

			sleep(1);
		}
		close(descripteur);
	} else return -1;

	return 0;
}
