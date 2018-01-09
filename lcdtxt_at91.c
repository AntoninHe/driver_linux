/*****************************************************************************/
/* File : lcdtxt_at91.c, based on lcdtxt.c                                   */
/*        it contains low level routines to access graphic display on        */ 
/*        DNP/EVA11 (T6963C-Controller from Tshiba) from userspace.          */ 
/*        This file is just for TEXT MODE of the LCD                         */   
/* 18.12.00 ARE                                                              */
/* 14.09.01 ARE                                                              */
/* 18.10.01 ARE: LINUX                                                       */
/* 08.08.06 SSC: LINUX/AT91                                                  */
/*****************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//#include <asm/io.h>
#include <unistd.h>
#include "lcdtxt_at91.h"
#include "ssv_at91rm9200.h"

#define VERSION      "1.00-20061808"

#define Set_xy(x, y) wr_cmd_2Data(LCD_ADRESS_PTR_SET,(unsigned char) (y << 5) | (((unsigned char) x) / 6u),(GR_HOME_ADR/256) + ((y >> 3) & 0x0007))

#define DEBUG 0

short Pixel_Add;
unsigned char Pixel_Byte;

unsigned char lcd_addr;        /*  Offset-Address in Display RAM   */
unsigned int  cur_addr;        /*  current Cursor-Position         */
unsigned int  cur_type = 1;

/*****************************************************************************/
/* Function..: init_AT91_REGS, init the AT91 registers for memory access     */
/* Parameter.: non                                                           */
/* Var.......: non                                                           */
/* Return....: 0 (success), -1 (failed)                                      */
/*****************************************************************************/
int init_AT91_REGS (void)
{
    /* map AT91 PIO register space at 0xFFFFFF000 / size 4096 bytes */
    pAT91_REGS = pMapmemory(0xFFFFF000, 0x00001000);
    if (!pAT91_REGS) {
    printf("MMAP pAT91_REGS fail !\n");
    exit(-1);
    }
    return(0);
}

/*****************************************************************************/
/* Function..: init_CS2_EXT, init external CS2 and map adress range to mem   */
/* Parameter.: non                                                           */
/* Var.......: non                                                           */
/* Return....: 0 (success), -1 (failed)                                      */
/*****************************************************************************/
int init_CS2_EXT (void)
{
    /* map CPU_CS4 to external memory space at 0x50000000 / size 4096 bytes */
    pCS2_EXT = pMapmemory(0x50000000, 0x00001000);
    if (!pCS2_EXT) {
    printf("MMAP pCS2_EXT fail !\n");
    exit(-1);
    }
    return(0);
}

/*****************************************************************************/
/* Function..: init_CS2_bus, set bus parameters for CS2 (look in ssv_AT91.h) */
/* Parameter.: non                                                           */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void init_CS2_bus(void) /* init external bus timing for CS2 memory space, DNP's CS2 is connected to CPU_CS4 */
{
    SMC2_CSR4 =(SMC2_RWHOLD|SMC2_RWSETUP|SMC2_ACSS_2_CYCLES|SMC2_DRP|SMC2_DBW_8|SMC2_BAT|SMC2_TDF|SMC2_WSEN|SMC2_NWS);
}

/*****************************************************************************/
/* Function..: read_lcd_port, reads data from the T6963C                     */
/* Parameter.: non                                                           */
/* Var.......: read_value, read_value                                        */
/* Return....: read_value                                                    */
/*****************************************************************************/
static unsigned char read_lcd_port (void)
{
   unsigned char read_value;
   
   read_value = *(pCS2_EXT + 4); /* LCD command register */
   return (read_value);
}

/*****************************************************************************/
/* Function..: write_lcd_port, Writes commands or data into the T6963C       */
/* Parameter.: mode   ,  Data or Commands mode                               */
/*             cmd_dat,  Data or Commands                                    */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void write_lcd_port (int mode, unsigned char cmd_dat)
{
   while ((read_lcd_port() & 0x03) != 0x03) {
      /* wait while LCD is busy!!! */
   } /* endwhile */

#if DEBUG
   if (mode == 1) {
      printf (" [%02X]\n", cmd_dat);
   } else {
      printf (" (%02X)", cmd_dat);
   } /* endif */
#endif

   /* Send Command */
   if (mode == 1)
    *(pCS2_EXT + 4) = cmd_dat; /* LCD command register */

   /* Send Data */
   if (mode == 0)
    *(pCS2_EXT + 0) = cmd_dat; /* LCD data register */
}

