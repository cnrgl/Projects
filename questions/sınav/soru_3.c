#include <stdio.h>

int main(void)
{
	char str[100];	
	int i=0;
	fgets(str,100,stdin);//kullan�c� giri�i buffera yaz�l�yor
		while(str[i])//string sonuna kadar tarama yap�l�yor
		{
			if((str[i]=='a')||(str[i]=='e')||(str[i]=='i')||(str[i]=='o')||(str[i]=='u')); //karakter tespiti  sesli ise yazma
			 else
			   printf("%c",str[i]);
			i++;	
		}
		
		
	return 0;
}
