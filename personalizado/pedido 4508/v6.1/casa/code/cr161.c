/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : CR161.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : CRC16
**
**     Version   : Bean 01.003, Driver 01.50, CPU db: 2.89.000
**
**     Compiler  : Metrowerks HC08 C Compiler
**
**     Date/Time : 22/7/05, 10:49
**
**     Abstract  :
**
**         This bean "CRC16" implements a CRC16 algorithm. You can use
**         CCITT, ITU-T or user define polynom..
**
**     Settings  :
**
**         Endian  : little
**         Polynom : CCITT
**
**     Contents  :
**
**         Init         - void CR161_Init(word *crc);
**         Update       - void CR161_Update(word *crc,byte *data,word len);
**         Done         - void CR161_Done(word *crc);
**         EncodeBuffer - void CR161_EncodeBuffer(byte *buffer,word len,word *crc);
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


/* MODULE CR161. */

#include "CR161.h"
#include "CR161_Tab.h"

/*
** ===================================================================
**     Method      :  CR161_EncodeBuffer (bean CRC16)
**
**     Description :
**         Enumeration crc for one block.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buffer          - Pointer to buffer
**         len             - Length
**       * crc             - Pointer to crc
**     Returns     : Nothing
** ===================================================================
*/
void CR161_EncodeBuffer(byte *buffer,word len,word *crc)
{
/*
 #####  Here will be code here if you have a full registered version  #####
*/
 
}

/*
** ===================================================================
**     Method      :  CR161_Done (bean CRC16)
**
**     Description :
**         This method returns final value of CRC. This value is
**         calculated from status variable.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * crc             - Pointer to crc
**     Returns     : Nothing
** ===================================================================
*/
/*
void CR161_Done(word *crc)

**      This method is implemented as macro      **
*/

/*
** ===================================================================
**     Method      :  CR161_Update (bean CRC16)
**
**     Description :
**         This method calculates new status and return it as
**         parameter (variable crc).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * crc             - Pointer to crc
**       * data            - Pointer to data
**         len             - Length
**     Returns     : Nothing
** ===================================================================
*/
void CR161_Update(word *crc,byte *data,word len)
{
/*
 #####  Here will be code here if you have a full registered version  #####
*/
 
}

/*
** ===================================================================
**     Method      :  CR161_Init (bean CRC16)
**
**     Description :
**         Initialize CRC (Sets new value to status variable).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * crc             - Pointer to crc
**     Returns     : Nothing
** ===================================================================
*/
void CR161_Init(word *crc)
{
/*
 #####  Here will be code here if you have a full registered version  #####
*/
 
}


/* END CR161. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.23 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
