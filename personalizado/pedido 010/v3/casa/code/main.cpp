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
#include "io_map.h"

#include "fx828.h"

#define CTCSS_SQL	12


#define APERTURA	PTC_PTC4
#define COLGADO		PTA_PTA6
#define PTT_ON		PTC_PTC2 = 0
#define PTT_OFF		PTC_PTC2 = 1
#define BEEP_ON		PTB_PTB7 = 1
#define BEEP_OFF	PTB_PTB7 = 0

#define SQL_ABIERTO	PTD_PTD4==0
#define SQL_CERRADO PTD_PTD4==1

#define MODE_STANDBY		0
#define MODE_COMUNICATION	1

extern "C" byte Terminal_SendChar(byte Chr);

//unsigned int const_indicativo[1] @0xf700;

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


bool SelCallDetected;
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
BOOL bMode;

void EnableTX (bool enable)
{
	FX828.Mod1Enable (enable);
//	FX828.Mod2Enable (~enable);
	FX828.CTCSS_tx_enable (enable);	//Fin transmisión
	PTC_PTC2 = ~enable;
}

//TIMBRE
void GeneraTimbre (void)
{
	int n,i;
	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	SelCallDetected = FALSE;
	for (n=0;n<5 && COLGADO;n++)
	{
		BEEP_ON;
		for (i=0;i<500 && COLGADO;i++)
			delay_ms(1);
		BEEP_OFF;
		for (i=0;i<200 && COLGADO;i++)
			delay_ms(1);
	}
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);
}

void ErrorIndicativo(void)
{
	int n;
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	FX828.ZVEI_tx_enable (TRUE);

	while (TRUE)	//Mientras esté descolgado
	{
		for (n=100;n<300;n++)
			FX828.ProgramZVEI(n);	
	}
}

//TONOS INDICACIÓN DESCONEXIÓN
void UnlinkSignal(void)
{
	FX828.ProgramZVEI(400.0);	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);

	while (!COLGADO)	//Mientras esté descolgado
	{
		FX828.ZVEI_tx_enable (TRUE);
		delay_ms(100);
		FX828.ZVEI_tx_enable (FALSE);
		delay_ms(100);
	}

}

//TONOS INDICACIÓN LÍNEA OCUPADA
void BussySignal(void)
{
	FX828.ProgramZVEI(400.0);	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);

	while (!COLGADO)	//Mientras esté descolgado
	{
		FX828.ZVEI_tx_enable (TRUE);
		delay_ms(500);
		FX828.ZVEI_tx_enable (FALSE);
		delay_ms(500);
	}
}

//Antes de abrir la puerta, nos aseguramos de los rebotes del pulsador
//Si se mantienen altos fijos durante 100ms enviaremos el subtono de apertura
//y devolveremos un TRUE.
//En caso contrario devolvemos un FALSE sin ninguna acción.

bool apertura(void)
{
	char tmp=~APERTURA;
	char n;

	//Bucle antirebotes	
	for (n=0;n<10;n++)
	{
		delay_ms(1);
		if (APERTURA==tmp)
		{
			n=0;
			tmp=~APERTURA;
		}
	}

	if (APERTURA)	//¿Ha estado la línea alta fija durante 10ms?
	{
		EnableTX (TRUE);
		FX828.ProgramFastCTCSS (123); //Transmitimos subtono de apertura
		delay_ms (3000);
		while (APERTURA);			 //Mientras están pulsando el botón
		FX828.ProgramFastCTCSS (100); //Reestablecemos subtono
		return TRUE;
	}
	return FALSE;
}
void comunicando (void)
{
	int n,i;
 	FX828.PutToneTable (0,RxCTCSSTable[CTCSS_SQL]);		//Tono de SQL
	for (n=1;n<15;n++)
		FX828.PutToneTable (n,0);

	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	FX828.EnableToneDecoder (TRUE,CTCSS_MODE);			//Ponemos el receptor de tonos en modo CTCSS
	bMode = MODE_COMUNICATION;

	// Tiene que haber subtono durante 1 segundo.
	for (n=0;n<500 && !bSQL;n++) delay_ms(1); 
	
	if (!bSQL)	//Si hay subtono es que la línea está ocupada
	{
		even_timeout=120000;
		PTT_ON;
		FX828.ProgramZVEI(400.0);		//Tono de espera comunicación
		FX828.CTCSS_tx_enable (TRUE);
		FX828.ZVEI_tx_enable (TRUE);
		FX828.Mod1Enable (TRUE);
	
		delay_ms(100);
		do
		{
			if (bSQL)
			{
				// Tiene que haber subtono durante 1 segundo.
				for (n=0;n<1000 && bSQL;n++) delay_ms(1); 
				if (n>999)
					break;
			}
		}while (!COLGADO && even_timeout); //Mientras no cuelgan y no pasan 2 minutos
		
		if (even_timeout)
		{	
			FX828.ZVEI_tx_enable (FALSE);
			FX828.EnableBPF (TRUE);    	//Habilitamos salida de audio rx
			
			//Mientras recibamos subtono, no cuelguen y no salte el time out
			while (!COLGADO && even_timeout)
			{
				if (!bSQL)	//Perdemos el subtono de recepción
				{
					for (n=0;n<1000 && !bSQL;n++) delay_ms(1);
					if (n>=999)
					{	
						even_timeout=0;
						break;
					}
				}

				if (APERTURA)	//Si hay subtono de RX,está descolgado y pulsan apertura.
				{
					if (apertura());
						even_timeout = 120000;		  //Empezamos la cuenta del time out					
				}
			}
		}
	
	
		EnableTX (FALSE);
		FX828.EnableBPF (FALSE);    	//Habilitamos salida de audio rx				

		if (!even_timeout)
			UnlinkSignal();	
	}
	else
		BussySignal();

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);

	FX828.EnableToneDecoder (FALSE,CTCSS_MODE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);
	bMode = MODE_STANDBY;
}

