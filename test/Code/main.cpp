
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
#include <string.h>
#include <cstring.h>
#include <ctype.h>
#include "delay.h"
#include "maincpp.h"
#include "io_map.h"

#include "fx828.h"
#define BS   '\010'
#define DEL  '\177'
#define CR   '\015'
#define LF   '\012'
#define ESC  '\033'

#define CTCSS_RX		12
#define CTCSS_APERTURA	0

#define APERTURA_PUERTA	PTC_PTC4

#define PTT_ON			PTC_PTC2 = 0
#define PTT_OFF			PTC_PTC2 = 1

#define AUDIO_ENABLE	PTC_PTC5 = 0
#define AUDIO_DISABLE	PTC_PTC5 = 1

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


const char *comandos[] = {"A",
						  "RxZVEI",
						  (char *)NULL
						  };
						 
const int indicativo = 1;

bool SelCallDetected;
bool CTCSSDetected;

byte GetChar (void)
{
 	while (!SCS1_SCRF);
	return SCDR;
	
}

void PutString (char *string)
{
	int n;
	for (n=0;string[n]!=0;n++)
		while (Terminal_SendChar (string[n])==ERR_TXFULL);
}

void GetString(char *str, int maxLen) {
	/* reads a string int buffer 'str' with maxLen */
	
	byte ch;
	int len = 0;

	ch = GetChar();
	while (ch >= ' ' || ch == BS || ch == DEL && ch!=CR && ch!=LF)
	{
		if (ch >= ' ') 
		{
		  if ( len < maxLen) 
		  {
		    str[len++] = ch; 
			while (Terminal_SendChar (ch)==ERR_TXFULL);
		  }
		} 
		else if (len > 0) 
		{
			while (Terminal_SendChar (ch)==ERR_TXFULL);
			len--;
		}
	
		ch = GetChar();
	}
	str[len++] = 0;
}

CFX828 FX828;


void TxZVEI (char *param)
{
	char par[10];
	
	PutString ("TxZVEI\n\r");
	if (strlen (param)==0)
	{
		PutString ("Falta un parámetro\n\r");
		return;
	}
	
	do
	{
		//Quitamos los parametros
		for (n=0;*param!=',' && *param!='\0';param++,n++)
			par[n]=*param;
		
		FX828.TxZVEI (par);
	}while (param!='\0');
}

void RxZVEI (void)
{
	PutString ("Recibiendo tonos ZVEI (Pulse cualquier tecla para finalizar)\n\r");

	//Grabamos los tonos ZVEEI en la tabla  RX TONE PROGRAM TABLE
	for (n=0;n<15;n++)
		FX828.PutToneTable (n,RxZVEI1Table[n]);
	
	while (!SCS1_SCRF)
	{
		if (
	}
}

void maincpp (void)
{	
	int n;
	bool bFlag;
	signed char tecla;
	char str[20];
	char *cmd;
	char *param;
	
	DDRA = 0;		//Porta como entradas (Teclado)
	DDRB = 0;
	DDRD_DDRD4=TRUE;

	DDRC_DDRC3=TRUE;
	DDRC_DDRC4=TRUE;
	DDRC_DDRC5=TRUE;

	SelCallDetected = FALSE;
	CTCSSDetected = FALSE;
	
	EnableInterrupts;
	FX828.Init ();
	TIMER_Enable();

	PutString ("VPH1 - Diagnostics\n\r");
	PutString ("Electrónica Barcelona,S.L.\n\r\n\r");
	
	while (TRUE)
	{
		PutString (">");
		GetString (str,20);
	
		//Quitamos los espacios de la izda.
		for (cmd=str;*cmd!='\0' && *cmd==' ';cmd++);
		
		//Buscamos los parámetros
		for (param=cmd;*param!='\0' && *param!=' ';param++);
		*param='\0';
		param++;

		for (n=0;n<2;n++)
			if (strcmp (cmd,comandos[n])==0)
				break;
				
		
		switch (n)
		{
			case 0:
				TxZVEI (param);
			break;
				
			case 1:
				RxZVEI ();
			break;
				
			default:
				PutString ("Error de sintaxis\n\r");
			break;
		}
	}
}

#ifdef __cplusplu
	extern "C"
#endif

void MainIRQ ()
{
	byte tono;
	static byte buffer[4];
	static bool bStart;
	static byte idx;
	
	FX828.IRQService();

	tono = FX828.GetTone();
	
	if (FX828.CTCSS_Detected())
	{
		if (!CTCSSDetected)
			CTCSSDetected=TRUE;	
	}
	else
	{
		if (CTCSSDetected)
			CTCSSDetected=FALSE;
	}
	
	if (tono!=0xff && tono!=UNKNOWN_TONE)	//Hemos detectado un tono conocido
	{	
		if (tono==0)
			APERTURA_PUERTA = TRUE;
		else
			APERTURA_PUERTA = FALSE;
	}
		else
			APERTURA_PUERTA = FALSE;

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
