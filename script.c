#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int i=0;
	char c[2] = {0x00,0x0C} ;
	char chips[2] = {0xF,0xF};
	//FILE * sp = fopen("/dev/spi","wb");
	int descripteur = open("/dev/spi",O_RDWR);

	if(descripteur>=0) {
		printf("SPI file opened\n");
		for (i=0;i<10;i++) {
			/*if(fwrite(&c,sizeof(char),2,sp)!=2) printf("error write\n");
			fflush(sp);*/
			if(write(descripteur,c,sizeof(c))!=2) printf("error write\n");
			sleep(1);
			/*if(fread(&chips,sizeof(char),2,sp)!=2) printf("error read\n");
			fflush(sp);*/
			if(read(descripteur,chips,sizeof(chips))!=2) printf("error read\n");
			printf("%x%x\n",chips[1],chips[0]);
			sleep(1);
		}
//		fclose(sp);
		close(descripteur);
	} else return -1;

	return 0;
}
