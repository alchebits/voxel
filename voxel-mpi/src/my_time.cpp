#include "../include/my_time.h"

#include <iostream>
#include <chrono>

double nowSeconds(){
//	static struct timeval tv;
//	gettimeofday(&tv, NULL);
//	return ((double)tv.tv_sec) + (tv.tv_usec/1000000.0);
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis / 1000.0;
}
