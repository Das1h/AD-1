#pragma once
#include <time.h>

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

void get_time_stamp(char* time_stamp)
{
	time_t t = time(NULL);
	strftime(time_stamp, 32, "%Y%m%d%H%M%S", localtime(&t));
}
