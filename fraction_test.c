#include <stdio.h>

	typedef union{
		int x;
		double y; // it is long float
	}data;
	
int main(void)
{
	
	data test;
	
	test.y = 10.1;
	test.x = (int)test.y; 
	
	printf("test int = %d\n",test.x);
	
	printf("test double = %lf\n",test.y);
	
	printf("test fraction existence %d",((test.y-test.x)>0));	// instead of test.x = (int)test.y can be usable 
	
	return 0;
}
