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

#define CTCSS_SQL		12

#define APERTURA_PUERTA1	PTC_PTC4
#define APERTURA_PUERTA2	PTB_PTB6
#define APERTURA_PUERTA3	PTB_PTB5
#define APERTURA_PUERTA4	PTB_PTB4

#define PTT_ON			PTC_PTC2 = 0;delay_ms(100)
#define PTT_OFF			PTC_PTC2 = 1

#define PTT_IN		  PTA_PTA6

#define AUDIO_ENABLE	PTC_PTC5 = 0
#define AUDIO_DISABLE	PTC_PTC5 = 1

#define INDICATIVO	(((~PTD)&0x7f)>>4)+1

extern "C" byte Terminal_SendChar(byte Chr);
extern "C" byte TIMER_Enable(void);
extern "C" byte TIMER_Disable(void);


const float FastCTCSSTable[47]=
{
		67.0,  /* Acceso 1 SUBTONO COMUNICACION*/ 	
		69.3,  /* Acceso 1 Abrir puerta 1*/ 
		71.9,  /* Acceso 1 Abrir puerta 2*/
		74.4,  /* Acceso 1 Abrir puerta 3*/ 
		77.0,  /* Acceso 1 Abrir puerta 4*/ 

		79.7,  /* Acceso 2 SUBTONO COMUNICACION*/ 	
		82.5,  /* Acceso 2 Abrir puerta 1*/ 
		85.4,  /* Acceso 2 Abrir puerta 2*/ 
		88.5,  /* Acceso 2 Abrir puerta 3*/ 
		91.5,  /* Acceso 2 Abrir puerta 4*/   

		94.8,  /* Acceso 3 SUBTONO COMUNICACION*/ 	 
		97.4,  /* Acceso 3 Abrir puerta 1*/   
		100.0,  /* Acceso 3 Abrir puerta 2*/   
		103.5, /* Acceso 3 Abrir puerta 3*/   
		107.2, /* Acceso 3 Abrir puerta 4*/   

		110.9, /* Acceso 4 SUBTONO COMUNICACION*/ 	 
		114.8, /* Acceso 4 Abrir puerta 1*/   
		118.8, /* Acceso 4 Abrir puerta 2*/   
		123.0, /* Acceso 4 Abrir puerta 3*/   
		127.3, /* Acceso 4 Abrir puerta 4*/   

		131.8, /* Acceso 5 SUBTONO COMUNICACION*/ 	 
		136.5, /* Acceso 5 Abrir puerta 1*/   
		141.3, /* Acceso 5 Abrir puerta 2*/   
		146.2, /* Acceso 5 Abrir puerta 3*/   
		151.4, /* Acceso 5 Abrir puerta 4*/   

		156.7, /* Acceso 6 SUBTONO COMUNICACION*/ 	 
		159.8, /* Acceso 6 Abrir puerta 1*/   
		162.2, /* Acceso 6 Abrir puerta 2*/   
		167.9, /* Acceso 6 Abrir puerta 3*/   
		173.8, /* Acceso 6 Abrir puerta 4*/   

		179.9, /* Acceso 7 SUBTONO COMUNICACION*/ 	 
		183.5, /* Acceso 7 Abrir puerta 1*/   
		186.2, /* Acceso 7 Abrir puerta 2*/   
		189.9, /* Acceso 7 Abrir puerta 3*/   
		192.8, /* Acceso 7 Abrir puerta 4*/   

		196.6, /* Acceso 8 SUBTONO COMUNICACION*/ 	 
		199.5, /* Acceso 8 Abrir puerta 1*/   
		203.5, /* Acceso 8 Abrir puerta 2*/   
		206.5, /* Acceso 8 Abrir puerta 3*/   
		210.7, /* Acceso 8 Abrir puerta 4*/   

		218.1, /* Acceso 9 SUBTONO COMUNICACION*/ 	 
		225.7, /* Acceso 9 Abrir puerta 1*/   
		229.1, /* Acceso 9 Abrir puerta 2*/   
		233.6, /* Acceso 9 Abrir puerta 3*/   
		241.8, /* Acceso 9 Abrir puerta 4*/   
		250.3,
		254.1};


