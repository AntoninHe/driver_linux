
CC	=	/usr/local/arm-ssv1-linux/bin/arm-ssv1-linux-gcc
CCFLAGS =	-march=armv4
CFLAGS	=	-Wall -O2 -Wl,-s $(CCFLAGS)

obj-m := Spi.o

all: module
	cp Spi.ko $(PRJROOT)/nfs/lib/modules/

module:
	make -C /home/ese/Documents/mini-projet_RIO_HERESON/kernel/sourceNoyau SUBDIRS=$(PWD) modules

affichage:	affichage_acc
	cp affichage_acc $(PRJROOT)/nfs/home/guest/

affichage_acc: lcdtxt_at91.c script.c Makefile
	$(CC) $(CFLAGS) -o affichage_acc lcdtxt_at91.c script.c