/*****************************************************************************/
/* Function..: wr_data, writes data on the BUS                               */
/* Parameter.: ch, value to write                                            */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void wr_data (unsigned char ch)
{
   write_lcd_port(DAT,ch);
}

/*****************************************************************************/
/* Function..: wr_cmd, writes a command on the BUS                           */
/* Parameter.: command, the command which will be sent on the BUS            */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void wr_cmd (unsigned char command)
{
   write_lcd_port(CMD,command); //CMD ==> mode = Command
}

/*****************************************************************************/
/* Function..: wr_cmd_1Data, writes a command and data on the BUS            */
/* Parameter.: command, the command which will be sent on the BUS            */
/*             data1  , the data which must be written                       */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void wr_cmd_1Data (unsigned char command, unsigned char data1)
{
   wr_data (data1);
   wr_cmd (command);
}

/*****************************************************************************/
/* Function..: wr_cmd_2Data, writes a command and data on the BUS            */
/* Parameter.: command, the command which will be sent on the BUS            */
/*             data1  ,  the 1. data which must be written                   */
/*             data2  ,  the 2. data which must be written                   */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void wr_cmd_2Data (unsigned char command,
                   unsigned char data1,
                   unsigned char data2)
{
   wr_data (data1);
   wr_data (data2);
   wr_cmd (command);
}

/* 18.10.01 ARE */
#ifdef _OLD_
/*****************************************************************************/
/* Function..: lcd_set_cur, sets the Cursor position                         */
/* Parameter.: cur, Position (0xYY XX)                                       */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_set_cur(int cur)
{
   wr_cmd_2Data(LCD_ADRESS_PTR_SET,(unsigned char)
                (cur>>3)|(unsigned char)(cur & 0x001F),0);
}
#endif

/*****************************************************************************/
/* Function..: lcd_gotoxy, sets the Cursor position                          */
/* OLD.......: lcd_set_cur                                                   */
/* Parameter.: x, X-Koordinate                                               */
/*             y, Y-Koordinate                                               */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/* !!!!! Achtung !!!!!                                                       */
/* In diesem Code wird nur der sichtbare Cursor auf dem Display              */
/* verschoben. Das Setzen des Adress-Pointers zum Schreiben wird             */
/* erst kurz vor dem Schreiben des Zeichens gesetzt. Die Berechnung          */
/* der Hardware-Adresse (Offset) erfolgt hier und wird in der                */
/* Variable "lcd_addr" gespeichert.                                          */
/*****************************************************************************/
void lcd_gotoxy (unsigned char x, unsigned char y)
{
   cur_addr = ((unsigned int) y << 8) + x;

   if (cur_type)
      wr_cmd_2Data(LCD_CURSOR_PTR_SET, x, y);

   lcd_addr = (unsigned char) ((y << 5) | x);

   wr_cmd_2Data(LCD_ADRESS_PTR_SET,(unsigned char)
                (cur_addr>>3)|(unsigned char)(cur_addr & 0x001F),0);
}

/*****************************************************************************/
/* Function..: lcd_putsxy, stringoutput at pos. defined by (x,y)             */
/* Parameter.: x, X-Koordinate                                               */
/*             y, Y-Koordinate                                               */
/*             str, pointer to string                                        */
/* Var.......: zeichen, char from str                                        */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_putsxy (unsigned char x, unsigned char y, char *str)
{
   char zeichen;

   lcd_gotoxy (x, y);

   while ((zeichen = *str++) != 0) {
      wr_cmd_1Data(LCD_DATA_WRITE,(zeichen-0x20));
   } /* endwhile */
}