const int RxCTCSSTable[47]={
					  	0x3d8,	/*67.0  Acceso 1 SUBTONO COMUNICACION*/ 	
					  	0x409,	/*69.3  Acceso 1 Abrir puerta 1*/ 
					  	0x41b,	/*71.9  Acceso 1 Abrir puerta 2*/
							0x44e,	/*74.4  Acceso 1 Abrir puerta 3*/ 
							0x483,	/*77.0  Acceso 1 Abrir puerta 4*/ 
							
							0x494,	/*79.7  Acceso 2 SUBTONO COMUNICACION*/ 	
							0x4cb,	/*82.5  Acceso 2 Abrir puerta 1*/ 
							0x502,	/*85.4  Acceso 2 Abrir puerta 2*/ 
							0x514,	/*88.5  Acceso 2 Abrir puerta 3*/ 
							0x54c,	/*91.5  Acceso 2 Abrir puerta 4*/   
							
							0x587,	/*94.8  Acceso 3 SUBTONO COMUNICACION*/ 	 
							0x594,	/*97.4  Acceso 3 Abrir puerta 1*/   
							0x5cb,	/*100.  Acceso 3 Abrir puerta 2*/   
							0x607,	/*103.5 Acceso 3 Abrir puerta 3*/   
							0x645,	/*107.2 Acceso 3 Abrir puerta 4*/   
							
							0x682,	/*110.9 Acceso 4 SUBTONO COMUNICACION*/ 	 
							0x6c0,	/*114.8 Acceso 4 Abrir puerta 1*/   
							0x6d1,	/*118.8 Acceso 4 Abrir puerta 2*/   
							0x710,	/*123.0 Acceso 4 Abrir puerta 3*/   
							0x750,	/*127.3 Acceso 4 Abrir puerta 4*/   
							
							0x7c0,	/*131.8 Acceso 5 SUBTONO COMUNICACION*/ 	 
							0x802,	/*136.5 Acceso 5 Abrir puerta 1*/   
							0x884,	/*141.3 Acceso 5 Abrir puerta 2*/   
							0x886,	/*146.2 Acceso 5 Abrir puerta 3*/   
							0x8c9,	/*151.4 Acceso 5 Abrir puerta 4*/   
							
							0x90c,	/*156.7 Acceso 6 SUBTONO COMUNICACION*/ 	 
							0x948,	/*159.8 Acceso 6 Abrir puerta 1*/   
							0x982,	/*162.2 Acceso 6 Abrir puerta 2*/   
							0x9c6,	/*167.9 Acceso 6 Abrir puerta 3*/   
							0xa0b,	/*173.8 Acceso 6 Abrir puerta 4*/   
							
							0xa84,	/*179.9 Acceso 7 SUBTONO COMUNICACION*/ 	 
							0xac2,	/*183.5 Acceso 7 Abrir puerta 1*/   
							0xac9,	/*186.2 Acceso 7 Abrir puerta 2*/   
							0xb08,	/*189.9 Acceso 7 Abrir puerta 3*/   
							0xb44,	/*192.8 Acceso 7 Abrir puerta 4*/   
							
							0xb83,	/*196.6 Acceso 8 SUBTONO COMUNICACION*/ 	 
							0xb8a,	/*199.5 Acceso 8 Abrir puerta 1*/   
							0xbc9,	/*203.5 Acceso 8 Abrir puerta 2*/   
							0xc06,	/*206.5 Acceso 8 Abrir puerta 3*/   
							0xc46,	/*210.7 Acceso 8 Abrir puerta 4*/   
							
							0xcc3,	/*218.1 Acceso 9 SUBTONO COMUNICACION*/ 	 
							0xd41,	/*225.7 Acceso 9 Abrir puerta 1*/   
							0xd48,	/*229.1 Acceso 9 Abrir puerta 2*/   
							0xd89,	/*233.6 Acceso 9 Abrir puerta 3*/   
							0xe08,	/*241.8 Acceso 9 Abrir puerta 4*/   
							0xe88,	/*250.3*/
							0xec7};/*254.1*/
								 
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


