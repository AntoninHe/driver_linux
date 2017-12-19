#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int i=0;
	char c[1] = {'X'};
	char value[2] = {'0','0'};
	int descripteur = open("/dev/spi",O_RDWR);

	printf("Sélection de l'axe : ");
	scanf("%c", &c[0]);
	if (c[0]=='X' | c[0]=='Y' | c[0]=='Z' | c[0]=='x' | c[0]=='y' | c[0]=='z') {

		write(descripteur,c,sizeof(c));

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
