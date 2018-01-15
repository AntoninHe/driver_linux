#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "spi.h"
#include "lcdtxt_at91.h"

short int convert_2short_int(char *value){
	short int valeur;
	valeur = (short int)value[1];
	valeur = valeur*256 + (short int)value[0];
	return valeur;
}

int main(int argc, char *argv[]) {
	char value[2] = {'0','0'};
	char choix[3];
	char buffer[24];
	int c;
  //initialisation de l'écran lcd
  affichage_init();

	printf("Opening the device file...");
	int spi_d = open("/dev/spi",O_RDWR);
	printf("OK\n");

	affichage_lcd(0,0,"SPI");

	printf("Sélection de l'axe : ");
	scanf("%s", choix);
	if(choix[1]=='Y') c=AXE_XY;
	else if(choix[0]=='Y') c=AXE_Y;
	else c=AXE_X;

	if(spi_d>=0) {
		if (ioctl(spi_d,SET_AXE,c)==0) {
			printf("SPI file opened, reading data...\n");
			while(1) {

				if(read(spi_d,value,sizeof(value))<2) printf("error read\n");

				if(c!=AXE_XY){
					sprintf(buffer,"Value %s : %d",choix,convert_2short_int(value));
					affichage_lcd(0,0,buffer);
				}
				else{
					sprintf(buffer,"Value X : %d",convert_2short_int(value));
					affichage_lcd(0,0,buffer);
					sprintf(buffer,"Value Y : %d",convert_2short_int(value+2));
					affichage_lcd(0,2,buffer);
				}

				sleep(1);

			}
			close(spi_d);
		}
	} else printf("erreur descripteur\n");
	return -1;
}
