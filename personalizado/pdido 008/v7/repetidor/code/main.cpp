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

// PARA QUE SE LINKE CORRECTAMENTE ASEGURARSE DE QUE LA SIGUIENTE LINEA EXISTE EN EL ARCHIVO VPH1.PRM
//     HEAP_SEGMENT					 INTO  HEAP;

//#pragma MESSAGE DISABLE C1420 /* WARNING C1420: Result of function-call is ignored. You can uncomment this line to disable this compiler warning */

/*Including used modules for compilling procedure*/
#include "vph1.h"
#include "Cpu.h"
#include "Events.h"
#include "TIMER.h"
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
#include "io_map.h"
#include "APERTURA.h"

#include "fx828.h"

#define CTCSS_ACCESO1	0

#define COLGADO		PTA_PTA6
#define PTT_ON		PTC_PTC2 = 0
#define PTT_OFF		PTC_PTC2 = 1
#define BEEP_ON		PTB_PTB7 = 1
#define BEEP_OFF	PTB_PTB7 = 0

#define AUDIO_ENABLE	PTC_PTC5 = 0
#define AUDIO_DISABLE	PTC_PTC5 = 1

#define LED_PUERTA1	PTB_PTB6

//#define MAKEBYTE(b, a)  ((((byte)(a)) | (((byte)(b))) << 4))
#define HEX2BIN(a) (((byte)(0x80)) >> (byte)(a))


extern "C" byte Terminal_SendChar(byte Chr);

const int RxZVEI1Table[15]={0xc44,	/*2400 0*/
							0x553,	/*1060 1*/
							0x5d2,	/*1160 2*/
							0x683,	/*1270 3*/
							0x70e,	/*1400 4*/
							0x7c8,	/*1530 5*/
							0x886,	/*1670 6*/
							0x949,	/*1830 7*/
							0xa42,	/*2000 8*/
							0xb43,	/*2200 9*/
							0xe46,	/*2800 A*/
							0x414,	/*810  B*/
							0x4d8,	/*970  C*/
							0x486,	/*885  D*/
							0xd45};	/*2600 E*/


bool SelCallDetected;
bool bSQL;
int SubtonoRX;
float SubtonoTX;

char str[50];

union 		//Indicativo recibido
{
	struct
	{
		char low	:8;
		char high	:8;
	}_byte;
	unsigned int valor;
	
}TeclaPulsada;


unsigned char IndicRx;

void PutString (char *string)
{
	int n;
	for (n=0;string[n]!=0;n++)
		while (Terminal_SendChar (string[n])==ERR_TXFULL);
}

CFX828 FX828;
void EnableTX (bool enable)
{
	FX828.Mod1Enable (enable);
//	FX828.Mod2Enable (~enable);
	FX828.CTCSS_tx_enable (enable);	//Fin transmisión
	PTC_PTC2 = ~enable;
}

void maincpp (void)
{
	int n;
	bool bSQL;
	
	DDRC_DDRC4 = FALSE;
	DDRA_DDRA6 = FALSE;
	DDRB = 0xff;		//PORTB TODO SALIDAS
	
	DDRC_DDRC2 = TRUE;
	DDRD_DDRD4 = FALSE;
	PTB = 00;
	
	SelCallDetected = FALSE;
	BEEP_OFF;
	PTT_OFF;
	bSQL=FALSE;

	EnableInterrupts;
	FX828.Init ();

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);
		
	FX828.SetDecoderBandwidth (0x0f);				//Ponemos el ancho de banda a 2.7
	FX828.Mod1Enable (FALSE);	//Deshabilitamos mod1		
	FX828.Mod2Enable (TRUE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);

	for(;;)
	{
		if (SelCallDetected)
		{
			SelCallDetected=FALSE;
			
	    if (IndicRx==0)
	    {
	    	if (TeclaPulsada.valor == 3)
	    		PTT_ON;
	    		
	    	if (TeclaPulsada.valor == 4)
	    		PTT_OFF;
	    }
	    else
	    {
				PTT_ON;
				AUDIO_DISABLE;
				delay_ms(1000);
				DisableInterrupts;
				sprintf (str,"%01d%03d",IndicRx,TeclaPulsada.valor);
				FX828.TxZVEI (0x0b);
				FX828.TxZVEI (str);
				EnableInterrupts;
				delay_ms(510);			
				FX828.EnableBPF (FALSE);			//Deshabilitamos salida de audio rx
				PTT_OFF;
			}	    	
    }
	}
}

#ifdef __cplusplu
	extern "C"
#endif
void MainIRQ ()
{
	static byte buffer[4];
	static bool bStart;
	static byte idx;
	byte tono;
	
	FX828.IRQService();

	tono = FX828.GetTone();
	
	if (tono!=0xff && tono!=UNKNOWN_TONE)	//Hemos detectado un tono conocido
	{		
		sprintf (str,"%x",tono);
		PutString (str);
	
		if (bStart==TRUE || PTB_PTB7)
		{
			buffer[idx++]=tono;	
			if (idx==4 || PTB_PTB7)
			{
				IndicRx = buffer[0];
				TeclaPulsada._byte.low  = buffer[1];
				TeclaPulsada._byte.high = MAKEBYTE (buffer[2],buffer[3]);
				SelCallDetected = TRUE;	
				idx =0;
				bStart=FALSE;

				while (Terminal_SendChar ((UCHAR)TeclaPulsada._byte.high)==ERR_TXFULL);
				while (Terminal_SendChar ((UCHAR)TeclaPulsada._byte.low)==ERR_TXFULL);				
		    PutString ("\n\r");
				SelCallDetected = TRUE;		    
			}		
		}

		if (tono==0xb)	//Inicio de trama
		{
			idx=0;
			bStart=TRUE;
		  PutString ("\n\r");
		}		
	}
	else
	{
		if (bStart)
			PutString ("Tono desconocido. Codigo descartado\n\r");

		idx=0;
		bStart=FALSE;
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
