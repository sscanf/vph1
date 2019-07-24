/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : SPI.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : SWSPI
**
**     Version   : Bean 01.002, Driver 01.01, CPU db: 2.87.041
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/28/lunes, 15:28
**
**     Abstract  :
**
**          This bean implements an external SPI communication interface specialy 
**          designed for SPI compatible beans. The interface uses three general-purpose 
**          I/O pins, first for the Input line, second for the Output line, third for 
**          the Serial Clock line. Is supported only Master mode with MSB first 
**          communication. 
**
**     Settings  :
**
**     Contents  :
**
**         RecvChar              - byte SPI_RecvChar(byte *Chr);
**         SendChar              - byte SPI_SendChar(byte Chr);
**         CharsInRxBuf          - byte SPI_CharsInRxBuf(word *Chr);
**         CharsInTxBuf          - byte SPI_CharsInTxBuf(word *Chr);
**         SetShiftClockPolarity - byte SPI_SetShiftClockPolarity(byte Edge);
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


/* MODULE SPI. */

#include "SPI.h"

#define OVERRUN_ERR  1                 /* Overrun error flag bit   */
#define CHAR_IN_RX   8                 /* Char is in RX buffer     */

static byte CLKshift;
static byte CLKsampl;
static byte InputBuffer;
static byte SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Unused */
                                       /*       2 - Unused */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Unused */
                                       /*       5 - Unused */
                                       /*       6 - Unused */
                                       /*       7 - Unused */

/*
** ===================================================================
**     Method      :  SPI_RecvChar (bean SWSPI)
**
**     Description :
**         If any data received, this method returns one character,
**         otherwise it returns error code (it does not wait for
**         data).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to received character.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_OVERRUN - Overrun error is detected
** ===================================================================
*/
byte SPI_RecvChar(byte *Chr)
{
  if(!(SerFlag & CHAR_IN_RX))          /* Is char. received */
    return ERR_RXEMPTY;
  *Chr = InputBuffer;                  /* Store the character */
  if(SerFlag & OVERRUN_ERR) {          /* Is "overrun" occured? */
    SerFlag &= ~(OVERRUN_ERR|CHAR_IN_RX); /* If yes, clear flags */
    return ERR_OVERRUN;                /* ... and return error */
  } 
  else {
    SerFlag &= ~CHAR_IN_RX;            /* If no, clear flag */
    return ERR_OK;                     /* ... and return */
  }
}

/*
** ===================================================================
**     Method      :  SPI_SendChar (bean SWSPI)
**
**     Description :
**         Send one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/
byte SPI_SendChar(byte Chr)
{
  byte i;                                                        
  for(i=0; i<8; i++) {
    Inhr3_PutVal(CLKshift);            /* Set CLK to shift value */
    Inhr2_PutVal(Chr > 127);           /* Set value on MOSI */
    InputBuffer <<= 1;
    Inhr3_PutVal(CLKsampl);            /* Set CLK to sample value */
    InputBuffer += Inhr1_GetVal()?1:0; /* Read value from MISO */
    Chr <<= 1;
  }
  if(SerFlag & CHAR_IN_RX)             /* Is char. received? */
    SerFlag |= OVERRUN_ERR;            /* If yes then set "overrun" flag */
  else
    SerFlag |= CHAR_IN_RX;             /* If no then set "char in Rx" flag */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SPI_CharsInRxBuf (bean SWSPI)
**
**     Description :
**         Return number of characters in the input buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to number of characters in the
**                           input buffer.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/
byte SPI_CharsInRxBuf(word *Chr)
{
  *Chr = (SerFlag >> 3) & 1;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SPI_CharsInTxBuf (bean SWSPI)
**
**     Description :
**         Return number of characters in the output buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to number of characters in the
**                           output buffer.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/
byte SPI_CharsInTxBuf(word *Chr)
{
  *Chr = 0;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SPI_SetShiftClockPolarity (bean SWSPI)
**
**     Description :
**         Set the shift clock polarity in runtime. Output data will
**         be sampled on the selected edge polarity.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Edge            - Edge polarity, possible values:
**                           0-falling edge,
**                           1-rising edge.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/
byte SPI_SetShiftClockPolarity(byte Edge)
{
  CLKsampl = Edge?0:1;                 /* Set sample value */
  CLKshift = CLKsampl ^ 1;             /* Set shift value */
  Inhr3_PutVal(CLKsampl);              /* Set CLK to idle (sample) value */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SPI_Init (bean SWSPI)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void SPI_Init(void)
{
  CLKsampl = 0;                        /* Set sample value to 0 */
  CLKshift = 1;                        /* Set shift value to 1 */
  Inhr3_PutVal(CLKsampl);              /* Set CLK to idle (sample) value */
  SerFlag = 0;                         /* Clear flags */
}


/* END SPI. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/

