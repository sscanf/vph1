/* ================================================================================================================================= **
** ================================================================================================================================= **
** CONFIGURATION FILE FOR PESL LIBRARY                                                                                               **
** ================================================================================================================================= **
** ================================================================================================================================= */

#ifndef __PESLH
#define __PESLH

#define _MC68HC08AB16A      1
#define _MC68HC08AZ32       2
#define _MC68HC08AZ32A      3
#define _MC68HC08AZ60       4
#define _MC68HC908AZ60      5
#define _MC68HC908AZ60A     6
#define _MC68HC908AB32      10

#define _MC68HC908GP32_44   11
#define _MC68HC908GP32_42   12
#define _MC68HC908GP32_40   13
#define _MC68HC908JL3       14
#define _MC68HC908JK1       15
#define _MC68HC908GR8_28    16
#define _MC68HC908GR8_32    17
#define _MC68HC908KX2       18
#define _MC68HC908KX8       19

#define _MC68HC908AS60A_52  20
#define _MC68HC908AS60A_64  21
#define _MC68HC908MR8_28    22
#define _MC68HC908MR8_32    23
#define _MC68HC908MR32_56   24
#define _MC68HC908MR32_64   25
#define _MC68HC908RK2       26
#define _MC68HC908EY16      27
#define _MC68HC908GT16_42   28
#define _MC68HC908GT16_44   29
#define _MC68HC908LJ12_52   30
#define _MC68HC908LJ12_64   31
#define _MC68HC908QT1       32
#define _MC68HC908QT4       33
#define _MC68HC908QY1_P_DW  34
#define _MC68HC908QY4_P_DW  35
#define _MC68HC908QY1_SD    36
#define _MC68HC908QY4_SD    37

#define _MC68HC908SR12_42   38
#define _MC68HC908SR12_48   39
#define _MC68HC908RF2       40
#define _MC68HC908LD64      41
#define _MC68HC908BD48_28   42
#define _MC68HC908BD48_42   43
#define _MC68HC908BD48_44   44
#define _MC68HC908JB8_20    45
#define _MC68HC908JB8_28    46
#define _MC68HC908JB8_44    47
#define _MC68HC908JB16_28   48
#define _MC68HC908JB16_32   49
#define _MC68HC908JG16      50
#define _MC68HC908JL8_20    51
#define _MC68HC908JL8_28    52
#define _MC68HC908JL8_32    53

#define _MC68HC908AP64_42   54
#define _MC68HC908AP64_44   55
#define _MC68HC908AP64_48   56
#define _MC68HC908GR16_28   57
#define _MC68HC908GR16_32   58
#define _MC68HC908GR16_48   59
#define _MC68HC908GZ16_32   60
#define _MC68HC908GZ16_48   61
#define _MC68HC908LK24_64   62
#define _MC68HC908LK24_80   63

/* Selected target MCU */

#define CPUtype _MC68HC908GP32_44


/* PESL library */

#pragma MESSAGE DISABLE C4000 /* WARNING C4000: Condition is always TRUE */
#pragma MESSAGE DISABLE C4001 /* WARNING C4001: Condition is always FALSE */

#include "PESLlib.h"

#endif /* __PESLH */
