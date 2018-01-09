
CCM='/usr/local/arm-ssv1-linux/bin/arm-ssv1-linux-gcc'
CROSS   =   /usr/local/arm-linux/bin/arm-linux-
CC	=	$(CROSS)gcc
CCFLAGS =	-march=armv4
CFLAGS	=	-Wall -O2 -Wl,-s $(CCFLAGS)
PROJ	=	lcdtxt_at91

obj-m := Spi.o

all: module
	cp Spi.ko $(PRJROOT)/nfs/lib/modules/

module:
	make -C /home/ese/Documents/mini-projet_RIO_HERESON/kernel/sourceNoyau SUBDIRS=$(PWD) modules

script: bin
	cp script $(PRJROOT)/nfs/home/guest/

bin:  script.o
	$(CCM) -o script script.o
	
script.o: script.c
	$(CCM) -o script.o -c script.c

lcd:	$(PROJ)
	cp $(PROJ) $(PRJROOT)/nfs/home/guest/

$(PROJ): $(PROJ).c Makefile
	$(CCM) $(CFLAGS) -o $(PROJ) $(PROJ).c
