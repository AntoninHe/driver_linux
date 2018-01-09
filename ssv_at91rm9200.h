#ifndef SSV_AT91RM9200_INCLUDED
#define SSV_AT91RM9200_INCLUDED

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef volatile unsigned char  vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int   vu32;

vu32 *pAT91_REGS;
vu8 *pCS1_EXT, *pCS2_EXT, *pCS3_EXT, *pCS4_EXT;

#define MAP_AT91_REGISTER() { pAT91_REGS = pMapmemory(0xFFFFF000, 0x00001000); }

/* PIOA Header */
#define PIOA_PER   (*(pAT91_REGS + 0x400 / 4)) /* PIOA Enable Register */
#define PIOA_PDR   (*(pAT91_REGS + 0x404 / 4)) /* PIOA Disable Register */
#define PIOA_OER   (*(pAT91_REGS + 0x410 / 4)) /* Output Enable Register (output) */
#define PIOA_ODR   (*(pAT91_REGS + 0x414 / 4)) /* Output Disable Register (input) */
#define PIOA_SODR  (*(pAT91_REGS + 0x430 / 4)) /* Set Output Data Register (outset) */
#define PIOA_CODR  (*(pAT91_REGS + 0x434 / 4)) /* Clear Output Data Register (outclr) */
#define PIOA_ODSR  (*(pAT91_REGS + 0x438 / 4)) /* Output Data Status Register (read output) */
#define PIOA_PDSR  (*(pAT91_REGS + 0x43C / 4)) /* Pin Data Status Register (read input) */

/* PIOB Header */
#define PIOB_PER   (*(pAT91_REGS + 0x600 / 4)) /* PIOB Enable Register */
#define PIOB_PDR   (*(pAT91_REGS + 0x604 / 4)) /* PIOB Disable Register */
#define PIOB_OER   (*(pAT91_REGS + 0x610 / 4)) /* Output Enable Register (output) */
#define PIOB_ODR   (*(pAT91_REGS + 0x614 / 4)) /* Output Disable Register (input) */
#define PIOB_SODR  (*(pAT91_REGS + 0x630 / 4)) /* Set Output Data Register (outset) */
#define PIOB_CODR  (*(pAT91_REGS + 0x634 / 4)) /* Clear Output Data Register (outclr) */
#define PIOB_ODSR  (*(pAT91_REGS + 0x638 / 4)) /* Output Data Status Register (read output) */
#define PIOB_PDSR  (*(pAT91_REGS + 0x63C / 4)) /* Pin Data Status Register (read input) */

/* PIOC Header */
#define PIOC_PER   (*(pAT91_REGS + 0x800 / 4)) /* PIOC Enable Register */
#define PIOC_PDR   (*(pAT91_REGS + 0x804 / 4)) /* PIOC Disable Register */
#define PIOC_OER   (*(pAT91_REGS + 0x810 / 4)) /* Output Enable Register (output) */
#define PIOC_ODR   (*(pAT91_REGS + 0x814 / 4)) /* Output Disable Register (input) */
#define PIOC_SODR  (*(pAT91_REGS + 0x830 / 4)) /* Set Output Data Register (outset) */
#define PIOC_CODR  (*(pAT91_REGS + 0x834 / 4)) /* Clear Output Data Register (outclr) */
#define PIOC_ODSR  (*(pAT91_REGS + 0x838 / 4)) /* Output Data Status Register (read output) */
#define PIOC_PDSR  (*(pAT91_REGS + 0x83C / 4)) /* Pin Data Status Register (read input) */

/* PIOD Header */
#define PIOD_PER   (*(pAT91_REGS + 0xA00 / 4)) /* PIOD Enable Register */
#define PIOD_PDR   (*(pAT91_REGS + 0xA04 / 4)) /* PIOD Disable Register */
#define PIOD_OER   (*(pAT91_REGS + 0xA10 / 4)) /* Output Enable Register (output) */
#define PIOD_ODR   (*(pAT91_REGS + 0xA14 / 4)) /* Output Disable Register (input) */
#define PIOD_SODR  (*(pAT91_REGS + 0xA30 / 4)) /* Set Output Data Register (outset) */
#define PIOD_CODR  (*(pAT91_REGS + 0xA34 / 4)) /* Clear Output Data Register (outclr) */
#define PIOD_ODSR  (*(pAT91_REGS + 0xA38 / 4)) /* Output Data Status Register (read output) */
#define PIOD_PDSR  (*(pAT91_REGS + 0xA3C / 4)) /* Pin Data Status Register (read input) */