/*****************************************************************************/
/* Function..: lcd_puts, writes a string to the LCD                          */
/* Parameter.: Text, pointer String                                          */
/* Var.......: zeichen, char from Text                                       */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_puts(char *Text)
{
   char zeichen;

   while ((zeichen = *Text++) != 0) {
      wr_cmd_1Data(LCD_DATA_WRITE,(zeichen-0x20));

   } /* endwhile */
   /* Hide Cursor */
   // wr_cmd_2Data(LCD_CURSOR_PTR_SET,0x1F,0);
}

/*****************************************************************************/
/* Function..: lcd_cls, clear LCD and set cursor at HOME-POS                 */
/* Parameter.: non                                                           */
/* Var.......: cur, counter                                                  */
/*             i  , counter                                                  */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_cls(void)
{
   int cur,i;
   
#define BLANK_CHR 0x00

   for (cur = 0; cur < 32*8; cur += 32)
   {
      /* clear text mem */
      wr_cmd_2Data(LCD_ADRESS_PTR_SET,cur,0);
      for (i=22;i;i--)
         wr_cmd_1Data(LCD_DATA_WRITE, BLANK_CHR);

      /* clear attribut mem */
      wr_cmd_2Data(LCD_ADRESS_PTR_SET,cur,GR_HOME_ADR/256);
      for (i=22;i;i--)
         wr_cmd_1Data(LCD_DATA_WRITE,0x00);
   }


   /* clear graphik mem */
   Pixel_Add = GR_HOME_ADR;
   wr_cmd_2Data (LCD_ADRESS_PTR_SET, Pixel_Add, Pixel_Add >> 8);
   
   for (i = GR_HOME_ADR; i < (GR_COLUMNS * GR_ROWS)+GR_HOME_ADR; i++)
           wr_cmd_1Data(LCD_DATA_WRITE,0x00);

   /* Setze Cursor auf Home-Position */
   // lcd_gotoxy (0,0);
}

/*****************************************************************************/
/* Function..: cur_on_off, switch cur ON/OFF                                 */
/* Parameter.: cur_on =  0 --> Cursor OFF                                    */
/*             cur_on != 0 --> Cursor ON (default)                           */
/* Var.......: non                                                           */
/* Return....: non                                                           */ 
/* ORG HNE...: void PASCAL LCD_Cur_Type (int cur_on)                         */
/*****************************************************************************/
void cur_on_off (int cur_on)
{
   cur_type = cur_on;

   if (cur_on) {
      wr_cmd_2Data (LCD_CURSOR_PTR_SET, cur_addr & 0x001F, cur_addr >> 8);
   } else {
      #define CUR 0x001F 
         wr_cmd_2Data(LCD_CURSOR_PTR_SET, CUR & 0x001F, CUR >> 8);
      #undef CUR
   } /* endif */
} 

/*****************************************************************************/
/* Function..: attr_to_lcd, writes attributes to LCD                         */
/* Parameter.: attribute, att. to write                                      */
/* Var.......: lcd_attribute                                                 */
/* Return....: non                                                           */ 
/*****************************************************************************/
static unsigned char attr_to_lcd (unsigned char attribute)
{
   unsigned char lcd_attribute;

   /* default: normal */
   lcd_attribute = 0;

   /* if attribute blink */
   if (attribute & 0x80)
      lcd_attribute |= 0x08;

   /* inverse */
   if (((attribute & 0x70) >> 4) > ((attribute & 0x07)))
      lcd_attribute |= 0x05;

   if (((attribute & 0x70) >> 4) == ((attribute & 0x07)))
      lcd_attribute = 0x03;

   return (lcd_attribute);
}

/*****************************************************************************/
/* Function..: lcd_putc, writes a char with attribute to LCD                 */
/* Parameter.: zeichen, char to write                                        */
/*             attribute, Attribut of char                                   */
/* Var.......: lcd_attribute,                                                */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_putc (char zeichen, unsigned char attribute)
{
   static unsigned char lcd_attribute;
   
   zeichen -= 0x20;

   /* Set textaddress */
   wr_cmd_2Data(LCD_ADRESS_PTR_SET, lcd_addr, 0);

   /* write char */
   wr_cmd_1Data(LCD_DATA_WRITE,zeichen);

   lcd_attribute = attr_to_lcd (attribute);

   /* Set address of attribute */
   wr_cmd_2Data(LCD_ADRESS_PTR_SET, lcd_addr, GR_HOME_ADR/256);

   /* write attribute */
   wr_cmd_1Data(LCD_DATA_WRITE, lcd_attribute);
}

