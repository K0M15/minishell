#include <libc.h>

int main()
{
	int test[2] = { 0 };
	
	*(long long*) test = ((long long) 0xAF<<32) | 0xFE;
	printf("DATA1 %d, DATA %d", test[0], test[1]);
}