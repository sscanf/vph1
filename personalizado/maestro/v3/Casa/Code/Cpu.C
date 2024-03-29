/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : Cpu.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : MC68HC908GP32_44
**
**     Version   : Bean 01.062, Driver 01.06, CPU db: 2.87.041
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/19/lunes, 9:58
**
**     Abstract  :
**
**         This bean "MC68HC908GP32_44" implements properties, methods,
**         and events of the CPU.
**
**     Settings  :
**
**         Clock setting
**             External clock      : 0.032768 MHz
**             CPU mode selection  : User mode
**             Initialization interrupt priority : 0
**             LVI module          : no
**
**         Speed modes
**             High speed clock    : Main clock frequency / 1
**             Internal bus clock  : 2.4576 MHz
**             Low speed mode      : Disabled
**             Slow speed mode     : Disabled
**
**     Contents  :
**
**         GetSpeedMode - byte Cpu_GetSpeedMode(void);
**         EnableInt    - void Cpu_EnableInt(void);
**         DisableInt   - void Cpu_DisableInt(void);
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

/* MODULE Cpu. */
#include "Teclado.h"
#include "Terminal.h"
#include "IRQ.h"
#include "TIMER.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"

  #define CGM_DELAY  255


/* Global variables */
byte CCR_reg;                          /* Current CCR register */
byte CpuMode = HIGH_SPEED;             /* Current speed mode */


/*
** ===================================================================
**     Method      :  Cpu_Interrupt (bean MC68HC908GP32_44)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
__interrupt void Cpu_Interrupt(void)
{
}

/*
** ===================================================================
**     Method      :  Cpu_SWIInterrupt (bean MC68HC908GP32_44)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
__interrupt void Cpu_SWIInterrupt(void)
{
  Cpu_OnSwINT();                       /* Invoke user event */
}

/*
** ===================================================================
**     Method      :  Cpu_DisableInt (bean MC68HC908GP32_44)
**
**     Description :
**         Disable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_DisableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (bean MC68HC908GP32_44)
**
**     Description :
**         Enable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_EnableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_GetSpeedMode (bean MC68HC908GP32_44)
**
**     Description :
**         Get current speed mode
**     Parameters  : None
**     Returns     :
**         ---             - Speed mode (HIGH_SPEED, LOW_SPEED,
**                           SLOW_SPEED)
** ===================================================================
*/
byte Cpu_GetSpeedMode(void)
{
  return CpuMode;                      /* Result the actual cpu mode */
}

/*
** ===================================================================
**         There is beginning of initialization code for CPU 
**         and embedded beans.
** ===================================================================
*/
void __low_level_init(void)
{
  /* Common initialization of the CPU registers */
  output( PTAPUE, input( PTAPUE ) & ~63 );
  output( DDRA, input( DDRA ) & ~63 );
  /* ### MC68HC908GP32_44 "Cpu" init code ... */
  CONFIG1 = 17;                        /* Set the CONFIG1 register */
  CONFIG2 = 1;                         /* Set the CONFIG2 register */
  PCTL_BCS = 0;                        /* Select clock source from XTAL */
  PCTL_PLLON = 0;                      /* Disable the PLL */
  PMS = 300;                           /* Set the multiplier */
  PMRS = 128;                          /* Set the range select */
  PCTL = 0;
  PCTL_VPR = 1;
  PBWC = 128;                          /* Select the operating modes */
  PCTL_PLLON = 1;                      /* Enable the PLL */
  while(!PBWC_LOCK);                   /* Wait */
  PCTL_BCS = 1;                        /* Select clock source from PLL */
  __asm("nop");
  __asm("nop");
  /* ### Asynchro serial "Terminal" init code ... */
  PTE |= 1;
  DDRE |= 1;
  Terminal_Init();
  /* ### External interrupt "IRQ" init code ... */
  INTSCR_MODE = 1;
  INTSCR_IMASK = 1;
  INTSCR_ACK = 1;
  INTSCR_IMASK = 0;
  /* ### TimerInt "TIMER" init code ... */
  TIMER_Init();
  __DI();                              /* Disable interrupts */
}

/* END Cpu. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
