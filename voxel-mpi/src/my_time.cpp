#include "../include/my_time.h"

double nowSeconds(){
	static struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((double)tv.tv_sec) + (tv.tv_usec/1000000.0);
}
