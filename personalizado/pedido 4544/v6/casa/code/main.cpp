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

#include "fx828.h"

#define CTCSS_ACCESO1	0

#define COLGADO		PTA_PTA6
#define PTT_ON		PTC_PTC2 = 0
#define PTT_OFF		PTC_PTC2 = 1
#define BEEP_ON		PTB_PTB7 = 1
#define BEEP_OFF	PTB_PTB7 = 0

#define LED_PUERTA1	PTB_PTB6

#define MODE_STANDBY		0
#define MODE_COMUNICATION	1

//#define MAKEBYTE(b, a)  ((((byte)(a)) | (((byte)(b))) << 4))
#define HEX2BIN_R(a) (((byte)(0x80)) >> (byte)(a))
#define HEX2BIN_L(a) (((byte)(0x1)) << (byte)(a-1))


extern "C" byte Terminal_SendChar(byte Chr);

unsigned int const_indicativo[1] @0xf700;
unsigned int indicativo;

const unsigned char  reverseByte[256] = {
       0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 
       0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
       0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 
       0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
       0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 
       0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
       0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 
       0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
       0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 
       0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
       0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 
       0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 
       0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 
       0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
       0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 
       0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 
       0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 
       0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 
       0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 
       0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
       0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 
       0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 
       0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 
       0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 
       0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 
       0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
       0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 
       0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 
       0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 
       0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
       0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 
       0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF 
};


const float TxCTCSSTable[]={

									94.8,  /* Acceso 3 SUBTONO COMUNICACION*/ 	 
									103.5, /* Acceso 3 Abrir puerta 3*/   
									100.0, /* Acceso 3 Abrir puerta 2*/   
									97.4,  /* Acceso 3 Abrir puerta 1*/   
									107.2, /* Acceso 3 Abrir puerta 4*/   
																									
									79.7, /* Acceso 2 SUBTONO COMUNICACION*/ 	
									88.5, /* Acceso 2 Abrir puerta 3*/ 
									85.4, /* Acceso 2 Abrir puerta 2*/ 									
									82.5, /* Acceso 2 Abrir puerta 1*/ 
									91.5, /* Acceso 2 Abrir puerta 4*/   									

							  	67.0, /* Acceso 1 SUBTONO COMUNICACION*/ 	
									74.4, /* Acceso 1 Abrir puerta 3*/ 
							  	71.9, /* Acceso 1 Abrir puerta 2*/
							  	69.3, /* Acceso 1 Abrir puerta 1*/ 
									77.0}; /* Acceso 1 Abrir puerta 4*/
						


const int RxCTCSSTable[]=  {0x587,	/*94.8  Acceso 3 SUBTONO COMUNICACION*/
														0x494,	/*79.7  Acceso 2 SUBTONO COMUNICACION*/
														0x3d8};	/*67.0  Acceso 1 SUBTONO COMUNICACION*/ 
														
								 
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

bool apertura(char);

char tmp_apertura=0x70;	//PORT B
char tmp_estacion=0x38;	//PORT C
bool SelCallDetected;
bool bSQL;
int SubtonoRX;
float SubtonoTX;

