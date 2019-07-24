#include "FX828.H"
#include "delay.h"
#include "stdlib.h"
#include "ctype.h"
#include "io_map.h"

#define XTAL	4032000

			
const int ZVEI[] = {0x01a4,	//0
				  0x03b7,	//1
				  0x0365,	//2
				  0x031a,	//3
				  0x02d0,	//4
				  0x0293,	//5
				  0x025c,	//6
				  0x0227,	//7
				  0x01f8,	//8
				  0x01ca,	//9
				  0x0168,	//a
				  0x04dc,	//b
				  0x040f,	//c
				  0x0473,	//d
				  0x0184};	//e


CFX828::CFX828 ()
{
	Init();
}

CFX828::~CFX828 ()
{
	delete _bufferTX;
	delete _bufferRX;
}

void CFX828::PutToneTable (byte pos, int tono)
{
	int val = pos;
	val = val <<12;
	val |= tono;
	
	SendCommand16 (RX_TONE_PROGRAM,val);
}

void CFX828::SetDecoderBandwidth (byte width)
{
	signalling_setup.bit._tonde_decode_wandwitch = width;	
	SendCommand8 (SIGNALLING_SETUP,signalling_setup.valor);
}

void CFX828::EnableCTCSSDecoder (bool enable, bool mode)
{
	if (!enable)
		selcall_subaudio_status.valor = 0;
	
	signalling_control.bit._fast_detect_enable = enable;
	SendCommand8 (SIGNALLING_CONTROL,signalling_control.valor);
	
	signalling_setup.bit._fast_ctcss_mode = mode;
	SendCommand8 (SIGNALLING_SETUP,signalling_setup.valor);

	irq_mask.bit._ctcss_fast = enable;
	SendCommand8 (IRQ_MASK,irq_mask.valor);

}

bool CFX828::CTCSS_Detected (void)
{
	return selcall_subaudio_status.bit._ctcss_fast;
	
}
void CFX828::EnableToneDecoder (bool enable, bool mode)
{
	signalling_control.bit._tone_decoder_enable = enable;
	SendCommand8 (SIGNALLING_CONTROL,signalling_control.valor);
	
	signalling_setup.bit._tone_decoder_mode = mode;
	SendCommand8 (SIGNALLING_SETUP,signalling_setup.valor);

	irq_mask.bit._tone = enable;
	SendCommand8 (IRQ_MASK,irq_mask.valor);
}

void CFX828::Mod1Enable (bool enable, char gain)
{
	audio_control.bit._mod1_enable = enable;
	audio_control.bit._mod1_gain = gain;
	SendCommand16 (AUDIO_CONTROL,audio_control.valor);
}

void CFX828::Mod2Enable (bool enable, char gain)
{
	audio_control.bit._mod2_enable = enable;
	audio_control.bit._mod2_gain = gain;
	SendCommand16 (AUDIO_CONTROL,audio_control.valor);
}

void CFX828::EnableBPF (bool enable)
{
	general_control.bit._bpf_unmute = enable;
	general_control.bit._bpf_enable = enable;
	SendCommand8 (GENERAL_CONTROL,general_control.valor);
}

void CFX828::Init ()
{
	_bufferTX = new CBuffer(10);
	_bufferRX = new CBuffer(10);
	
	signalling_control.valor=0;
	signalling_setup.valor=0;
	general_control.valor=0;
	irq_mask.valor=0;
	ctcss_tx_fast_rx_freq.valor=0;
	rx_tone_program.valor=0;
	audio_control.valor=0;
	selcall_tx.valor=0;

	_RepeatTone = 0xE;
	
	DDRD_DDRD0 = 0;
	DDRD_DDRD1 = 0;
	DDRD_DDRD2 = 1;
	DDRD_DDRD3 = 1;
	
	DDRC_DDRC6 = 1;
	
    Clock_PutVal(1);
	chip_select_ClrVal ();
	SendChar ((byte)GENERAL_RESET);
	chip_select_SetVal ();

	//Interrupción cada 100ms.
	//Cada tono de ZVEI será de duración un tick de timer.
	SendCommand8 (GENERAL_PURPOSE_TIMER,(byte)70);

	//El contador del timer se auto recicla.
	general_control.bit._gp_timer_enable = TRUE;
	general_control.bit._gp_timer_recycle = TRUE;
	SendCommand8 (GENERAL_CONTROL,general_control.valor);	
	
	//Desactivamos la interrupción para el GENERAL PURPOSE TIMER
	irq_mask.bit._gp_timer = FALSE;
	SendCommand8 (IRQ_MASK,irq_mask.valor);
	
}

void CFX828::CTCSS_tx_enable (bool enable)
{
	signalling_control.bit._subaudio_tx_enable=enable;
	CFX828::SendCommand8 (SIGNALLING_CONTROL,signalling_control.valor);	
}

void CFX828::ProgramFastCTCSS (int tono)
{
	// a = fXTAL (hz) / (16 * fTONE (hz))
	int a;
	
	a = XTAL / (16 * tono);
	ctcss_tx_fast_rx_freq.bit._ctss_tx_fast_rx_frequency = a;
	CFX828::SendCommand16 (CTSS_TX_FAST_RX_FREQ,ctcss_tx_fast_rx_freq.valor);
}

int CFX828::TxZVEI (char tono)
{
	if (_bufferTX->GetLen()>9)
		return BUFFER_FULL;

	if (_bufferTX->GetLen()==0)
	{
		_bufferTX->put (tono);	
		//Activamos la interrupción para el GENERAL PURPOSE TIMER
		irq_mask.bit._gp_timer = TRUE;
		SendCommand8 (IRQ_MASK,irq_mask.valor);	
		ZVEI_tx_enable (TRUE);		
	}
	else
		_bufferTX->put (tono);

	return 0;
}

