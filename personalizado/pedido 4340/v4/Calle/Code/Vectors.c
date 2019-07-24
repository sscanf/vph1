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
**     Date/Time : 26/5/05, 9:36
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
**             Initialization interrupt priority : 1
**             LVI module          : no
**
**         Speed modes
**             High speed clock    : Main clock frequency / 1
**             Internal bus clock  : 4.9152 MHz
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

#include "Cpu.h"
#include "IRQ.h"
#include "TIMER.h"

extern void _Startup(void);

void (* const _vect[])() @0xFFDC = {   // Interrupt table
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          TIMER_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          Cpu_Interrupt,
          IRQ_Interrupt,
          Cpu_SWIInterrupt,
          _Startup
   };
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
