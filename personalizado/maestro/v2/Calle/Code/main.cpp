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

#include "fx828.h"

#define CTCSS_APERTURA	18
#define CTCSS_SQL		12

#define APERTURA_PUERTA	PTC_PTC4

#define PTT_ON			PTC_PTC2 = 0;delay_ms(100)
#define PTT_OFF			PTC_PTC2 = 1

#define AUDIO_ENABLE	PTC_PTC5 = 0
#define AUDIO_DISABLE	PTC_PTC5 = 1

#define SQL_ABIERTO	PTD_PTD4==0
#define SQL_CERRADO PTD_PTD4==1

extern "C" byte Terminal_SendChar(byte Chr);
extern "C" byte TIMER_Enable(void);
extern "C" byte TIMER_Disable(void);

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

const int indicativo = 1;

bool bFlag;
bool bAperPuerta;
bool bSQL;

union 		//Indicativo recibido
{
	struct
	{
		char low	:8;
		char high	:8;
	}_byte;
	int valor;
	
}IndicRx;

void PutString (char *string)
{
	int n;
	for (n=0;string[n]!=0;n++)
		while (Terminal_SendChar (string[n])==ERR_TXFULL);
}

CFX828 FX828;

void desconexion(void)
{
	AUDIO_DISABLE;
	FX828.CTCSS_tx_enable (FALSE);		//Fin de transmisión subtono
	FX828.EnableBPF (FALSE);			//Deshabilitamos salida de audio rx
	PTT_OFF;
}

signed char teclado (void)
{
	if (PTA<0x7f)	//Se detecta una pulsación
	{
		delay_ms (100);
		if (PTA<0x7f)
			return (~PTA)&0x7f;
	}
	return -1;	//No hay ninguna tecla pulsada
}
void maincpp (void)
{	
	int n;
	bool bFlag;
	signed char tecla;
	char str[5];
	
	DDRA = 0;		//Porta como entradas (Teclado)
	DDRB = 0;

	DDRC_DDRC2=TRUE;
	DDRC_DDRC3=TRUE;
	DDRC_DDRC4=TRUE;
	DDRC_DDRC5=TRUE;
	DDRD_DDRD4=FALSE;

	APERTURA_PUERTA = FALSE;

	bFlag=FALSE;
	bAperPuerta = FALSE;
	bSQL = FALSE;
	
	EnableInterrupts;
	FX828.Init ();

	
	FX828.Mod1Enable (TRUE,0x1f);	//Habilitamos mod1 con volumen al máximo
	FX828.Mod2Enable (FALSE,0);		//Deshabilitamos mod2
	
	//Grabamos los tonos CTCSS en la tabla  RX TONE PROGRAM TABLE
	FX828.PutToneTable (0,RxCTCSSTable[CTCSS_SQL]);		//Tono de SQL
	FX828.PutToneTable (1,RxCTCSSTable[CTCSS_APERTURA]);		//Tono de apertura de puerta

	FX828.SetDecoderBandwidth (0x08);			//Ponemos el ancho de banda a 1.1%
	FX828.EnableToneDecoder (TRUE,CTCSS_MODE);	//Habilitamos el decodificador de tonos
	FX828.ProgramFastCTCSS (100);				//Subtonos de 100 Hz para recibir y transmitir
	
	TIMER_Enable();

	desconexion();

	while (TRUE)
	{
		tecla = teclado();
		if (tecla>-1 && !bFlag)	//Pulsan una tecla del teclado
		{		
			PTT_ON;
			//Componemos el código ZVEI
			sprintf (str,"%04d",tecla);
						
			//Transmitimos:
			//
			// --- --- --- --- ---
			//| B | C | C | C | C |
			// --- --- --- --- ---
			//
			// B = Inicio de trama (B)
			// C = Indicativo (3 dígitos)
			
			delay_ms(500);
			FX828.CTCSS_tx_enable (TRUE);
			delay_ms(1000);		//Damos tiempo a que el decodificador detecte el subtono.
			DisableInterrupts;
			FX828.TxZVEI (0x0b);
			FX828.TxZVEI (str);
			EnableInterrupts;
			delay_ms(500);
			desconexion();
			while (teclado()>-1);
		}
	
		if (bAperPuerta==TRUE)
		{
			for (n=0;n<50 && bAperPuerta;n++) delay_ms(1);
			if (n>=49)
			{
				APERTURA_PUERTA=TRUE;
				delay_ms(100);
				while (bAperPuerta)	//Mientras esté el subotono de apertura
				{
					if (!bAperPuerta)	//Nos aseguramos de que realmente ya no hay subtono
						for (int i=0;i<500 && !bAperPuerta;i++) delay_ms(1);
				}
				APERTURA_PUERTA=FALSE;
			}
		}
		
		if (bSQL && !bFlag)	//Nos llaman
		{
			for (n=0;n<50 && bSQL;n++) delay_ms(1);
			if (n>=49)
			{
				PTT_ON;
				AUDIO_ENABLE;
				FX828.CTCSS_tx_enable (TRUE);//Transmitimos subtono
				FX828.EnableBPF (TRUE);		 //Habilitamos salida de audio rx
				bFlag = TRUE;
			}
		}
		
		if (!bSQL && bFlag)	//Perdemos subtono
		{
			AUDIO_DISABLE;
			if (bFlag == TRUE)
			{
				for (n=0;n<500 && !bSQL;n++) delay_ms(1);
				if (n>=499)
				{
					desconexion();
					bFlag = FALSE;				
				}
				else	
					AUDIO_ENABLE;
			}
		}
	}
}

#ifdef __cplusplu
	extern "C"
#endif

void MainIRQ ()
{
	byte tono;
	FX828.IRQService();
	tono = FX828.GetTone();

	switch (tono)
	{
		case 0:
			bSQL = TRUE;
			bAperPuerta=FALSE;
		break;
		
		case 1:
			bAperPuerta = TRUE;
			bSQL=FALSE;
		break;

		case 0xFF:
		case UNKNOWN_TONE:
			bAperPuerta = FALSE;
			bSQL = FALSE;
		break;

		default:
			bAperPuerta = FALSE;
			bSQL = FALSE;
		break;
	
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