int CFX828::TxZVEI (const unsigned char *tonos)
{
	unsigned int val,n;
	unsigned char bte[2];

	for (n=0;tonos[n]!=0;n++)	
	{
		bte[0] = tonos[n];
		bte[1] = 0;
		
		val = atoi (bte);
		if (val>15)
			return ERROR;

		while (TxZVEI (val)==BUFFER_FULL);
	}
		
	return 0;
}

void CFX828::ZVEI_tx_enable (bool enable)
{
	signalling_control.bit._selcall_tx_enable=enable;
	SendCommand8 (SIGNALLING_CONTROL,signalling_control.valor);
}

//Programa un tono
void CFX828::ProgramZVEI (float tono)
{
	// a = fXTAL (hz) / (4 * fTONE (hz))

	int a;
	
	a = XTAL / (4 * tono);		
	selcall_tx.bit._selcall_tx_tone = a;
	SendCommand16 (SELCALL_TX,selcall_tx.valor);
	
}

char CFX828::GetTone()
{
	if (!(_bufferRX->kbhit()))
		return -1;
	else
		return _bufferRX->get();

	return 0;
}

bool CFX828::IsTxBufferEmpty()
{
	return ~(_bufferTX->kbhit());
}

void CFX828::IRQService()
{
	static char toneTx,tono;
	static char toneRx = 0xff;
	
	irq_flag.valor = ReadCommand(IRQ_FLAG);

	if (irq_flag.bit._gpt) //General Purpouse Timer IRQ
	{
		if (_bufferTX->kbhit())
		{
			tono = _bufferTX->get();
			if (toneTx == tono)
				tono=_RepeatTone;
				
			 SendCommand16 (SELCALL_TX, ZVEI[tono]);
			 toneTx = tono;
		}
		else
		{
			SendCommand16 (SELCALL_TX, 0);
			signalling_control.bit._selcall_tx_enable=FALSE;
			SendCommand8 (SIGNALLING_CONTROL,signalling_control.valor);
			//Desactivamos la interrupción para el GENERAL PURPOSE TIMER
			irq_mask.bit._gp_timer = FALSE;
			SendCommand8 (IRQ_MASK,irq_mask.valor);	
		}
	}
	
	
	//Hemos detectado un tono.
	if (irq_flag.bit._tone)
	{			
		selcall_subaudio_status.valor = ReadCommand (SELCALL_SUBAUDIO_STATUS);
		if (selcall_subaudio_status.bit._tone_decode &&		//El tono recibido no es un NOTONE (Perdida de tono).
			_rx_tone != toneRx) 							//Además no es el mismo que ha recibido anteriormente
		{
			if (_rx_tone == _RepeatTone)	//Hemos recibido un tono de repetición (R).
											//Esto quiere decir que tenemos que poner en el buffer el mismo tono
				_bufferRX->put (toneRx);		//que habiamos recibido antes.
			else																	
				_bufferRX->put (_rx_tone); //Añadimos el nuevo tono al buffer de recepción
								 		   //Guardamos el tono recibido para próxima comparación.
			toneRx = _rx_tone;
		}
		else
			toneRx = 0xff;
	}
	
	if (irq_flag.bit._ctcss_fast)
	{			
		selcall_subaudio_status.valor = ReadCommand (SELCALL_SUBAUDIO_STATUS);
	}
	
}

void CFX828::SendChar (byte Chr)
{

  byte i;
  
  for(i=0; i<8; i++) {
    DataOut_PutVal(Chr > 127);           /* Set value on MOSI */
    Clock_PutVal(0);		           /* Set CLK to sample value */
    Clock_PutVal(1);		           /* Set CLK to shift value */
    Chr <<= 1;
  }
}
void CFX828::SendCommand16(byte address, unsigned int command)
{

    Clock_PutVal(1);
	chip_select_ClrVal ();
	SendChar ((byte)address);
	SendChar ((byte)HIBYTE (command));
	SendChar ((byte)LOBYTE (command));
	chip_select_SetVal ();
}

void CFX828::SendCommand8(byte address, unsigned char command)
{

    Clock_PutVal(1);
	chip_select_ClrVal ();
	SendChar ((byte)address);
	SendChar ((byte)command);
	chip_select_SetVal ();
}

byte CFX828::ReadCommand(byte address)
{
	byte ret;
	
    Clock_PutVal(1);
	chip_select_ClrVal ();
	SendChar ((byte)address);
	ret = ReadChar ();
	chip_select_SetVal ();
	
	return ret;
}

byte CFX828::ReadChar (void)
{

  byte Chr,i;
  
  for(i=0; i<8; i++)
  {
    Clock_PutVal(0);            /* Set CLK to shift value */
    Clock_PutVal(1);           /* Set value on MOSI */
    Chr <<= 1;
    Chr += DataIn_GetVal()?1:0; /* Read value from MISO */
  }
  return Chr;
}


void CFX828::DataOut_PutVal(byte Value)
{
  if (Value) {                         /* Is it one to be written? */
    PTD |= 4;                          /* Set bit on port */
  }
  else {                               /* Is it zero to be written? */
    PTD &= ~4;                         /* Clear bit on port */
  }
}

void CFX828::Clock_PutVal(byte Value)
{
  if (Value) {                         /* Is it one to be written? */
    PTD |= 8;                          /* Set bit on port */
  }
  else {                               /* Is it zero to be written? */
    PTD &= ~8;                         /* Clear bit on port */
  }
}

