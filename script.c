#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "spi.h"
#include "lcdtxt_at91.h"

int main(int argc, char *argv[]) {
	char value[2] = {'0','0'};
	char c;
	char buffer[24];

	printf("Opening the device file...");
	int spi = open("/dev/spi",O_RDWR);
	printf("OK\n");

	affichage_lcd(0,0,"SPI");
	printf("Sélection de l'axe : ");
	scanf("%c", &c);
	if(spi>=0) {
		if (ioctl(spi,SET_AXE,c)==0) {
			printf("SPI file opened, reading data...\n");
			while(1) {

				if(read(spi,value,sizeof(value))!=2) printf("error read\n");
				sprintf(buffer,"Value %c : %s",c,value);
				affichage_lcd(0,0,buffer);
				sleep(1);

			}
			close(spi);
		}	
	} else printf("erreur descripteur\n");
	return -1;
}



