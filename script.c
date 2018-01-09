#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "spi.h"
#include "lcdtxt_at91.h"


int main(int argc, char *argv[]) {
	char value[2] = {'0','0'};
	char choix[3];
	char buffer[24];
	int c;
	short int valeur;

	printf("Opening the device file...");
	int spi_d = open("/dev/spi",O_RDWR);
	printf("OK\n");

	affichage_lcd(0,0,"SPI");
	printf("Sélection de l'axe : ");
	scanf("%s", choix);
	if(choix[1]=='Y')
		c=AXE_XY;
	else if(choix[0]=='Y')
		c=AXE_Y;
	else
		c=AXE_X;

	if(spi_d>=0) {
		if (ioctl(spi_d,SET_AXE,c)==0) {
			printf("SPI file opened, reading data...\n");
			while(1) {

				if(read(spi_d,value,sizeof(value))!=2) printf("error read\n");
				valeur= value[0]+value[1]<<8;
				sprintf(buffer,"Value %s : %d",choix,valeur);
				affichage_lcd(0,0,buffer);
				sleep(1);

			}
			close(spi_d);
		}	
	} else printf("erreur descripteur\n");
	return -1;
}