/*****************************************************************************/
/* Function..: lcd_putsxy_att, stringoutput with att. at pos. def. by (x,y)  */
/* Parameter.: x, X-Koordinate                                               */
/*             y, Y-Koordinate                                               */
/*             str, pointer to string                                        */
/*             att, attribute of string                                      */ 
/* Var.......: zeichen, char from str                                        */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_putsxy_att (unsigned char x, unsigned char y,
                     char *str, unsigned char att)
{
   int i;
   for (i = 0; str[i]; i++) {
      lcd_gotoxy (x, y);
      lcd_putc (str[i], att);
      x++;
      #if LCD_DEMO
         delay (60);
      #endif
   } /* endfor */
}
/*****************************************************************************/
/* Function..: lcd_init, initializes T6963C-Controller for text, (A)DNP      */
/* Parameter.: non                                                           */
/* Var.......: non                                                           */
/* Return....: non                                                           */
/*****************************************************************************/
void lcd_init (void)
{
   wr_cmd(LCD_DISPLAY_TXT_GR_MODE);
   wr_cmd(LCD_MODE_SET_ATTRIBUTE);
   wr_cmd_2Data(LCD_OFFSET_REG_SET,0,0);
   wr_cmd_2Data(LCD_TEXT_HOME_ADD,0x00,0x00);
   wr_cmd_2Data(LCD_TEXT_AREA_SET,0x20,0);
   wr_cmd_2Data(LCD_GRAPH_HOME_ADD,0x00,GR_HOME_ADR/256);
   wr_cmd_2Data(LCD_GRAPH_AREA_SET,0x20,0);
   wr_cmd(LCD_CURSOR_8_LINE);
   lcd_cls ();
}

/*****************************************************************************/
/* Function..: main, uses arguments from command line to display text on LCD */
/* Parameter.: argc, is counting arguments                                   */
/*             *argv[] pointer to commandline parameter                      */
/* Var.......: buffer, line, position                                        */
/* Return....: 0 (success), 1 (errrors)                                      */
/*****************************************************************************/
int demo(int argc, char *argv[])
{
    u8 buffer[24];
    u32 line, position;

    /* check user identity */
    if (geteuid() != 0) {
    printf("No root access rights !\n");
    exit(1);
    }

    init_AT91_REGS();
    init_CS2_EXT ();
    init_CS2_bus ();

    if (argc == 2) {
	if (!strcasecmp(argv[1], "cls"))
	    lcd_cls();
	if (!strcasecmp(argv[1], "init"))
	    lcd_init();
	else
	goto usage;
    } else
    if (argc == 3)
        goto usage;

    if (argc == 4) {
        position = strtoul(argv[1], NULL, 0);
        line = strtoul(argv[2], NULL, 0);
        if ((line > 7) | (position > 23))
            goto usage;
        memset(buffer, 0x20, sizeof(buffer));
        strncpy(buffer, argv[3], (strlen(argv[3])>24) ? sizeof(buffer):strlen(argv[3]));
        lcd_putsxy (position, line, buffer);

    } else {
usage:
    printf("lcdtxt -- Version: %s\n"
           "---------------------------------------\n"
           "print chars on Graphic LCD in text mode\n\n"
           "usage: %s [cls] | [init] | [<lcdtextpos0..23> <lcdline0..7> <lcdtext>]\n",
           VERSION, argv[0]);
    return(1);
    }

    return 0;
}


int affichage_lcd(int pos_x, int pos_y, char* texte)
{
    u8 buffer[24];
    u32 line, position;

    init_AT91_REGS();
    init_CS2_EXT ();
    init_CS2_bus ();

    lcd_cls();

	position = (u32)pos_x;
	line = (u32)pos_y;
	if ((line > 7) | (position > 23)) return -1;
	memset(buffer, 0x20, sizeof(buffer));
	strncpy(buffer, texte, (strlen(texte)>24) ? sizeof(buffer):strlen(texte));
	lcd_putsxy (position, line, buffer);

    return 0;
}





