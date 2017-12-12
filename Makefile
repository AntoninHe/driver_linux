
CC='/usr/local/arm-ssv1-linux/bin/arm-ssv1-linux-gcc'

obj-m := Spi.o

all: module
	mv Spi.ko $(PRJROOT)/nfs/lib/modules/

module:
	make -C /home/ese/Documents/mini-projet_RIO_HERESON/kernel/sourceNoyau SUBDIRS=$(PWD) modules

script: bin
	mv script $(PRJROOT)/nfs/home/guest/

bin:  script.o
	$(CC) -o script script.o
	

script.o: script.c
	$(CC) -o script.o -c script.c
