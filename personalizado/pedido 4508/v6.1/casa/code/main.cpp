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

#define TIEMPO_COMUNICACION		15000
#define LED_PUERTA1	PTB_PTB6

#define MODE_STANDBY		0
#define MODE_COMUNICATION	1

//#define MAKEBYTE(b, a)  ((((byte)(a)) | (((byte)(b))) << 4))
#define HEX2BIN(a) (((byte)(0x80)) >> (byte)(a))


extern "C" byte Terminal_SendChar(byte Chr);

unsigned int const_indicativo[1] @0xf700;
unsigned int indicativo;


const float TxCTCSSTable[]={
							  	67.0, /* Acceso 1 SUBTONO COMUNICACION*/ 	
							  	69.3, /* Acceso 1 Abrir puerta 1*/ 
							  	71.9, /* Acceso 1 Abrir puerta 2*/
									74.4, /* Acceso 1 Abrir puerta 3*/ 
									77.0, /* Acceso 1 Abrir puerta 4*/ 
									
									79.7, /* Acceso 2 SUBTONO COMUNICACION*/ 	
									82.5, /* Acceso 2 Abrir puerta 1*/ 
									85.4, /* Acceso 2 Abrir puerta 2*/ 
									88.5, /* Acceso 2 Abrir puerta 3*/ 
									91.5, /* Acceso 2 Abrir puerta 4*/   
									
									94.8,  /* Acceso 3 SUBTONO COMUNICACION*/ 	 
									97.4,  /* Acceso 3 Abrir puerta 1*/   
									100.0, /* Acceso 3 Abrir puerta 2*/   
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
									};


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
	FX828.Mod2Enable (TRUE);
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
	FX828.Mod2Enable (TRUE);
	FX828.ProgramZVEI(400.0);	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);

	while (!COLGADO)	//Mientras esté descolgado
	{
		FX828.ZVEI_tx_enable (TRUE);
		delay_ms(100);
		FX828.ZVEI_tx_enable (FALSE);
		delay_ms(100);
	}

//	FX828.Mod2Enable (FALSE);
}

//TONOS INDICACIÓN LÍNEA OCUPADA
void BussySignal(void)
{
	FX828.Mod2Enable (TRUE,0x1f);
	FX828.ProgramZVEI(400.0);	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);

	while (!COLGADO)	//Mientras esté descolgado
	{
		FX828.ZVEI_tx_enable (TRUE);
		delay_ms(500);
		FX828.ZVEI_tx_enable (FALSE);
		delay_ms(500);
	}

//	FX828.Mod2Enable (FALSE);

}

//Antes de abrir la puerta, nos aseguramos de los rebotes del pulsador
//Si se mantienen altos fijos durante 100ms enviaremos el subtono de apertura
//y devolveremos un TRUE.
//En caso contrario devolvemos un FALSE sin ninguna acción.

bool apertura(void)
{
	char tmp=APERTURA_GetVal();
	char n;
	char tono;

//	FX828.Mod2Enable (FALSE);
	//Bucle antirebotes	
	for (n=0;n<10;n++)
	{
		delay_ms(1);
		if (APERTURA_GetVal()!=tmp)
		{
			n=0;
			tmp=APERTURA_GetVal();
		}
	}

	if (APERTURA_GetVal())	//¿Ha estado la línea alta fija durante 10ms?
	{
	
		PutString ("Subtono apertura\n\r");
		EnableTX (TRUE);
	

		sprintf (str,"Subtono = %f \n\r",TxCTCSSTable [((IndicRx-1)*5)+1]);
		PutString (str);
		
		
		FX828.ProgramFastCTCSS (TxCTCSSTable [((IndicRx-1)*5)]); //Transmitimos subtono de voz
		delay_ms (150);
		
		FX828.ProgramFastCTCSS (TxCTCSSTable [((IndicRx-1)*5)+1]); //Transmitimos subtono de apertura
		delay_ms (1000);
		
		while (APERTURA_GetVal());	 //Mientras están pulsando el botón
		FX828.ProgramFastCTCSS (SubtonoTX); //Reestablecemos subtono
		FX828.Mod2Enable (TRUE);
		EnableTX (FALSE);
		return TRUE;
	}
	FX828.Mod2Enable (TRUE);
	EnableTX (FALSE);
	return FALSE;
}

