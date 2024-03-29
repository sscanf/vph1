/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : FX828_DataOut.H
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : BitIO
**
**     Version   : Bean 02.015, Driver 02.10, CPU db: 2.87.041
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/28/lunes, 16:43
**
**     Abstract  :
**
**         This bean "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This bean is set to work in Output direction only.
**         Methods of this bean are mostly implemented as a macros 
**         (if supported by target language and compiler).
**
**     Settings  :
**
**         Used pin                    : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       14            |  PTD2_MOSI
**             ----------------------------------------------------
**
**         Port name                   : PTD
**
**         Bit number (in port)        : 2
**         Bit mask of the port        : 4
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTD       [3]
**         Port control register       : DDRD      [7]
**         Port function register      : SPCR      [16]
**
**         Optimization for            : speed
**
**     Contents  :
**
**         GetVal - bool FX828_DataOut_GetVal(void);
**         PutVal - void FX828_DataOut_PutVal(bool Output);
**         ClrVal - void FX828_DataOut_ClrVal(void);
**         SetVal - void FX828_DataOut_SetVal(void);
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

#ifndef __FX828_DataOut_H
#define __FX828_DataOut_H


/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

/*
** ===================================================================
**     Method      :  FX828_DataOut_GetVal (bean BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This bean is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---        - Input value (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
** ===================================================================
*/
#define FX828_DataOut_GetVal() ((PTD) & 4)

/*
** ===================================================================
**     Method      :  FX828_DataOut_PutVal (bean BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Value      - Output value (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
**     Returns     : Nothing
** ===================================================================
*/
void FX828_DataOut_PutVal(byte Value);




/*
** ===================================================================
**     Method      :  FX828_DataOut_ClrVal (bean BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define FX828_DataOut_ClrVal() (PTD &= ~4)

/*
** ===================================================================
**     Method      :  FX828_DataOut_SetVal (bean BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define FX828_DataOut_SetVal() (PTD |= 4)


#endif /* __FX828_DataOut_H*/
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
