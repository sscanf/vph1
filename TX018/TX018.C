///////////////////////////////////////////////////////
//
// PROGRAMA PARA EL MODEM RM018
//---------------------------------------------------------
//
// Parametros para el compilador: +FM +t
//
// 
////////////////////////////////////////////////////////////

#include <y:\picc\examples\16c63.h>
//#include <y:\picc\examples\16c54.h>
#include <y:\picc\examples\ctype.h>
#include <y:\picc\examples\stdlib.h>
#include <y:\picc\examples\string.h>      

#byte port_a=5
#byte port_b=6


#use delay (clock=20000000)

//#fuses HS,NOWDT,NOPROTECT,PUT

void PllProgram (long n, long r, int a);
char frec (int fr);
void frecQuery (int fr);

#define EE_N	0
#define EE_R	2
#define EE_A	4

void main (void)
{   
	// CALCULADO CON MD2.EXE

	// FRECUENCIA: 868975
	// PRESCALER: 64
	// FREC.REF: 25
	// OSCILADOR: 4000

	char h,l;
	long n;
	long a;
	long r;
    
	n=543;
	a=14;
	r=321;
	
	PllProgram (n,r,LOBYTE(a));
	while (1);
}

void LoadEnable()
{
	output_high(PIN_A2);
	output_low (PIN_A2);	
}

void Write (int bt)
{                    
	int n;
	
	for (n=0;n<8;n++)
	{
		output_bit (PIN_C5,shift_left (&bt,1,0));
		output_high (PIN_C3);	//Clock
		delay_us(1);
		output_low (PIN_C3);	//Clock
	}
}

void PllProgram (long n, long r, int a)
{
	Write (HIBYTE(r));
	Write (LOBYTE(r));
	LoadEnable();
	
	Write (HIBYTE(n));
	Write (LOBYTE(n));
	Write (a);
	LoadEnable();
}

