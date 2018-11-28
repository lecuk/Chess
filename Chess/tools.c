#include "tools.h"

int sign(int value)
{
	return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}

int abs(int value)
{
	return (value > 0) ? value : -value;
}