void maincpp (void)
{
	int n;
	unsigned int indicativo;
	
	DDRA_DDRA6 = FALSE;
	DDRB_DDRB7 = TRUE;
	DDRC_DDRC4 = FALSE;
	DDRC_DDRC2 = TRUE;
	
	//Pins para indicativo
	DDRD_DDRD4 = FALSE;
	DDRD_DDRD5 = FALSE;
	DDRD_DDRD6 = FALSE;
	
	SelCallDetected = FALSE;
	BEEP_OFF;
	bSQL=FALSE;


	indicativo = (PTD&0x70)>>4;
	
	while (Terminal_SendChar ((UCHAR)HIBYTE (indicativo))==ERR_TXFULL);
	while (Terminal_SendChar ((UCHAR)LOBYTE (indicativo))==ERR_TXFULL);

	bMode=MODE_STANDBY;
	EnableInterrupts;
	FX828.Init ();

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);
		
	FX828.SetDecoderBandwidth (0x0f);				//Ponemos el ancho de banda a 2.7
	FX828.ProgramFastCTCSS (100);					//Esperamos subtonos de 100 Hz
	FX828.Mod1Enable (FALSE);	//Deshabilitamos mod1		
	FX828.Mod2Enable (TRUE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);

	if (indicativo==0xffff)
		ErrorIndicativo();


	for(;;)
	{
		if (!COLGADO) 	//El usuario descuelga sin que llamen.
		{
			delay_ms (100);
			if (!COLGADO)
				comunicando();				//Realiza la comunicación
		}
		
		//Pulsan apertura sin haber descolgado y sin que hayan llamado.
		//Pero no se recibe ningún subtono
		if (APERTURA)
		{
			if (apertura());
				EnableTX (FALSE);				
		}
		
		if (SelCallDetected)
		{	
			if (!COLGADO)
				BussySignal();
				
			if (SelCallDetected && IndicRx.valor == indicativo)	//Hemos recibido una llamada selectiva, genera timbre
					GeneraTimbre();
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
	
	if (bMode == MODE_STANDBY)
	{
		if (tono!=0xff && tono!=UNKNOWN_TONE)	//Hemos detectado un tono conocido
		{		
			if (bStart==TRUE)
			{
				buffer[idx++]=tono;	
				if (idx==4)
				{
					IndicRx._byte.low  = MAKEBYTE (buffer[0],buffer[1]);
					IndicRx._byte.high = MAKEBYTE (buffer[2],buffer[3]);
					SelCallDetected = TRUE;	
					idx =0;
					bStart=FALSE;

					while (Terminal_SendChar ((UCHAR)IndicRx._byte.high)==ERR_TXFULL);
					while (Terminal_SendChar ((UCHAR)IndicRx._byte.low)==ERR_TXFULL);				
				}		
			}

			if (tono==0xb)	//Inicio de trama
			{
				idx=0;
				bStart=TRUE;
			}		
		}	
	}
	
	if (bMode == MODE_COMUNICATION)
	{
		switch (tono)
		{
			case 0:
				while (Terminal_SendChar ((UCHAR)'1')==ERR_TXFULL);
				bSQL = TRUE;
			break;
			
			default:
				while (Terminal_SendChar ((UCHAR)'0')==ERR_TXFULL);
				bSQL = FALSE;
			break;
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
