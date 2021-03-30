#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	char ad[20],soyad[20],pass=0;
	int num;
	
	printf("numara girin=");
	scanf("%d",&num);
	printf("ad soyad giriniz(ad soyad(ascii karakterleri ile))=");
	scanf("%s %s",ad,soyad);
	
	if(num!=17014000)
	{
		printf("numara yanlis!!!\n");
		pass++;
	}
	if(strcmp(ad,"ad"))
	{
		printf("ad yanlis!!!\n");
		pass++;
	}
	if(strcmp(soyad,"soyad"))
	{
		printf("soyad yanlis!!!%s\n",soyad);
		pass++;
	}
	if(!pass)
		printf("hos geldiniz %s %s",ad,soyad);
		
	return 0;
}
