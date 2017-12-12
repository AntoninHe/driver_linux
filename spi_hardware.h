#ifndef __ASM_SPI_HARDWARE_H
#define __ASM_SPI_HARDWARE_H

#include <asm/arch-at91rm9200/hardware.h>
#include <asm/arch-at91rm9200/at91rm9200_spi.h>
#include <asm/arch-at91rm9200/at91rm9200_sys.h>

static inline unsigned int at91_spi_read(unsigned int reg_offset)
{
	void __iomem *addr = (void __iomem *)AT91_VA_BASE_SPI;

	return readl(addr + reg_offset);
}


//fonction d'écriture mémoire pour le SPI
static inline void at91_spi_write(unsigned int reg_offset, unsigned long value)
{
	void __iomem *addr = (void __iomem *)AT91_VA_BASE_SPI;

	writel(value, addr + reg_offset);
}

#endif 

