/*
** ###################################################################
**
**     Filename  : vph1.C
**
**     Project   : vph1
** 
**     Processor : MC68HC908GP32CFB
**
**     Version   : Driver 01.00
**
**     Compiler  : Metrowerks HC08 C Compiler V-5.0.13
**
**     Date/Time : 4/17/jueves, 15:23
**
**     Abstract  :
**
**         Main module. 
**         Here is to be placed user's code.
**
**     Settings  :
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
/* MODULE vph1 */

//#pragma MESSAGE DISABLE C1420 /* WARNING C1420: Result of function-call is ignored. You can uncomment this line to disable this compiler warning */

/*Including used modules for compilling procedure*/
#include "vph1.h"
#include "Cpu.h"
#include "Events.h"
#include "TIMER.h"
#include "Teclado.h"
#include "IRQ.h"
/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include <stdio.h>
#include <ctype.h>
#include "delay.h"
#include "maincpp.h"

#include "fx828.h"

#define CTCSS_100	12
#define CTCSS_110	16

extern "C" byte Terminal_SendChar(byte Chr);

const int RxCTCSSTable[47]={0x3d8,	/*67.0*/ 0x409,	/*69.3*/ 0x41b,	/*71.9*/
							0x44e,	/*74.4*/ 0x483,	/*77.0*/ 0x494,	/*79.7*/
							0x4cb,	/*82.5*/ 0x502,	/*85.4*/ 0x514,	/*88.5*/
							0x54c,	/*91.5*/ 0x587,	/*94.8*/ 0x594,	/*97.4*/
							0x5cb,	/*100.0*/0x607,	/*103.5*/0x645,	/*107.2*/
							0x682,	/*110.9*/0x6c0,	/*114.8*/0x6d1,	/*118.8*/
							0x710,	/*123.0*/0x750,	/*127.3*/0x7c0,	/*131.8*/
							0x802,	/*136.5*/0x884,	/*141.3*/0x886,	/*146.2*/
							0x8c9,	/*151.4*/0x90c,	/*156.7*/0x948,	/*159.8*/
							0x982,	/*162.2*/0x9c6,	/*167.9*/0xa0b,	/*173.8*/
							0xa84,	/*179.9*/0xac2,	/*183.5*/0xac9,	/*186.2*/
							0xb08,	/*189.9*/0xb44,	/*192.8*/0xb83,	/*196.6*/
							0xb8a,	/*199.5*/0xbc9,	/*203.5*/0xc06,	/*206.5*/
							0xc46,	/*210.7*/0xcc3,	/*218.1*/0xd41,	/*225.7*/
							0xd48,	/*229.1*/0xd89,	/*233.6*/0xe08,	/*241.8*/
							0xe88,	/*250.3*/0xec7};/*254.1*/
								 
const int RxZVEI1Table[15]={0xc44,	/*2400 */0x553,	/*1060*/0x5d2,	/*1160*/
							0x683,	/*1270 */0x70e,	/*1400*/0x7c8,	/*1530*/
							0x886,	/*1670 */0x949,	/*1830*/0xa42,	/*2000*/
							0xb43,	/*2200 */0xe46,	/*2800*/0x414,	/*810*/
							0x4d8,	/*970  */0x486,	/*885 */0xd45};	/*2600*/

void PutString (char *string)
{
	int n;
	for (n=0;string[n]!=0;n++)
		while (Terminal_SendChar (string[n])==ERR_TXFULL);
}

CFX828 FX828;

void maincpp (void)
{	
	int n;
	EnableInterrupts;
	FX828.Init ();
	
	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)		
		FX828.PutToneTable (n,RxCTCSSTable[n]);

	FX828.SetDecoderBandwidth (0x08);	//Ponemos el ancho de banda a 1.1
	FX828.EnableToneDecoder (TRUE,CTCSS_MODE);	//Habilitamos el decodificador de tonos

	FX828.EnableCTCSSDecoder (TRUE, DETECT_MODE);
	
	FX828.TxCTCSS (100);			//Programamos un subtono de 100Hz
	FX828.CTCSS_tx_enable (TRUE);	//Activamos la transmisión
	
	FX828.TxZVEI ("12345");			//Enviamos 12345 vía ZVEI
	
	
	for(;;);
}
#ifdef __cplusplu
	extern "C"
#endif
void MainIRQ ()
{
	char str[10];
	signed char tono;
	static bool flag=0;
	
	FX828.IRQService();
	
	if ((tono = FX828.GetTone())>-1)
	{
		sprintf (str,"%d ",tono);
		PutString (str);
	}
	
	if (FX828.CTCSS_Detected())
	{
		if (!flag)
		{
			PutString ("\n\rCTCSS ON!\n\r");
			flag=TRUE;
		}
	}
	else
	{
		if (flag)
		{
			PutString ("\n\rCTCSS OFF!\n\r");
			flag=FALSE;
		}
	}
}

/* END vph1 */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 02.89 for 
**     the Motorola HC08 series of microcontrollers.
**
** ###################################################################
*/
