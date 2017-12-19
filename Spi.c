#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#include "spi_hardware.h"
MODULE_AUTHOR("optionESE");
MODULE_DESCRIPTION("Projet");
MODULE_LICENSE("none");

static int majeur;

void debug(void) {
	printk(KERN_DEBUG "%x\n",at91_spi_read(AT91_SPI_SR));	
}

static ssize_t spi_write(struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
	if((at91_spi_read(AT91_SPI_SR)&0x2)==0x2) {
		debug();
		unsigned int value =0;
		printk(KERN_DEBUG "write SPI\n");
		if(count<1) return -1;

		value = (unsigned int)buffer[1];
		value = value << 8;
		value += (unsigned int)buffer[0];
		at91_spi_write( AT91_SPI_TDR,/* Transmit Data Register (16bit)*/
					value);	/* Transmit Data */ //16bit //& AT91_SPI_TD
		debug();

	  	return count;
	}
	return -1;
}

static ssize_t spi_read(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
	if((at91_spi_read(AT91_SPI_SR)&0x1)==0x1) {
		debug();
		unsigned int value;
		printk(KERN_DEBUG "read SPI\n");
		debug();
		value = AT91_SPI_RD & at91_spi_read(AT91_SPI_RDR);/* Receive Data Register (16bit)*/

		if( (buffer==NULL) || (count!=2) ) {
			printk(KERN_DEBUG "count = %d\n",count);
			return -1;
		}

		buffer[0] =  (char)(value & 0x000F);
		buffer[1] = (char)(value >> 8);
		return 2;
	}
	return -1;
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

	printk(KERN_DEBUG "initialisation du module SPI\n");

	/*************************
	PMC setup
	*************************/

	at91_sys_write(	AT91_PMC_PCER,			/*Peripheral Clock Enable*/
					1<<AT91_ID_SPI);	/*SPI enable*/

	/*************************
	PIO Controler setup
	*************************/

	at91_sys_write(	AT91_PIOA+PIO_PDR, 	/*Pin controlled by peripheral*/
					AT91_PA0_MISO 	| 	/*MISO*/
					AT91_PA1_MOSI 	| 	/*MOSI*/
					AT91_PA2_SPCK 	| 	/*SPCK*/
					AT91_PA3_NPCS0);	/*NPCS0*/


	at91_sys_write(	AT91_PIOA+PIO_ASR, 	/*Peripheral A pin select*/
					AT91_PA0_MISO 	| 	/*MISO*/
					AT91_PA1_MOSI 	| 	/*MOSI*/
					AT91_PA2_SPCK 	| 	/*SPCK*/
					AT91_PA3_NPCS0);	/*NPCS0*/

					
	
					
	/*************************
	SPI setup
	*************************/


	at91_spi_write( AT91_SPI_CR,AT91_SPI_SWRST);	/* SPI Software Reset */

	at91_spi_write( AT91_SPI_MR,				/* Mode Register */
					AT91_SPI_MSTR		|/* Master/Slave Mode */
					AT91_SPI_PS_FIXED   	|/* Chip select fixed*/
					0x00<<16		|/*NPCS 0*/
					64<<24			|/* AT91_SPI_DLYBCT = 64 = 11.38µs */
					AT91_SPI_MODFDIS	|/*mode fault detection disabled*/
					AT91_SPI_DIV32);	/* Clock Selection */				

	at91_spi_write(	AT91_SPI_CSR(0),	/*	Chips select register 0*/
					AT91_SPI_CPOL		|/*	Clock Polarity*/
					AT91_SPI_BITS_16	|/*	16-bits transfer*/
					9<<16				|/* AT91_SPI_DLYBS = 9/180MHz = 50ns */
					64<<24				|/* AT91_SPI_DLYBCT = 64 = 11.38µs */
					4);				/*	Baud rate MCK / (64*SCBR) SCBR=4 SPCK=703,1 kHz*/

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

