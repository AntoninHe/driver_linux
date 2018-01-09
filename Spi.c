#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/semaphore.h>

#include "spi_hardware.h"
#include "spi.h"

MODULE_AUTHOR("Héréson & Rio");
MODULE_DESCRIPTION("Projet option ESE : driver SPI");
MODULE_LICENSE("GPL");

static int majeur;
static struct semaphore spi_sem;
static short int data;
int AXE_CONFIG=AXE_X;

//#define debug(); printk(KERN_DEBUG "Line number %d, status : %x\n", __LINE__,at91_spi_read(AT91_SPI_SR)); 
#define debug();

static irqreturn_t spi_interrupt(int irq, void *dev_id)
{
	data = at91_spi_read(AT91_SPI_RDR);
	up(&spi_sem);
	return IRQ_HANDLED;
}

short int convert_spi_value(short int data){
	// Traduction de la reponse en degrés
	short int result=0;
	if ((data & (1<<13))==0x0000){
		result = ((short int)(data & 0x3FFF)) /40 ;	
	}
	else if ((data  & (1<<13))==0x2000){
		result = ((short int)(data | 0x8000)) /40 ;	
	}
	return result;
}


short int spiReadX()
{
	at91_spi_write(AT91_SPI_TDR, 0x0C00);
	down(&spi_sem);	// Attente de la fin de transfert
	return convert_spi_value(data);	
}

short int spiReadY()
{
	at91_spi_write(AT91_SPI_TDR, 0x0E00);
	down(&spi_sem);	// Attente de la fin de transfert
	return convert_spi_value(data);
}


static ssize_t spi_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{	
	short int result;
	switch (AXE_CONFIG){
		case AXE_X :
		//read X
		result = spiReadX();
		buf[0] = result & 0x00FF;
		buf[1] = (result & 0xFF00)>>8;
		return 2;

		case AXE_Y :
		//read Y
		result = spiReadY();
		buf[0] = result & 0x00FF;
		buf[1] = (result & 0xFF00)>>8;
		return 2;

		case AXE_XY :
		//read X
		result = spiReadX();
		buf[0] = result & 0x00FF;
		buf[1] = (result & 0xFF00)>>8;
		//read Y
		result = spiReadY();
		buf[2] = result & 0x00FF;
		buf[3] = (result & 0xFF00)>>8;
		return 4;
	}
}



//int ioctl(int fd, int cmd, char *argp); user
static int spi_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case SET_AXE :
			switch(arg){
				case  AXE_X:
					AXE_CONFIG = AXE_X; // Pour X
					break;
			
				case AXE_Y :
					AXE_CONFIG =  AXE_Y; // Pour Y
					break;

				case AXE_XY :
					AXE_CONFIG =  AXE_XY; // Pour Y
					break;
			
				default :
					printk("axe unknown\n");
					return -1;
			}
				break;

			default :
				printk("cmd unknown\n");
				return -1;
	}
	return 0;
}


static ssize_t spi_open(struct inode *inode, struct file *file)
{
	if((at91_spi_read(AT91_SPI_IMR) & AT91_SPI_RDRF) != AT91_SPI_RDRF) return -1;
	printk(KERN_DEBUG "valeur de AIC_IMR : %x\n",at91_sys_read(AT91_AIC_IMR));
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
  ioctl : spi_ioctl,
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
//					4<<8);				/*	Baud rate MCK / (64*SCBR) SCBR=4 SPCK=703,1 kHz*/

	init_MUTEX_LOCKED(&spi_sem);
	if (request_irq(AT91_ID_SPI, spi_interrupt,0,"at91_spi",NULL) < 0) printk(KERN_DEBUG "request_irq fault\n");

	at91_spi_write( AT91_SPI_IER,			/* Interrupt Enable Register */
				AT91_SPI_RDRF);		/* Receive Data Register Full Interrupt Enable */

	at91_sys_write( AT91_AIC_IECR,			/* AIC Interrupt Enable Command Register */
				(1<<AT91_ID_SPI));	/* Enable Interrupt n°13 = SPI */

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
	free_irq (AT91_ID_SPI,NULL);
	printk(KERN_DEBUG "arret du module SPI\n");
	ret=unregister_chrdev(majeur,"spi_inclinometre_driver");
	if (ret < 0)
		printk(KERN_WARNING "erreur fonction unregister_chrdev\n");
	else 
		printk(KERN_DEBUG "pilote spi decharge avec succes\n");
}

module_init(module_spi_init);
module_exit(module_spi_cleanup);

