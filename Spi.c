#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#include "spi_hardware.h"
MODULE_AUTHOR("optionESE");
MODULE_DESCRIPTION("Projet");
MODULE_LICENSE("none");

static int majeur;
int tr,i,result,bufint,axis;

//#define debug(); printk(KERN_DEBUG "Line number %d, status : %x\n", __LINE__,at91_spi_read(AT91_SPI_SR)); 
#define debug();

static ssize_t spi_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	//printk(KERN_DEBUG "read()\n");
		
	// Transfert	
	at91_spi_write(AT91_SPI_TDR, tr);
	
	// Attente de la fin de transfert
	debug();
	while ((0x00000001 & at91_spi_read(AT91_SPI_SR)) != AT91_SPI_RDRF);	//boucle infinie, car on n'utilise pas d'interruption

	// Lecture de la reponse de l'inclinometre
	result = at91_spi_read(AT91_SPI_RDR);

	// Traduction de la reponse en degres
	if ((result & (1<<13))==0x0000){
		bufint = ((short int)(result & 0x1FFF)) /40 ;	
	}
	else if ((result  & (1<<13))==0x2000){
		bufint = ((short int)(result | 0xC000)) /40 ;	
	}
	
	// Enregistrement de la donnee
	sprintf(buf, "%d", bufint);
	
	return 2;
}

//Choix de l'axe
static ssize_t spi_write(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	printk(KERN_DEBUG "write()\n");
	
	if ((buf[0] == 'X') | (buf[0] == 'x')){
		tr = 0x0C00; // Pour X
	}
	if ((buf[0] == 'Y') | (buf[0] == 'y')){
		tr = 0x0E00; // Pour Y
	}		
	if ((buf[0] == 'Z') | (buf[0] == 'z')){
		tr = 0x1000; // Pour Z
	}

 	return 0;
}

static ssize_t spi_open(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "open SPI\n");
	debug();
  return 0;
}

static ssize_t spi_close(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG "close SPI\n");
	debug();
  return 0;
}

static struct file_operations fops =
{
  read : spi_read,
  write : spi_write,
  open : spi_open,
  release : spi_close
};

static int __init module_spi_init(void)
{
	int ret;
	int temp,temp1;
	printk(KERN_DEBUG "initialisation du module SPI\n");

	/*************************
	PIO Controler setup
	*************************/
	temp = 	AT91_PA0_MISO 	| 	/*MISO*/
			AT91_PA1_MOSI 	| 	/*MOSI*/
			AT91_PA2_SPCK 	| 	/*SPCK*/
			AT91_PA3_NPCS0;	/*NPCS0*/
	
	at91_sys_write(	AT91_PIOA+PIO_PDR,temp);
	
	//at91_sys_write(	AT91_PIOA+PIO_PDR, 	/*Pin controlled by peripheral*/
	//				AT91_PA0_MISO 	| 	/*MISO*/
	//				AT91_PA1_MOSI 	| 	/*MOSI*/
	//				AT91_PA2_SPCK 	| 	/*SPCK*/
	//				AT91_PA3_NPCS0);	/*NPCS0*/

	temp = 	AT91_PA0_MISO 	| 	/*MISO*/
			AT91_PA1_MOSI 	| 	/*MOSI*/
			AT91_PA2_SPCK 	| 	/*SPCK*/
			AT91_PA3_NPCS0;	/*NPCS0*/
			
	at91_sys_write(	AT91_PIOA+PIO_ASR, temp);
	
	//at91_sys_write(	AT91_PIOA+PIO_ASR, 	/*Peripheral A pin select*/
	//				AT91_PA0_MISO 	| 	/*MISO*/
	//				AT91_PA1_MOSI 	| 	/*MOSI*/
	//				AT91_PA2_SPCK 	| 	/*SPCK*/
	//				AT91_PA3_NPCS0);	/*NPCS0*/

			

	/*************************
	PMC setup
	*************************/
	temp=1<<AT91_ID_SPI;
	at91_sys_write(AT91_PMC_PCER,temp);
//	at91_sys_write(	AT91_PMC_PCER,			/*Peripheral Clock Enable*/
//					1<<AT91_ID_SPI);	/*SPI enable*/		
	
					
	/*************************
	SPI setup
	*************************/


	at91_spi_write( AT91_SPI_CR,AT91_SPI_SWRST);	/* SPI Software Reset */

	
	temp=64<<24;
	temp|=AT91_SPI_MSTR|AT91_SPI_PS_FIXED|AT91_SPI_PS_FIXED|AT91_SPI_MODFDIS|AT91_SPI_DIV32;
	at91_spi_write(AT91_SPI_MR,temp);
//	at91_spi_write( AT91_SPI_MR,				/* Mode Register */
//					AT91_SPI_MSTR		|/* Master/Slave Mode */
//					AT91_SPI_PS_FIXED   	|/* Chip select fixed*/
//					0x00<<16		|/*NPCS 0*/
//					64<<24			|/* AT91_SPI_DLYBCT = 64 = 11.38µs */
//					AT91_SPI_MODFDIS	|/*mode fault detection disabled*/
//					AT91_SPI_DIV32);	/* Clock Selection */				
	temp =9<<16;
	temp1=64<<24;
	temp|=temp1 | AT91_SPI_CPOL | AT91_SPI_BITS_16 |(4<<8);
	at91_spi_write(	AT91_SPI_CSR(0),temp);
	
//	at91_spi_write(	AT91_SPI_CSR(0),	/*	Chips select register 0*/
//					AT91_SPI_CPOL		|/*	Clock Polarity*/
//					AT91_SPI_BITS_16	|/*	16-bits transfer*/
//					9<<16				|/* AT91_SPI_DLYBS = 9/180MHz = 50ns */
//					64<<24				|/* AT91_SPI_DLYBCT = 64 = 11.38µs */
//					4);				/*	Baud rate MCK / (64*SCBR) SCBR=4 SPCK=703,1 kHz*/

	at91_spi_write( AT91_SPI_CR,AT91_SPI_SPIEN);	/* SPI Enable */
	debug();

	ret = register_chrdev(0,"spi_inclinometre_driver",&fops);
	if (ret < 0)
	{
		printk(KERN_WARNING "erreur fonction register_chrdev\n");
		return ret;
	}
	majeur = ret;
	printk(KERN_DEBUG "pilote spi charge avec succes\n");
	printk(KERN_DEBUG "MAJEUR : %d\n",ret);
	return 0;
} 

static void __exit module_spi_cleanup(void)
{
	int ret;
	
	printk(KERN_DEBUG "arret du module SPI\n");
	ret=unregister_chrdev(majeur,"spi_inclinometre_driver");
	if (ret < 0)
		printk(KERN_WARNING "erreur fonction unregister_chrdev\n");
	else 
		printk(KERN_DEBUG "pilote spi decharge avec succes\n");
}

module_init(module_spi_init);
module_exit(module_spi_cleanup);

