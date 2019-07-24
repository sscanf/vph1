/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : IRQ.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : ExtInt
**
**     Version   : Bean 02.024, Driver 01.17, CPU db: 2.89.000
**
**     Compiler  : Metrowerks HC08 C Compiler
**
**     Date/Time : 05/01/2007, 11:43
**
**     Abstract  :
**
**         This bean "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The bean uses one pin which generates interrupt on 
**         selected edge.
**
**     Settings  :
**
**         Interrupt name              : INT_IRQ
**         User handling procedure     : IRQ_OnInterrupt
**
**         Used pin                    : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       11            |  IRQ
**             ----------------------------------------------------
**
**         Signal edge/level           : low
**         Initial state               : Enabled
**
**         Edge register               : INTSCR    [29]
**         Enable register             : INTSCR    [29]
**         Request register            : INTSCR    [29]
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

#include "IRQ.h"

/*Including shared modules, which are used for all project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"

#pragma MESSAGE DISABLE C4002          /* Disable warning C4002 "Result not used" */

/*
** ===================================================================
**     Method      :  IRQ_Interrupt (bean ExtInt)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
__interrupt void IRQ_Interrupt(void)
{
  IRQ_OnInterrupt();
}

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.23 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/