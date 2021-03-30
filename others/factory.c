#include <stdio.h>
#include <stdlib.h>

int fak(int num)
{
	if(num)
	return fak(num-1)*num;
	else
	return 1;
}
int main(void)
{
	int num;
	printf("faktoriyeli alinacak sayiyi girin...");
	scanf("%d",&num);	
	printf("sonuc=%d",fak(num));
	
	return 0;
}
