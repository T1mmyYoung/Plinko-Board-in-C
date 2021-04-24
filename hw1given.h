#pragma once
#include <assert.h>
#include <stdlib.h>
#include <time.h>

/*IMPORTANT: CALL THIS FUNCTION ONCE AT THE START OF THE PROGRAM!*/
/*INITIALIZAES THE RANDOM NUMBER GENERATOR*/
void InitRandom(void)
{
	time_t timestr;

#ifdef _DEBUG
	timestr = 0;
#else
	time(&timestr);
#endif

	srand((unsigned int)timestr);
}

/*RETURNS A RANDOM UNSIGNED INTEGER IN THE INTERVAL [MINVAL,MAXVAL]*/
unsigned int GetRandom(unsigned int minval, unsigned int maxval)
{
	char temp[sizeof(int)];
	int* val;
	int i;
	

	val = (int*)temp;

	for (i = 0; i < sizeof(unsigned int); i++)
	{
		temp[i] = rand() & 0xFF;
	}

	*val = ((*val) % (maxval - minval + 1)) + minval;
	assert((*val <= maxval) && (*val >= minval));
	return  *val;
}

int start();

int LeftOrRight()
{
	unsigned int flag = GetRandom(1,2);
	int move = 0;

	if(flag == 1)
		move =  -1;
	else
		move =  1;

	return move;
}












