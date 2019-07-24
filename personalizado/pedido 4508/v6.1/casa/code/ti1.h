/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : TI1.H
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : TimerInt
**
**     Version   : Bean 02.055, Driver 01.10, CPU db: 2.87.041
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/23/mi�rcoles, 16:18
**
**     Abstract  :
**
**         This bean "TimerInt" implements a periodic interrupt.
**         When the bean and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload 
**         register or timer-overflow interrupt (of free running counter).
**
**     Settings  :
**
**         Timer name                  : TIM2 (16-bit)
**         Compare name                : TIM20
**         Counter shared              : No
**
**         High-speed CPU mode
**             Prescaler               : divide-by-1
**             Clock                   : 4915200 Hz
**           Initial period/frequency
**             Xtal ticks              : 33
**             microseconds            : 1000
**             milliseconds            : 1
**             seconds (real)          : 0.0010000
**             Hz                      : 1000
**             kHz                     : 1
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : T2CNT     [44]
**              Mode                   : T2SC      [43]
**              Run                    : T2SC      [43]
**              Prescaler              : T2SC      [43]
**
**         Compare registers
**              Compare                : T2CH0     [49]
**
**         Flip-flop registers
**              Mode                   : T2SC0     [48]
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

#ifndef __TI1
#define __TI1

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


/* MODULE TI1. */

#include "Cpu.h"

__interrupt void  TI1_Interrupt(void);
/*
** ===================================================================
**     Method      :  TI1_Interrupt (bean TimerInt)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/

void TI1_Init(void);
/*
** ===================================================================
**     Method      :  TI1_Init (bean TimerInt)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/

/* END TI1. */

#endif /* ifndef __TI1 */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/