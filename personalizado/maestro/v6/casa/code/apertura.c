/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : APERTURA.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : BitsIO
**
**     Version   : Bean 02.022, Driver 02.16, CPU db: 2.89.000
**
**     Compiler  : Metrowerks HC08 C Compiler
**
**     Date/Time : 20/02/2006, 13:59
**
**     Abstract  :
**
**         This bean "BitsIO" implements a multi-bit input/output.
**         It uses selected pins of one 1-bit to 8-bit port.
**
**     Settings  :
**
**         Port name                   : PTC
**
**         Bit mask of the port        : 48
**         Number of bits/pins         : 2
**         Single bit numbers          : 0 to 1
**         Values range                : 0 to 3
**
**         Initial direction           : Input (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0 = 000H
**         Initial pull option         : off
**
**         Port data register          : PTC       [2]
**         Port control register       : DDRC      [6]
**
**             ----------------------------------------------------
**                   Bit     |   Pin   |   Name
**             ----------------------------------------------------
**                    0      |    6    |   PTC4
**                    1      |    7    |   PTC5
**             ----------------------------------------------------
**
**         Optimization for            : speed
**
**     Contents  :
**
**         GetDir - bool APERTURA_GetDir(void);
**         SetDir - void APERTURA_SetDir(bool Dir);
**         GetVal - byte APERTURA_GetVal(void);
**         PutVal - void APERTURA_PutVal(byte Val);
**         GetBit - bool APERTURA_GetBit(byte Bit);
**         PutBit - void APERTURA_PutBit(byte Bit,bool Val);
**         SetBit - void APERTURA_SetBit(byte Bit);
**         ClrBit - void APERTURA_ClrBit(byte Bit);
**         NegBit - void APERTURA_NegBit(byte Bit);
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

#include "APERTURA.h"

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

const byte APERTURA_Table[8]={ 1, 2, 4, 8, 16, 32, 64, 128 }; /* Table of mask constants */
/*
** ===================================================================
**     Method      :  APERTURA_GetMsk (bean BitsIO)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
byte APERTURA_GetMsk(byte Value)
{
  return((Value<2)?APERTURA_Table[Value+4]:0); /* Return appropriate bit mask */
}

/*
** ===================================================================
**     Method      :  APERTURA_GetVal (bean BitsIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pins and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---        - Input value (0 to 3)
** ===================================================================
*/
/*
byte APERTURA_GetVal(void)

**  This method is implemented as a macro. See APERTURA.h file.  **
*/

/*
** ===================================================================
**     Method      :  APERTURA_PutVal (bean BitsIO)
**
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pins
**     Parameters  :
**         NAME       - DESCRIPTION
**         Value      - Output value (0 to 3)
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_PutVal(byte Value)
{
  byte Temp;                           /* Temporary variable */

  Temp = (Value<<4) & 48;              /* Prepare value for output */
  Shadow_PTC = Shadow_PTC & ~48 | Temp; /* Set-up bits in shadow variable */
  PTC = PTC & ~48 | Temp;              /* Set-up bits on port */
}

/*
** ===================================================================
**     Method      :  APERTURA_GetBit (bean BitsIO)
**
**     Description :
**         This method returns the specified bit of the input value.
**           a) direction = Input  : reads the input value from pins
**                                   and returns the specified bit
**           b) direction = Output : returns the specified bit
**                                   of the last written value
**     Parameters  :
**         NAME       - DESCRIPTION
**         BitNum     - Number of the bit to read (0 to 1)
**     Returns     :
**         ---        - Value of the specified bit (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
** ===================================================================
*/
bool APERTURA_GetBit(byte BitNum)
{
  byte Mask=APERTURA_GetMsk(BitNum);   /* Temporary variable - bit mask */

  return((Mask)?(PTC & Mask) == Mask:0); /* Return input value */
}