bool bFlag;
byte AperPuerta;
bool bSQL;

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
	char buffer[80];
	
	
	DDRA = 0;		//Porta como entradas (Teclado)
	DDRB = 0;

	DDRC_DDRC2=TRUE;
	DDRC_DDRC3=TRUE;
	DDRC_DDRC4=TRUE;
	DDRC_DDRC5=TRUE;
	DDRB_DDRB6=TRUE;
	DDRB_DDRB5=TRUE;
	DDRB_DDRB4=TRUE;
	DDRA_DDRA6=FALSE;

	// Codificacion indicativo
	DDRD_DDRD4=FALSE;
	DDRD_DDRD5=FALSE;
	DDRD_DDRD6=FALSE;
	
	APERTURA_PUERTA1 = FALSE;
	APERTURA_PUERTA2 = FALSE;
	APERTURA_PUERTA3 = FALSE;
	APERTURA_PUERTA4 = FALSE;

	bFlag=FALSE;
	AperPuerta = 0;
	bSQL = FALSE;

	EnableInterrupts;
	TIMER_Enable();
	delay_ms(1);

	PutString ("VPH1 Electronica Barcelona,S.L.\n\r");
	sprintf (buffer,"Indicativo = %d\n\r",INDICATIVO);
	PutString (buffer);
	
	FX828.Init ();
	
	FX828.Mod1Enable (TRUE,0x1f);	//Habilitamos mod1 con volumen al máximo
	FX828.Mod2Enable (FALSE,0);		//Deshabilitamos mod2

	desconexion();

	while (TRUE)
	{
		tecla = teclado();
		if (tecla>-1 && !bFlag)	//Pulsan una tecla del teclado
		{		
			//Grabamos los tonos CTCSS en la tabla  RX TONE PROGRAM TABLE
			FX828.PutToneTable (0,RxCTCSSTable[((INDICATIVO-1)*5)]);		//Tono de SQL
			FX828.PutToneTable (1,RxCTCSSTable[((INDICATIVO-1)*5)+1]);	//Tono de apertura de puerta 1
			FX828.PutToneTable (2,RxCTCSSTable[((INDICATIVO-1)*5)+2]);	//Tono de apertura de puerta 2
			FX828.PutToneTable (3,RxCTCSSTable[((INDICATIVO-1)*5)+3]);	//Tono de apertura de puerta 3
			FX828.PutToneTable (4,RxCTCSSTable[((INDICATIVO-1)*5)+4]);	//Tono de apertura de puerta 4

			FX828.SetDecoderBandwidth (0x08);			//Ponemos el ancho de banda a 1.1%
			FX828.EnableToneDecoder (TRUE,CTCSS_MODE);	//Habilitamos el decodificador de tonos
			FX828.ProgramFastCTCSS (FastCTCSSTable[((INDICATIVO-1)*5)]);				//Subtonos de 100 Hz para recibir y transmitir
			sprintf (buffer,"Indicativo = %d\n\r",INDICATIVO);
			PutString (buffer);
		
			//GENERAMOS UN TONO EN EL ALTAVOZ
			AUDIO_ENABLE;
			FX828.Mod1Enable (FALSE,0);	//Habilitamos mod1 con volumen al máximo
			FX828.Mod2Enable (TRUE,0x1f);
			FX828.ProgramZVEI (200.0);
			FX828.ZVEI_tx_enable(TRUE);
			while (teclado()>-1);
			FX828.ZVEI_tx_enable(FALSE);
			AUDIO_DISABLE;
			FX828.Mod1Enable (TRUE,0x1f);	//Habilitamos mod1 con volumen al máximo
			FX828.Mod2Enable (FALSE,0);		//Deshabilitamos mod2
			
			PTT_ON;
		
			//Componemos el código ZVEI
//			sprintf (str,"%04d",tecla);
						
			//Transmitimos:
			//
			// --- --- --- --- ---
			//| B | I | T | T | T |
			// --- --- --- --- ---
			//
			// B = Inicio de trama (B)
			// I = Indicativo (Número de puerta) 1 dígito
			// T = Tecla pulsada o pulsador 3 Dígitos
			
			sprintf (str,"%01d%03d",INDICATIVO,tecla);
			delay_ms(500);
			FX828.CTCSS_tx_enable (TRUE);
			DisableInterrupts;
			FX828.TxZVEI (0x0b);
			FX828.TxZVEI (str);
			EnableInterrupts;
			sprintf (str,"%04d",tecla);		
			delay_ms(510);
			DisableInterrupts;
			FX828.TxZVEI (0x0b);
			FX828.TxZVEI (str);
			EnableInterrupts;
			delay_ms(500);
			desconexion();
		
		
			while (teclado()>-1);
			
		}
	
		if (bFlag && AperPuerta)
		{
			switch (AperPuerta)
			{
				case 1:
					for (n=0;n<50 && AperPuerta;n++) delay_ms(1);
					if (n>=49)
					{
						APERTURA_PUERTA1=TRUE;
						delay_ms(100);
						while (AperPuerta==1)	//Mientras esté el subotono de apertura
						{
							if (AperPuerta!=1)	//Nos aseguramos de que realmente ya no hay subtono
								for (int i=0;i<500 && !AperPuerta;i++) delay_ms(1);
						}
						APERTURA_PUERTA1=FALSE;
					}
				break;
				
				case 2:
					for (n=0;n<50 && AperPuerta;n++) delay_ms(1);
					if (n>=49)
					{
						APERTURA_PUERTA2=TRUE;
						delay_ms(100);
						while (AperPuerta==2)	//Mientras esté el subotono de apertura
						{
							if (AperPuerta!=2)	//Nos aseguramos de que realmente ya no hay subtono
								for (int i=0;i<500 && !AperPuerta;i++) delay_ms(1);
						}
						APERTURA_PUERTA2=FALSE;
					}
				break;

				case 3:
					for (n=0;n<50 && AperPuerta;n++) delay_ms(1);
					if (n>=49)
					{
						APERTURA_PUERTA3=TRUE;
						delay_ms(100);
						while (AperPuerta==3)	//Mientras esté el subotono de apertura
						{
							if (AperPuerta!=3)	//Nos aseguramos de que realmente ya no hay subtono
								for (int i=0;i<500 && !AperPuerta;i++) delay_ms(1);
						}
						APERTURA_PUERTA3=FALSE;
					}
				break;
				
				case 4:
					for (n=0;n<50 && AperPuerta;n++) delay_ms(1);
					if (n>=49)
					{
						APERTURA_PUERTA4=TRUE;
						delay_ms(100);
						while (AperPuerta==4)	//Mientras esté el subotono de apertura
						{
							if (AperPuerta!=4)	//Nos aseguramos de que realmente ya no hay subtono
								for (int i=0;i<500 && !AperPuerta;i++) delay_ms(1);
						}
						APERTURA_PUERTA4=FALSE;
					}
				break;		
			}
		}
		
		if (bSQL && !bFlag)	//Nos llaman
		{
			for (n=0;n<100 && bSQL;n++) delay_ms(1);
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
			AperPuerta=0;
		break;	
		
		case 1:
			AperPuerta =1;
//			bSQL=TRUE;
		break;
		
		case 2:
			AperPuerta =2;
//			bSQL=TRUE;
		break;

		case 3:
			AperPuerta =3;
//			bSQL=TRUE;
		break;

		case 4:
			AperPuerta =4;
//			bSQL=TRUE;
		break;

		case 0xFF:
		case UNKNOWN_TONE:
			AperPuerta =0;
			bSQL = FALSE;
		break;

		default:
			AperPuerta = 0;
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
