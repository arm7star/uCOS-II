
#ifndef __LPC2106_VAL__ 
#define __LPC2106_VAL__

#ifdef __cplusplus
extern "C" {
#endif

/********************************/
/*      ARM specital code      */
/********************************/
//This segment should not be modify

/********************************/
/*Application Program Configurations*/
/********************************/
//This segment could be modified as needed.

#include	"type.h"

/********************************/
/*Configuration of the example */
/********************************/
/* System configuration .Fosc、Fcclk、Fcco、Fpclk must be defined */
#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz��should be the same as actual status. 
#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
#define Fpclk           (Fcclk / 4) * 1             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).

// LPC21000 misc uart0 definitions
#define UART0_PCB_PINSEL_CFG     (u32_t)0x00000005
#define UART0_INT_BIT            (u32_t)0x0040
#define LCR_DISABLE_LATCH_ACCESS (u32_t)0x00000000
#define LCR_ENABLE_LATCH_ACCESS  (u32_t)0x00000080
#define LCR_DISABLE_BREAK_TRANS  (u32_t)0x00000000
#define LCR_ODD_PARITY           (u32_t)0x00000000
#define LCR_ENABLE_PARITY        (u32_t)0x00000008
#define LCR_1_STOP_BIT           (u32_t)0x00000000
#define LCR_CHAR_LENGTH_8        (u32_t)0x00000003 
#define LSR_THR_EMPTY            (u32_t)0x00000020

#ifdef __cplusplus
}
#endif


#endif // __LPC2106_VAL__

