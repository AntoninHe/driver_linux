/*****************************************************************************/
/* File : lcdtxt.h                                                           */
/* 18.12.00 ARE                                                              */
/* 08.08.06 SSC modified for AT91                                            */
/*****************************************************************************/


#define LCD_DISPLAY_TXT_CUR_BLINK_MODE    0x97/* Text on Grafik off, Cur on and blink */
#define LCD_DISPLAY_TXT_GR_CUR_BLINK_MODE 0x9F/* Text on Grafik on, Cur on and blink */
#define LCD_MODE_SET_ATTRIBUTE  0x84    /* Attribute Mode       */
#define LCD_DISPLAY_TXT_MODE    0x97    /* text on gr off       */
#define LCD_DISPLAY_TXT_GR_MODE 0x9C    /* text on, gr onNOCUR  */
#define LCD_MODE_SET_EXOR       0x81    /* EXOR-Mode            */
#define LCD_CURSOR_PTR_SET      0x21    /* Cursor Ptr. Set      */
#define LCD_OFFSET_REG_SET      0x22    /* Offset Reg. Set      */
#define LCD_ADRESS_PTR_SET      0x24    /* Address Ptr. Set     */
#define LCD_TEXT_HOME_ADD       0x40    /* Text Home Add. Set   */
#define LCD_TEXT_AREA_SET       0x41    /* Text Area Set        */
#define LCD_GRAPH_HOME_ADD      0x42    /* Graph. Home Add. Set */
#define LCD_GRAPH_AREA_SET      0x43    /* Graph. Area Set      */
#define LCD_CURSOR_8_LINE       0xA7    /* Cursour 8 Lines      */
#define LCD_DATA_WRITE          0xC0    /* Write Data           */
#define GR_HOME_ADR             0x1000  /* Gr. Home-Add.        */
#define GR_LINE_OFFSET          0x05    /* Zeilenoffset         */
#define GR_COLUMNS              0x20    /* 32 Spalten           */
#define GR_ROWS                 0x40    /* Zeilen               */
#define LCD_BIT_CLR             0xF0    /* 11110xxx */
#define LCD_BIT_SET             0xF8    /* 11111xxx */
#define CMD                     1       /* Command              */
#define DAT                     0       /* Data                 */

extern void lcd_init (void);
extern void lcd_cls (void);
extern void lcd_gotoxy (unsigned char x, unsigned char y);
extern void lcd_putsxy (unsigned char x, unsigned char y, char *str);
extern void lcd_puts(char *Text);
extern void lcd_cls(void);
extern void cur_on_off (int cur_on);
extern void lcd_putc (char zeichen, unsigned char attribute);
extern void lcd_putsxy_att (unsigned char x, unsigned char y, char *str, unsigned char att);
extern int affichage_lcd (int pos_x, int pos_y, char* texte);
extern void affichage_init();
extern void affichage_cls();
