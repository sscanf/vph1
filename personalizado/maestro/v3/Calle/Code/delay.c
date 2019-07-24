#include "events.h"

void delay_ms (int time)
{
	int n;
	for (n=0;n<time;n++)
	{
		even_timer = FALSE;
		while (!even_timer);
	}
}
