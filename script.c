#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
	int i=0;
	char value[2] = {'0','0'};
	char c;
	int descripteur = open("/dev/spi",O_RDWR);
	sleep(1);
	
	printf("Sélection de l'axe : ");
	scanf("%c", &c);
	if (ioctl(descripteur,SET_AXE,c)==0) {
		
		if(descripteur>=0) {
			printf("SPI file opened\n");
			while(1) {

				if(read(descripteur,value,sizeof(value))!=2) printf("error read\n");
				printf("%s\n",value);
			}
			close(descripteur);
		} else return -1;
		
	} else  printf("wrong axe");

	return 0;
}
