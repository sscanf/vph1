/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : IO_Map.H
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : IO_Map
**
**     Version   : Driver 01.02
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 11/7/viernes, 13:55
**
**     Abstract  :
**
**         This bean "IO_Map" implements an IO devices mapping.
**
**     Settings  :
**
**
**
**     Contents  :
**
**         No public methods
**
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2002
**
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
**
** ###################################################################
*/
/* Based on CPU DB MC68HC908GP32_44, version 2.82.031 */
#include "PE_types.h"
#include "IO_Map.h"

volatile ADCLKSTR _ADCLK; 
volatile ADRSTR _ADR;          /* ADC Data Register */
volatile ADSCRSTR _ADSCR;      /* ADC Status and Control Register */
volatile BRKHSTR _BRKH;        /* Break Address Register */
volatile BRKLSTR _BRKL;        /* Break Address Register */
volatile BRKSCRSTR _BRKSCR;    /* Break Status and Control Register */
volatile CONFIG1STR _CONFIG1;  /* Configuration Register 2 */
volatile CONFIG2STR _CONFIG2;  /* Configuration Register 2 */
volatile COPCTLSTR _COPCTL;    /* COP Control Register */
volatile DDRASTR _DDRA;        /* Data Direction Register A */
volatile DDRBSTR _DDRB;        /* Data Direction Register B */
volatile DDRCSTR _DDRC;        /* Data Direction Register C */
volatile DDRDSTR _DDRD;        /* Data Direction Register D */
volatile DDRESTR _DDRE;        /* Data Direction Register E */
volatile FLBPRSTR _FLBPR;      /* FLASH1 Block Protect Register */
volatile FLCR1STR _FLCR1;      /* FLASH1 Control Register */
volatile INT1STR _INT1;        /* Interrupt Status Register 1 */
volatile INT2STR _INT2;        /* Interrupt Status Register 2 */
volatile INT3STR _INT3;        /* Interrupt Status Register 3 */
volatile INTKBIERSTR _INTKBIER; /* Keyobard Interrupt Enable Register */
volatile INTKBSCRSTR _INTKBSCR; /* Keyboard Status and Control Register */
volatile INTSCRSTR _INTSCR;    /* IRQ Status and Control Register */
volatile LVISRSTR _LVISR;      /* LVI Status Register */
volatile PBWCSTR _PBWC;        /* PLL Bandwidth Control Register */
volatile PCTLSTR _PCTL;        /* PLL Bandwidth Control Register */
volatile PMDSSTR _PMDS;        /* PLL Reference Divider Select Register */
volatile PMRSSTR _PMRS;        /* PLL VCO Range Select */
volatile PMSHSTR _PMSH;        /* PLL Multiplier Select Register High */
volatile PMSLSTR _PMSL;        /* PLL Multiplier Select Register Low */
volatile PTASTR _PTA;          /* Port A Data Register */
volatile PTAPUESTR _PTAPUE;    /* Port A Input Pullup Enable */
volatile PTBSTR _PTB;          /* Port B Data Register */
volatile PTCSTR _PTC;          /* Port C Data Register */
volatile PTCPUESTR _PTCPUE;    /* Port C Input Pullup Enable */
volatile PTDSTR _PTD;          /* Port D Data Register */
volatile PTDPUESTR _PTDPUE;    /* Port D Input Pullup Enable */
volatile PTESTR _PTE;          /* Port E Data Register */
volatile SBFCRSTR _SBFCR;      /* SIM Break Flag Control Register */
volatile SBSRSTR _SBSR;        /* SIM Break Status Register */
volatile SCBRSTR _SCBR;        /* SCI Baud Rate Register */
volatile SCC1STR _SCC1;        /* SCI Control Register 1 */
volatile SCC2STR _SCC2;        /* SCI Control Register 2 */
volatile SCC3STR _SCC3;        /* SCI Control Register 3 */
volatile SCDRSTR _SCDR;        /* SCI Data Register */
volatile SCS1STR _SCS1;        /* SCI Status Register 1 */
volatile SCS2STR _SCS2;        /* SCI Status Register 2 */
volatile SPCRSTR _SPCR;        /* SPI Control Register */
volatile SPDRSTR _SPDR;        /* SPI Data Register */
volatile SPSCRSTR _SPSCR;      /* SPI Status and Control Register */
volatile SRSRSTR _SRSR;        /* SIM Reset Status Register */
volatile T1CNTHSTR _T1CNTH;    /* TIM1 Counter Register */
volatile T1CNTLSTR _T1CNTL;    /* TIM1 Counter Register */
volatile T1CH0HSTR _T1CH0H;    /* TIM1 Channel Register */
volatile T1CH0LSTR _T1CH0L;    /* TIM1 Channel Register */
volatile T1CH1HSTR _T1CH1H;    /* TIM1 Channel Register */
volatile T1CH1LSTR _T1CH1L;    /* TIM1 Channel Register */
volatile T1MODHSTR _T1MODH;    /* TIM1 Counter Modulo Register */
volatile T1MODLSTR _T1MODL;    /* TIM1 Counter Modulo Register */
volatile T1SCSTR _T1SC;        /* TIM1 Status and Control Register */
volatile T1SC0STR _T1SC0;      /* TIM1 Channel Status and Control Register */
volatile T1SC1STR _T1SC1;      /* TIM1 Channel Status and Control Register */
volatile T2CNTHSTR _T2CNTH;    /* TIM2 Counter Register */
volatile T2CNTLSTR _T2CNTL;    /* TIM2 Counter Register */
volatile T2CH0HSTR _T2CH0H;    /* TIM2 Channel Register */
volatile T2CH0LSTR _T2CH0L;    /* TIM2 Channel Register */
volatile T2CH1HSTR _T2CH1H;    /* TIM2 Channel Register */
volatile T2CH1LSTR _T2CH1L;    /* TIM2 Channel Register */
volatile T2MODHSTR _T2MODH;    /* TIM2 Counter Modulo Register */
volatile T2MODLSTR _T2MODL;    /* TIM2 Counter Modulo Register */
volatile T2SCSTR _T2SC;        /* TIM2 Status and Control Register */
volatile T2SC0STR _T2SC0;      /* TIM2 Channel Status and Control Register */
volatile T2SC1STR _T2SC1;      /* TIM2 Channel Status and Control Register */
volatile TBCRSTR _TBCR;        /* Timebase Control Register */
volatile BRKSTR _BRK;          /* Break Address Register */
volatile PMSSTR _PMS;          /* PLL Multiplier Select Register */
volatile T1CNTSTR _T1CNT;      /* TIM1 Counter Register */
volatile T1CH0STR _T1CH0;      /* TIM1 Channel Register */
volatile T1CH1STR _T1CH1;      /* TIM1 Channel Register */
volatile T1MODSTR _T1MOD;      /* TIM1 Counter Modulo Register */
volatile T2CNTSTR _T2CNT;      /* TIM2 Counter Register */
volatile T2CH0STR _T2CH0;      /* TIM2 Channel Register */
volatile T2CH1STR _T2CH1;      /* TIM2 Channel Register */
volatile T2MODSTR _T2MOD;      /* TIM2 Counter Modulo Register */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