/* Chip Select Assignment Register */
#define EBI_CSA    (*(pAT91_REGS + 0xF60 / 4)) /* */

#define SMC2_CSR0  (*(pAT91_REGS + 0xF70 / 4)) /* */
#define SMC2_CSR1  (*(pAT91_REGS + 0xF74 / 4)) /* */
#define SMC2_CSR2  (*(pAT91_REGS + 0xF78 / 4)) /* */
#define SMC2_CSR3  (*(pAT91_REGS + 0xF7C / 4)) /* */
#define SMC2_CSR4  (*(pAT91_REGS + 0xF80 / 4)) /* */
#define SMC2_CSR5  (*(pAT91_REGS + 0xF84 / 4)) /* */
#define SMC2_CSR6  (*(pAT91_REGS + 0xF88 / 4)) /* */
#define SMC2_CSR7  (*(pAT91_REGS + 0xF8C / 4)) /* */

/* Bus setup, nearly IDE timing, taken from Atmel hdd-Demo, set ACSS value for LCD timing */
/* unused masks are signed as comment, they are used for different modes */

#define SMC2_NWS                (0xA <<  0) /* (SMC2) Number of Wait States 10 + 1,5 CLKs ~ 191ns (RD & WR pulse length; max. 0x7F) */
#define SMC2_WSEN               (0x1 <<  7) /* (SMC2) Wait State Enable */
#define SMC2_TDF                (0x0 <<  8) /* (SMC2) Data Float Time 0 (max. 0xF) */
#define SMC2_BAT                (0x1 << 12) /* (SMC2) Byte Access Type */
//#define SMC2_DBW              (0x1 << 13) /* (SMC2) Data Bus Width */
//#define SMC2_DBW_16           (0x1 << 13) /* (SMC2) 16-bit. */
#define SMC2_DBW_8              (0x2 << 13) /* (SMC2) 8-bit. */
#define SMC2_DRP                (0x0 << 15) /* (SMC2) Data Read Protocol */
//#define SMC2_ACSS             (0x3 << 16) /* (SMC2) Address to Chip Select Setup */
//#define SMC2_ACSS_STANDARD      (0x0 << 16) /* (SMC2) Standard, asserted at the beginning of the access and deasserted at the end. */
//#define SMC2_ACSS_1_CYCLE     (0x1 << 16) /* (SMC2) One cycle less at the beginning and the end of the access. */
#define SMC2_ACSS_2_CYCLES    (0x2 << 16) /* (SMC2) Two cycles less at the beginning and the end of the access. */
//#define SMC2_ACSS_3_CYCLES    (0x3 << 16) /* (SMC2) Three cycles less at the beginning and the end of the access. */
#define SMC2_RWSETUP            (0x5 << 24) /* (SMC2) Read and Write Signal Setup Time 5 + 0.5 CLKs ~ 92ns (max. 0x7) */
#define SMC2_RWHOLD             (0x3 << 28) /* (SMC2) Read and Write Signal Hold Time 3 + 0.5 CLKs ~ 58ns (max. 0x7) */


/* this is the memory map base function, should work on all ARM architectures */
/* the physical adress of memory and the memory block size are given as parameter */
void *pMapmemory(off_t phy_addr, size_t phy_length)
{
#define MAP_PAGESIZE 4096UL

int iFd;
void *pMem;

    if ((phy_addr % MAP_PAGESIZE) != 0) {
    fprintf(stderr,"physical address not aligned on PAGESIZE boundary!\n");
    return(NULL);
    }

    if ((phy_length % MAP_PAGESIZE) != 0) {
    fprintf(stderr,"physical length not aligned on PAGESIZE boundary!\n");
    return(NULL);
    }

    /* open mem device for read/write */
    iFd = open("/dev/mem", O_RDWR | O_SYNC);
    if (iFd < 0) {
    fprintf(stderr,"open of /dev/mem fail !\n");
    return(NULL);
    }

    /* get pointer to DNP9200 memory */
    pMem = mmap(NULL,
        phy_length,
        (PROT_READ | PROT_WRITE),
        MAP_SHARED,
        iFd,
        phy_addr);

    if ((pMem == MAP_FAILED) || (pMem == NULL)) {
    fprintf(stderr,"mmap of /dev/mem fail !\n");
    return(NULL);
    }

    /* close mem device */
    if (close(iFd) != 0)
    fprintf(stderr,"close of /dev/mem fail !\n");

    return(pMem);
}

#endif
