#include "math.h"
#include "buffer.h"
#include <stdlib.h>

CBuffer::CBuffer (void)
{
	Init();
}

char CBuffer::operator[] (char pos)
{
	return _ptrBuffer[pos];
}
CBuffer::CBuffer (int len)
{
	Init(len);
}

CBuffer::~CBuffer(void)
{
	free (_ptrBuffer);
}


void CBuffer::Init (int len /* = 10*/)
{
	_ptrBuffer = (char *)malloc ((char)10);
	_next_in=0;
	_next_out=0;
}
unsigned char CBuffer::kbhit()
{
	return (_next_in!=_next_out);
}

int CBuffer::GetLen()
{
	return abs (_next_out-_next_in);
}

void CBuffer::put (char Chr)
{
   int t;

   _ptrBuffer[_next_in]=Chr;
   t=_next_in;
   _next_in=(_next_in+1) & BUFFER_SIZE;
   if(_next_in==_next_out)
     _next_in=t;           // Buffer full !!
}

char CBuffer::get()
{
   unsigned char c;

   while(!kbhit());
   c=_ptrBuffer[_next_out];
   _next_out=(_next_out+1) & BUFFER_SIZE;
   return(c);
}