/*
** ===================================================================
**     Method      :  APERTURA_PutBit (bean BitsIO)
**
**     Description :
**         This method writes the new value to the specified bit
**         of the output value.
**           a) direction = Input  : sets the value of the specified
**                                   bit; this operation will be
**                                   shown on output after the
**                                   direction has been switched to
**                                   output (SetDir(TRUE);)
**           b) direction = Output : directly writes the value of the
**                                   bit to the appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         BitNum     - Number of the bit (0 to 1)
**         Value      - New value of the bit (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_PutBit(byte BitNum, byte Value)
{
  byte Mask=APERTURA_GetMsk(BitNum);   /* Temporary variable - bit mask */

  if (Mask)  {                         /* Is bit mask correct? */
    if (Value) {                       /* Is it one to be written? */
      Shadow_PTC |= Mask;              /* Set appropriate bit in shadow variable */
      PTC |= Mask;                     /* Set appropriate bit on port */
    }
    else {                             /* Is it zero to be written? */
      Shadow_PTC &= ~Mask;             /* Clear appropriate bit in shadow variable */
      PTC &= ~Mask;                    /* Clear appropriate bit on port */
    }
  }
}

/*
** ===================================================================
**     Method      :  APERTURA_SetBit (bean BitsIO)
**
**     Description :
**         This method sets (sets to one) the specified bit of the
**         output value.
**         [ It is the same as "PutBit(Bit,TRUE);" ]
**           a) direction = Input  : sets the specified bit to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         BitNum     - Number of the bit to set (0 to 1)
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_SetBit(byte BitNum)
{
  byte Mask=APERTURA_GetMsk(BitNum);   /* Temporary variable - bit mask */

  if (Mask) {                          /* Is bit mask correct? */
    Shadow_PTC |= Mask;                /* Set appropriate bit in shadow variable */
    PTC |= Mask;                       /* Set appropriate bit on port */
  }
}

/*
** ===================================================================
**     Method      :  APERTURA_ClrBit (bean BitsIO)
**
**     Description :
**         This method clears (sets to zero) the specified bit
**         of the output value.
**         [ It is the same as "PutBit(Bit,FALSE);" ]
**           a) direction = Input  : sets the specified bit to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   beenswitched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         BitNum     - Number of the bit to clear (0 to 1)
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_ClrBit(byte BitNum)
{
  byte Mask=APERTURA_GetMsk(BitNum);   /* Temporary variable - bit mask */

  if (Mask) {                          /* Is bit mask correct? */
    Shadow_PTC &= ~Mask;               /* Clear appropriate bit in shadow variable */
    PTC &= ~Mask;                      /* Clear appropriate bit on port */
  }
}

/*
** ===================================================================
**     Method      :  APERTURA_NegBit (bean BitsIO)
**
**     Description :
**         This method negates (invertes) the specified bit of the
**         output value.
**           a) direction = Input  : invertes the specified bit;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly invertes the value
**                                   of the appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         BitNum     - Number of the bit to invert (0 to 31)
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_NegBit(byte BitNum)
{
  byte Mask=APERTURA_GetMsk(BitNum);   /* Temporary variable - bit mask */

  if (Mask) {                          /* Is bit mask correct? */
    Shadow_PTC ^= Mask;                /* Negate appropriate bit in shadow variable */
    PTC ^= Mask;                       /* Negate appropriate bit on port */
  }
}

/*
** ===================================================================
**     Method      :  APERTURA_SetDir (bean BitsIO)
**
**     Description :
**         This method sets direction of the bean.
**     Parameters  :
**         NAME       - DESCRIPTION
**         DirOutput  - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void APERTURA_SetDir(byte DirOutput)
{
  if (DirOutput) {                     /* Is given direction output? */
    PTC = (PTC & ~48)|(Shadow_PTC & 48); /* Restore correct value of output from shadow variable */
    DDRC |= 48;                        /* Set direction to output */
  }
  else                                 /* Is direction input? */
    DDRC &= ~48;                       /* Set direction to input */
}

/*
** ===================================================================
**     Method      :  APERTURA_GetDir (bean BitsIO)
**
**     Description :
**         This method returns direction of the bean.
**     Parameters  : None
**     Returns     :
**         ---        - Direction of the bean (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
** ===================================================================
*/
/*
bool APERTURA_GetDir(void)

**  This method is implemented as a macro. See APERTURA.h file.  **
*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.23 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/