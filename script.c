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
	int descripteur = open("/dev/spi",O_RDWR);
	affichage_lcd(0,0,"SPI");
	printf("Sélection de l'axe : ");
	scanf("%c", &c);
	if(descripteur>=0) {
		/*if (ioctl(descripteur,SET_AXE,c)==0) {
			printf("SPI file opened\n");
			while(1) {

				if(read(descripteur,value,sizeof(value))!=2) printf("error read\n");
				sprintf(buffer,"Value %c : %s",c,value);
				affichage_lcd(0,0,buffer);
				sleep(1);

			}*/
			close(descripteur);
		}	
	} else printf("erreur descripteur\n");
	return -1;
}

//lcd_cls

