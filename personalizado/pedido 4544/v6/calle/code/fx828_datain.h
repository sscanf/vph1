/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : FX828_DataIn.H
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
**         Methods of this bean are mostly implemented as a macros 
**         (if supported by target language and compiler).
**
**     Settings  :
**
**         Used pin                    : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       13            |  PTD1_MISO
**             ----------------------------------------------------
**
**         Port name                   : PTD
**
**         Bit number (in port)        : 1
**         Bit mask of the port        : 2
**
**         Initial direction           : Input (direction can be changed)
**         Safe mode                   : yes
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
**         SetDir - void FX828_DataIn_SetDir(bool Output);
**         GetVal - bool FX828_DataIn_GetVal(void);
**         PutVal - void FX828_DataIn_PutVal(bool Output);
**         ClrVal - void FX828_DataIn_ClrVal(void);
**         SetVal - void FX828_DataIn_SetVal(void);
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

#ifndef __FX828_DataIn_H
#define __FX828_DataIn_H


/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

/*
** ===================================================================
**     Method      :  FX828_DataIn_GetVal (bean BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---        - Input value (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
** ===================================================================
*/
#define FX828_DataIn_GetVal() ((PTD) & 2)

/*
** ===================================================================
**     Method      :  FX828_DataIn_PutVal (bean BitIO)
**
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Value      - Output value (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
**     Returns     : Nothing
** ===================================================================
*/
void FX828_DataIn_PutVal(byte Value);


/*
** ===================================================================
**     Method      :  FX828_DataIn_SetDir (bean BitIO)
**
**     Description :
**         This method sets direction of the bean.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void FX828_DataIn_SetDir(byte Output);



/*
** ===================================================================
**     Method      :  FX828_DataIn_ClrVal (bean BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define FX828_DataIn_ClrVal() ((Shadow_PTD &= ~2), (PTD &= ~2))

/*
** ===================================================================
**     Method      :  FX828_DataIn_SetVal (bean BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define FX828_DataIn_SetVal() ((Shadow_PTD |= 2), (PTD |= 2))


#endif /* __FX828_DataIn_H*/
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/