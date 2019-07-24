/*
** ###################################################################
**
**     Filename  : Events.H
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : Events
**
**     Version   : Driver 01.00
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/17/jueves, 15:23
**
**     Abstract  :
**
**         This is user's event module.
**         Put your event handler code here.
**
**     Settings  :
**
**
**     Contents  :
**
**         Cpu_OnSwINT - void Cpu_OnSwINT(void);
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

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

void Cpu_OnSwINT(void);
/*
** ===================================================================
**     Event       :  Cpu_OnSwINT (module Events)
**
**     From bean   :  Cpu [MC68HC908GP32_44]
**     Description :
**         This software event is called after software reset.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
**     Event       :  IRQ_OnInterrupt (module Events)
**
**     From bean   :  IRQ [ExtInt]
**     Description :
**         This event is called when the active signal edge/level
**         occurs.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void IRQ_OnInterrupt(void);
void TIMER_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TIMER_OnInterrupt (module Events)
**
**     From bean   :  TIMER [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SPI_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SPI_OnRxChar (module Events)
**
**     From bean   :  SPI [SynchroMaster]
**     Description :
**         This event is called after a correct character is
**         received.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SPI_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SPI_OnTxChar (module Events)
**
**     From bean   :  SPI [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SPI_OnError(void);
/*
** ===================================================================
**     Event       :  SPI_OnError (module Events)
**
**     From bean   :  SPI [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be
**         read using GetError method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/

extern unsigned char even_timer;
extern unsigned long even_timeout;
