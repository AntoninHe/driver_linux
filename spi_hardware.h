#ifndef __ASM_SPI_HARDWARE_H
#define __ASM_SPI_HARDWARE_H

#include </asm-arm/arch-at91rm9200/hardware.h>

static inline unsigned int at91_spi_read(unsigned int reg_offset)
{
	void __iomem *addr = (void __iomem *)AT91_VA_BASE_SPI;

	return readl(addr + reg_offset);
}

AT91_SPI_RD -> ajout masque de lecture 32->16 bits

//fonction d'écriture mémoire pour le SPI
static inline void at91_spi_write(unsigned int reg_offset, unsigned long value)
{
	void __iomem *addr = (void __iomem *)AT91_VA_BASE_SPI;

	writel(value, addr + reg_offset);
}

#endif 
