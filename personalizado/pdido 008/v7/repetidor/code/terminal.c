/*
** ###################################################################
**
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**
**     Filename  : Terminal.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Beantype  : AsynchroSerial
**
**     Version   : Bean 02.216, Driver 01.43.01, CPU db: 2.89.000
**
**     Compiler  : Metrowerks HC08 C Compiler
**
**     Date/Time : 05/01/2007, 11:43
**
**     Abstract  :
**
**         This bean "AsynchroSerial" implements an asynchronous serial
**         communication. The bean supports different settings of 
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The bean requires one on-chip asynchronous serial channel.
**
**     Settings  :
**
**         Serial channel              : SCI
**
**         Protocol
**             Init baud rate          : 9600baud
**             Width                   : 8 bits
**             Stop bits               : 1
**             Parity                  : none
**             Breaks                  : Disabled
**
**         Registers
**             Input buffer            : SCDR      [24]
**             Output buffer           : SCDR      [24]
**             Control register        : SCC1      [19]
**             Mode register           : SCC2      [20]
**             Baud setting reg.       : SCBR      [25]
**             Special register        : SCS1      [22]
**
**
**
**         Used pins                   : 
**             ----------------------------------------------------
**               Function | On package |    Name
**             ----------------------------------------------------
**                Output  |     9      |  PTE0_TxD
**             ----------------------------------------------------
**
**
**
**     Contents  :
**
**         SendChar        - byte Terminal_SendChar(Terminal_TComData Chr);
**         GetCharsInTxBuf - word Terminal_GetCharsInTxBuf(void);
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


/* MODULE Terminal. */

#pragma MESSAGE DISABLE C4002 /* WARNING C4002: Result not used is ignored */

#include "Terminal.h"
#include "IRQ.h"
#include "TIMER.h"
#include "APERTURA.h"


#define OVERRUN_ERR      1             /* Overrun error flag bit   */
#define FRAMING_ERR      2             /* Framing error flag bit   */
#define PARITY_ERR       4             /* Parity error flag bit    */
#define CHAR_IN_RX       8             /* Char is in RX buffer     */
#define FULL_TX          16            /* Full transmit buffer     */
#define RUNINT_FROM_TX   32            /* Interrupt is in progress */
#define FULL_RX          64            /* Full receive buffer      */
#define NOISE_ERR        128           /* Noise erorr flag bit     */
#define IDLE_ERR         256           /* Idle character flag bit  */
#define BREAK_ERR        512           /* Break detect             */

static word SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Framing error */
                                       /*       2 - Parity error */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Unused */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Noise error */
                                       /*       8 - Idle character  */
                                       /*       9 - Break detected  */
                                       /*      10 - Unused */


/*
** ===================================================================
**     Method      :  HWEnDi (bean AsynchroSerial)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
static void HWEnDi(void)
{
    SCC1_ENSCI = 1;                    /* Enable device */
    SCC2_TE = 1;                       /* Enable transmitter */
    SCC2_RE = 0;                       /* Disable receiver */
}


/*
** ===================================================================
**     Method      :  Terminal_SendChar (bean AsynchroSerial)
**
**     Description :
**         Send one character to the channel. This method is
**         available only if the transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte Terminal_SendChar(Terminal_TComData Chr)
{
  if(!SCS1_SCTE)                       /* Is the transmitter empty? */
    return ERR_TXFULL;                 /* If no then error */
  SCDR = (byte)Chr;
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Terminal_GetCharsInTxBuf (bean AsynchroSerial)
**
**     Description :
**         Return number of characters in the output buffer. This
**         method is available only if the transmitter property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the output
**                           buffer.
** ===================================================================
*/
word Terminal_GetCharsInTxBuf(void)
{
  return (!SCS1_SCTE);                 /* Return number of chars in the transmitter buffer */
}

/*
** ===================================================================
**     Method      :  Terminal_Init (bean AsynchroSerial)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void Terminal_Init(void)
{
  SerFlag = 0;                         /* Reset flags */
  SCC1 = 0;
  /* SCC3: R8=0,T8=0,??=0,??=0,ORIE=0,NEIE=0,FEIE=0,PEIE=0 */
  SCC3 = 0;                            /* Disable error interrupts */
  SCBR_SCP = 0;                        /* Set prescaler bits */
  SCBR_SCR = 2;                        /* Set divisor bits */
  HWEnDi();                            /* Enable/disable device according to status flags */
}


/* END Terminal. */


/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.23 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
