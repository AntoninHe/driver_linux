obj-m := Spi.o

all:
	  make -C /home/ese/Documents/mini-projet_RIO_HERESON/kernel/sourceNoyau SUBDIRS=$(PWD) modules
