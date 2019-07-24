#include "pe_types.h"
#include "buffer.h"

#define BUFFER_FULL	-2
#define ERROR		-1

#define	GENERAL_RESET			0x01
#define SIGNALLING_CONTROL		0x80
#define SELCALL_SUBAUDIO_STATUS 0x81
#define	SIGNALLING_SETUP		0x82
#define CTSS_TX_FAST_RX_FREQ	0x83
#define	RX_TONE_PROGRAM			0x84
#define	DCS_BYTE3				0x85
#define	DCS_BYTE2				0x86
#define	DCS_BYTE1				0x87
#define GENERAL_CONTROL			0x88
#define	AUDIO_CONTROL			0x8A
#define GENERAL_PURPOSE_TIMER	0x8B
#define	SELCALL_TX				0x8D
#define	IRQ_MASK				0x8E
#define IRQ_FLAG				0x8F

#define CTCSS_MODE	 			0
#define SELCALL_MODE 			1

#define chip_select_ClrVal() (PTC &= ~64)
#define chip_select_SetVal() (PTC |= 64)
#define DataIn_GetVal() ((PTD) & 2)

#define DETECT_MODE		0
#define	PREDICTIVE_MODE	1
#define	UNKNOWN_TONE	0x0f

#ifdef __cplusplus

class CFX828
{
	public:
		CFX828 ();
		~CFX828 ();
		
		void Init ();
		int  TxZVEI (const byte *);
		int	 TxZVEI (byte tono);
		bool IsTxBufferEmpty();
		void ProgramFastCTCSS (int tono);
		void CTCSS_tx_enable (unsigned char enable);
		void PutToneTable (byte pos, int tono);
		void SetDecoderBandwidth (byte width);
		void EnableToneDecoder (bool enable, bool mode);
		void EnableCTCSSDecoder (bool enable, bool mode);
		void IRQService ();
		char GetTone ();
		bool CTCSS_Detected (void);
		void SetRepeatTone (byte tono = 0xe) {_RepeatTone = tono;}
		void Mod1Enable (bool enable, char gain = 0x1f);
		void Mod2Enable (bool enable, char gain = 0x1f);
		void ZVEI_tx_enable (bool enable);
		void ProgramZVEI (float tono);
		void EnableBPF(bool enable);
		
		
		
	private:
		void SendCommand8(unsigned char address, unsigned char command);
		void SendCommand16(unsigned char address, unsigned int command);
		unsigned char ReadCommand(unsigned char address);
		void Clock_PutVal(byte Value);
		void SendChar (byte Chr);
		byte ReadChar (void);
		void DataOut_PutVal(byte Value);

		CBuffer *_bufferTX;
		CBuffer *_bufferRX;
		
		byte _RepeatTone;
};
#endif

extern union
{
	struct
	{
		unsigned int _dcs			:1;
		unsigned int bit1			:1;
		unsigned int _ctcss_fast	:1;
		unsigned int _tone			:1;
		unsigned int _comp1to0		:1;
		unsigned int _comp0to1		:1;
		unsigned int _gpt			:1;
		unsigned int bit7			:1;
	}bit;
	unsigned char valor;
}irq_flag;

extern union 
{
   struct
   {
       unsigned int	_dcs_rx_enable		:1; //Bit 0
       unsigned int	_null2				:1; //Bit 1
       unsigned int	_selcall_tx_enable	:1; //Bit 2
       unsigned int	_null1				:2; //Bit 4,3
       unsigned int	_fast_detect_enable	:1; //Bit 5
       unsigned int	_tone_decoder_enable:1;	//Bit 6
       unsigned int	_subaudio_tx_enable	:1;	//Bit 7
   }bit;
   unsigned char valor;
}signalling_control;

extern union
{
   struct 
   {
       unsigned int _dcs					:1;
       unsigned int	_subaudio_tx_mode		:1;
       unsigned int	_tone_decoder_mode		:1;
       unsigned int	_fast_ctcss_mode		:1;
       unsigned int	_tonde_decode_wandwitch	:4;
   }bit;
   unsigned char valor;
}signalling_setup;


extern union
{
   struct
   {      
       unsigned int	_gp_timer_recycle	:1;
       unsigned int	_gp_timer_enable	:1;
       unsigned int	_dac				:3;
       unsigned int	_bpf_6db_pad		:1;
       unsigned int	_bpf_unmute			:1;
       unsigned int	_bpf_enable			:1;       
   }bit;
   unsigned char valor;
}general_control;

extern union
{
   struct
   {      
       unsigned int	_dcs		:1;
       unsigned int	_bit1		:1;
       unsigned int	_ctcss_fast	:1;
       unsigned int	_tone		:1;
       unsigned int	_comp		:2;
       unsigned int	_gp_timer	:1;
       unsigned int	_bit7		:1;
   }bit;
   unsigned char valor;
}irq_mask;

extern union
{
   struct
   {
       unsigned int	_ctss_tx_fast_rx_frequency	:13;
       unsigned int	_null						:2;
       unsigned int	_ctcss_tx_notone			:1;      
   }bit;
   unsigned int valor;
}ctcss_tx_fast_rx_freq;

extern union
{
   struct
   {
       unsigned int _tone_frequency	:12;
       unsigned int	_tone_address	:4;
       
   }bit;
   unsigned int valor;
}rx_tone_program;

extern union
{
   struct
   {
   	   unsigned int	_mod1_gain		:5;	//Bits 0-4
   	   unsigned int	_mod1_enable	:1; //Bits 5
   	   unsigned int	_null2			:2; //Bits 6,7
   	   unsigned int	_mod2_gain		:5; //Bits 8-12
   	   unsigned int	_mod2_enable	:1; //Bits 13
  	   unsigned int	_null			:2; //Bits 14,15
   }bit;
   unsigned int valor;
}audio_control;

extern union
{
   struct
   {
   	   unsigned int	_selcall_tx_tone	:13;
   	   unsigned int	_null				:2;
   	   unsigned int	_secall_notone		:1;
   }bit;
   unsigned int valor;
}selcall_tx;


extern union
{
	struct
	{
		unsigned _rx_tone		:4;
		unsigned _tone_decode	:1;
		unsigned _bit5			:1;
		unsigned _ctcss_fast	:1;
		unsigned _dcs_decode	:1;
	}bit;
	unsigned char valor;
	
}selcall_subaudio_status;

#define _rx_tone	selcall_subaudio_status.bit._rx_tone