char str[50];
char memCamara[3];	//Memoriza la camara seleccionada en cada estación

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
void GeneraTimbre (BOOL bFlag)
{
	int n,i;
	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	SelCallDetected = FALSE;
	for (n=0;n<5 && (COLGADO || bFlag);n++)
	{
		BEEP_ON;
		for (i=0;i<500 && (COLGADO || bFlag);i++)
			delay_ms(1);
		BEEP_OFF;
		for (i=0;i<200 && (COLGADO || bFlag);i++)
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

char APERTURA_GetVal()
{
	char val=(PTB&0x70);	
	return (val^tmp_apertura);
}

char ESTACION_GetVal()
{
	char val=(PTC&0x38);	
	return (val^tmp_estacion);
}


bool SelecEstacion(char estacion=0)
{
	unsigned char tmp= ESTACION_GetVal();
	char n;
	char tono;
	static char antTecla;

	if (!estacion)
	{
		//Bucle antirebotes	
		for (n=0;n<10;n++)
		{
			delay_ms(1);
			if (ESTACION_GetVal()!=tmp)
			{
				n=0;
				tmp=ESTACION_GetVal();
			}
		}

		if (ESTACION_GetVal())	//¿Ha estado la línea alta fija durante 10ms?
		{

			DDRC|=0x38;
			PTC|=0x38;
			
			DDRC&=0xC7;		//Bits como entradas
			delay_ms (10);
			DDRC|=tmp;	//Ponemos los bits cambiados como salidas.
			PTC|=0x38; 
			PTC&=~tmp; 	//Encendemos el led correspondiente.

			tmp_estacion=(PTC&0x38);
			IndicRx = (((tmp)>>3)/2)+1;
			apertura (memCamara[IndicRx-1]);
		return TRUE;
		}
	}
	else
	{
		
		tmp=reverseByte [HEX2BIN_L (estacion)]>>5; //Los pulsadores están alambrados al reves.
		tmp = (tmp<<3);
		DDRC|=0x38;
		PTC|=0x38;
		
		DDRC&=0xC7;
		delay_ms(10);
		
		
		DDRC|=tmp;
		PTC&=~tmp;
		
		tmp_estacion=(PTC&0x38);
		IndicRx = (((tmp)>>3)/2)+1;
		
		apertura (memCamara[IndicRx-1]);
		return TRUE;			
	}
	return FALSE;
}

//Antes de abrir la puerta, nos aseguramos de los rebotes del pulsador
//Si se mantienen altos fijos durante 100ms enviaremos el subtono de apertura
//y devolveremos un TRUE.
//En caso contrario devolvemos un FALSE sin ninguna acción.

bool apertura(char camara=0)
{
	unsigned char tmp=APERTURA_GetVal();
	char tmpPB,indic;
	char n;
	char tono;
	static char antTecla;

	if (!camara)
	{	
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
			DDRB|=0x70;
			PTB|=0x70;
			
			DDRB&=0x8f;		//Bits como entradas
		
			PutString ("Subtono apertura\n\r");
			EnableTX (TRUE);
			
			DDRB|=tmp;	//Ponemos los bits cambiados como salidas.
			
			PTB|=0x70;
			PTB&=~tmp; //Encendemos el led correspondiente.
			tmp_apertura = (PTB&0x70);
		
			indic = (IndicRx-1)*5;
			TeclaPulsada.valor = ((tmp)>>4)/2;

			FX828.ProgramFastCTCSS (TxCTCSSTable [indic+4]); //Transmitimos subtono de voz
			delay_ms (500);
			FX828.ProgramFastCTCSS (TxCTCSSTable [(indic+(TeclaPulsada.valor))+1]); //Transmitimos subtono de apertura
			delay_ms(1000);
			
			memCamara[IndicRx-1]=((tmp>>4)/2)+1;
			FX828.Mod2Enable (TRUE);
					
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		{
			if (camara<0xff)
			{
				tmp=0x01;
				tmp=tmp<<(camara-1);
				DDRB|=0x70;
				PTB|=0x70;
				
				DDRB&=0x8F;
				delay_ms(10);
				
				
				DDRB|=(tmp<<4);
				PTB&=~(tmp<<4);
				
				tmp_apertura=(PTB&0x70);
				TeclaPulsada.valor = tmp/2;
  			memCamara[IndicRx-1]=(tmp/2)+1;
  			delay_ms(100);
			}
			else
			{
				DDRB|=0x70;
				PTB|=0x70;
				DDRB&=0x8F;
				delay_ms(100);
				tmp_apertura=(PTB&0x70);
			}
		}
	return TRUE;
}

void comunicando ()
{
	int n,i;
	BOOL bFlag;
	
	for (n=1;n<15;n++)
		FX828.PutToneTable (n,0);

	PutString ("Entrando en comunicación \n\r");
	
	FX828.EnableToneDecoder (FALSE,SELCALL_MODE);
	FX828.EnableToneDecoder (TRUE,CTCSS_MODE);			//Ponemos el receptor de tonos en modo CTCSS
	bMode = MODE_COMUNICATION;
	PutString ("En modo comunicación ... \n\r");

	PutString ("Esta la linea ocupada? \n\r");
	// Tiene que haber subtono durante 1 segundo.
	for (n=0;n<500 && !bSQL;n++) delay_ms(1); 
	
	if (!bSQL)	//Si hay subtono es que la línea está ocupada
	{
		PutString ("Linea libre \n\r");
		even_timeout=120000;

		PTT_ON;
		
		PutString ("Generando tono de comunicacion ... \n\r");
		FX828.ProgramZVEI(400.0);		//Tono de espera comunicación
		FX828.ZVEI_tx_enable (TRUE);
		FX828.CTCSS_tx_enable (TRUE);
		
		FX828.Mod1Enable (TRUE);
	
		delay_ms(100);
		PutString ("Esperando subtono de puerta válido \n\r");
		bFlag=TRUE;
		do
		{
			PTB_PTB2 = TRUE;
			delay_ms (100);
			
			if (bSQL)
			{
				// Tiene que haber subtono durante 1 segundo.
				for (n=0;n<1000 && bSQL;n++) delay_ms(1); 
				if (n>999)
				{
					PutString ("Hay subtono \n\r");
					break;
				}
			}

			PTB_PTB2 = FALSE;
			delay_ms (300);
			
		}while (!COLGADO && even_timeout); //Mientras no cuelgan y no pasan 2 minutos
		
		if (even_timeout && !COLGADO)
		{	
			PutString ("Subtono validado \n\r");
			
			FX828.ZVEI_tx_enable (FALSE);		

			PTB_PTB2 = TRUE;	//Encendemos el led de comunicación (amarillo)
			bFlag=FALSE;
			
			//Mientras recibamos subtono, no cuelguen y no salte el time out
			while (!COLGADO && even_timeout)
			{
				FX828.EnableBPF (PTB_PTB3);    	//Habilitamos salida de audio rx

				if (!bSQL)	//Perdemos el subtono de recepción
				{
					//Durante 1 segundo ha de volver el subtono, si no cortamos la comunicación
					PutString ("Ha desaparecido el subtono, esperando...  \n\r");
					for (n=0;n<1000 && !bSQL;n++) 
					{				
						if (bSQL)	//Vuelve subtono
							//Nos aseguramos de que no sea ruido aleatorio
							for (int i=0;i<100 && bSQL;i++) delay_ms(1); 
						else
							delay_ms(1);
					}
					if (n>=999)
					{	
						PutString ("El subtono no aparece, salimos \n\r");
					
						even_timeout=0;
						break;
					}
				}

				if (APERTURA_GetVal())	//Si hay subtono de RX,está descolgado y pulsan apertura.
				{
					PutString ("Pulsan apertura \n\r");
					if (apertura());
						even_timeout = 120000;		  //Empezamos la cuenta del time out					
				}
			}
		}

		FX828.CTCSS_tx_enable (FALSE);		//Quitamos subtono
		for (n=0;n<1000 && !bFlag;n++) delay_ms(1);	//Esperamos 1 segundo 
		EnableTX (FALSE);
		FX828.EnableBPF (FALSE);    	//Habilitamos salida de audio rx
	}
	else
	{
		PutString ("Generando tonos de linea ocupada... \n\r");
		BussySignal();
	}

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);

	FX828.EnableToneDecoder (FALSE,CTCSS_MODE);
	FX828.EnableToneDecoder (TRUE,SELCALL_MODE);
	bMode = MODE_STANDBY;
	PutString ("En modo standby ... \n\r");
	
//	if (!even_timeout)
//			UnlinkSignal();	
	
	PTB_PTB2 = FALSE;	//Apagamos el led de comunicación (amarillo)
	PutString ("Salgo de comunicación \n\r\n\r");
}


void maincpp (void)
{
	int n;
	indicativo = *const_indicativo;
	bool bSQL,bColgado;
	
	DDRC_DDRC2 = TRUE;

	DDRC_DDRC3 = FALSE;		//BITS DE SELECCION 
	DDRC_DDRC4 = FALSE;
	DDRC_DDRC5 = FALSE;
	
	
	DDRA_DDRA6 = FALSE;
	DDRB = 0x87;
	
	DDRD_DDRD4 = FALSE;
	PTB = 00;
	
	SelCallDetected = FALSE;
	BEEP_OFF;
	PTT_OFF;
	bSQL=FALSE;
	bColgado=FALSE;

	sprintf (str,"%d",indicativo);
	PutString (str);
	PutString ("\n\r");

	bMode=MODE_STANDBY;
	EnableInterrupts;
	FX828.Init ();

	memset (memCamara,0xff,sizeof (memCamara));
	
	SelecEstacion (1);	//Por defecto, seleccionamos la estación 1
	TeclaPulsada.valor=1;
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
		if (!COLGADO && !bColgado) 	//El usuario descuelga sin que llamen.
		{														//Si hay algún led encendido es que ha habido comunicación
			delay_ms (100);
			
			if (!COLGADO)
			{					
				SubtonoTX = TxCTCSSTable [(IndicRx-1)*5];
				SubtonoRX = RxCTCSSTable[(IndicRx-1)];
				
				
				sprintf (str,"Subtono = %f \n\r",TxCTCSSTable [(IndicRx-1)*5]);
				PutString (str);
			
				FX828.ProgramFastCTCSS (SubtonoTX);//Subtono de comunicación
			 	FX828.PutToneTable (0,SubtonoRX);		//Tono de SQL
				comunicando();				//Realiza la comunicación			
				
				//Sigue estando descolgado
				if (!COLGADO)
					bColgado=TRUE;
			}
		}
		
		if (COLGADO && bColgado) //El usuario cuelga despues de un TimeOut de comunicación
			bColgado=FALSE;
			

		if (ESTACION_GetVal())
			SelecEstacion();
		
		//Pulsan apertura sin haber descolgado y sin que hayan llamado.
		//Pero no se recibe ningún subtono
		if (APERTURA_GetVal())
		{
			if (apertura())
				EnableTX (FALSE);			
		}
		
		if (SelCallDetected)
		{
			if (TeclaPulsada.valor == indicativo)
			{
				if (IndicRx>0 && IndicRx<10)
				{
					SelecEstacion (IndicRx);
					GeneraTimbre(bColgado);
				}
			}			
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
		else
		{
			idx=0;
			bStart=FALSE;
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
