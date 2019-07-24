void SPI_SendChar8 (byte Chr)
{
  byte i;                                                        
  for(i=0; i<8; i++) {
    SPI_CLOCK_PutVal(1);		            /* Set CLK to shift value */
    SPI_DATAOUT_PutVal(Chr > 0x7f);        /* Set value on MOSI */
    SPI_CLOCK_PutVal(0);		            /* Set CLK to sample value */
    Chr <<= 1;
  }
}

void SPI_SendChar16(unsigned int Chr)
{
  byte i;
  
  for(i=0; i<16; i++) {
    SPI_CLOCK_PutVal(1);		            /* Set CLK to shift value */
    SPI_DATAOUT_PutVal(Chr > 0x7fff);       /* Set value on MOSI */
    SPI_CLOCK_PutVal(0);		            /* Set CLK to sample value */
    Chr <<= 1;
  } 
}

void SPI_SendChar24(unsigned long Chr)
{
  byte i;                                                        
  for(i=0; i<24; i++) {
    SPI_CLOCK_PutVal(1);		     	      /* Set CLK to shift value */
    SPI_DATAOUT_PutVal(Chr > 0x7fffffff);       /* Set value on MOSI */
    SPI_CLOCK_PutVal(0);		              /* Set CLK to sample value */
    Chr <<= 1;
  }
}