void comunicando ()
{
	int n,i;
	
	//Conmutamos a recepción de subtonos
	for (n=1;n<15;n++)
		FX828.PutToneTable (n,0);

	PutString ("Entrando en comunicación \n\r");
	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	FX828.EnableToneDecoder (TRUE,CTCSS_MODE);			//Ponemos el receptor de tonos en modo CTCSS
	bMode = MODE_COMUNICATION;
	PutString ("En modo comunicación ... \n\r");
	FX828.ZVEI_tx_enable (FALSE);

	even_timeout=TIEMPO_COMUNICACION;	
	//Mientras no salte el time out	
	
	while (even_timeout)
	{
	
		if (!COLGADO)
		{
				PutString ("Pulsan PTT, genero subtono y pongo PTT \n\r");
				PTT_ON;
				FX828.CTCSS_tx_enable (TRUE);
				FX828.Mod1Enable (TRUE);	
		
				while (!COLGADO);
				
				PutString ("Sueltan PTT, quito subtono y PTT \n\r");
				PTT_OFF;
				FX828.CTCSS_tx_enable (FALSE);
				even_timeout = TIEMPO_COMUNICACION;		  //Empezamos la cuenta del time out					
		}
			
		if (bSQL)	//Perdemos el subtono de recepción
		{
				FX828.EnableBPF (TRUE);  //Habilitamos salida de audio rx
				PutString ("Recibido subtono, abro audio y reinicio tiempo \n\r");
			
				while (bSQL);

				FX828.EnableBPF (FALSE); //Deshabilitamos salida de audio rx
				PutString ("Perdemos subtono, cierro audio \n\r");
				even_timeout = TIEMPO_COMUNICACION;		  //Empezamos la cuenta del time out					
		}

		if (APERTURA_GetVal())	//Pulsan apertura.
		{
			PutString ("Pulsan apertura \n\r");
			if (apertura());
			{
				even_timeout = TIEMPO_COMUNICACION;		  //Empezamos la cuenta del time out					
			}
		}
	}

	EnableTX (FALSE);
	FX828.EnableBPF (FALSE);    	//Deshabilitamos salida de audio rx

	//Conmutamos a recepción de 5 tonos
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);

	FX828.EnableToneDecoder (FALSE,CTCSS_MODE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);
	bMode = MODE_STANDBY;
	PutString ("En modo standby ... \n\r");
	
//	if (!even_timeout)
//			UnlinkSignal();	
	
	PTB = FALSE; //Leds apagados
	IndicRx=0;
	PutString ("Salgo de comunicación \n\r\n\r");
}


void maincpp (void)
{
	int n;
	indicativo = *const_indicativo;
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

	sprintf (str,"%d",indicativo);
	PutString (str);
	PutString ("\n\r");

	bMode=MODE_STANDBY;
	EnableInterrupts;
	FX828.Init ();

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);
		
	FX828.SetDecoderBandwidth (0x0f);				//Ponemos el ancho de banda a 2.7
	FX828.Mod1Enable (FALSE);	//Deshabilitamos mod1		
	FX828.Mod2Enable (TRUE);
	FX828.EnableCTCSSDecoder (FALSE, PREDICTIVE_MODE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);

	if (indicativo==0xffff)
		ErrorIndicativo();

	for(;;)
	{
		if (!COLGADO && PTB) 	//El usuario descuelga sin que llamen.
		{											//Si hay algún led encendido es que ha habido comunicación
			delay_ms (100);
			
			if (!COLGADO)
			{					
				SubtonoTX = TxCTCSSTable [(IndicRx-1)*5];
				SubtonoRX = RxCTCSSTable[(IndicRx-1)*5];
				
				
				sprintf (str,"Subtono = %f \n\r",TxCTCSSTable [(IndicRx-1)*5]);
				PutString (str);
			
				FX828.ProgramFastCTCSS (SubtonoTX);//Subtono de comunicación
			 	FX828.PutToneTable (0,SubtonoRX);		//Tono de SQL
				comunicando();				//Realiza la comunicación			
			}
		}
		
		if (SelCallDetected)
		{
			sprintf (str,"Tecla pulsada = %03x\n\r",TeclaPulsada.valor);
			PutString (str);
			if (TeclaPulsada.valor == indicativo)
			{
				if (IndicRx>0 && IndicRx<10)
				{
					PTB = HEX2BIN (IndicRx);
					GeneraTimbre();
				}	
			}			
			
			if (TeclaPulsada.valor == 0x999)
				PTB = 0;
				
			SelCallDetected=FALSE;
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
			sprintf (str,"%x",tono);
			PutString (str);
		
			if (bStart==TRUE)
			{
				buffer[idx++]=tono;	
				if (idx==4)
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
				}		
			}

			if (tono==0xb)	//Inicio de trama
			{
				idx=0;
				bStart=TRUE;
			  PutString ("\n\r");
			}		
		}	
	}
	
	if (bMode == MODE_COMUNICATION)
	{
		switch (tono)
		{
			case 0:
//				while (Terminal_SendChar ((UCHAR)'1')==ERR_TXFULL);
				bSQL = TRUE;
			break;
			
			default:
//				while (Terminal_SendChar ((UCHAR)'0')==ERR_TXFULL);
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
