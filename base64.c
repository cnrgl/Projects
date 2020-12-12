#include <stdio.h>

char b64_table[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};
void encode(char *input)
{
	char out,R=0x00;
	int i=0;
	
	while(input[i])
	{
		out = R + (input[i] >> (2*(i%3)+2)); //calc output with remainder
		R = (input[i] << (4 - 2*(i%3))) & 63; 
		printf("%c",b64_table[out]);
		
		if((i%3)==2)
		{ 
			printf("%c",b64_table[R]);
			R=0x00;
		}
		i++;
	}
	//<-- end of the main section encoding -->
	if(i%3==1)
		printf("%c%c%c",b64_table[R],'=','=');
	else if(i%3==2)
		printf("%c%c",b64_table[R],'=');
	//<-- end of the tail section -->	
}
void decode(char *enc)// not completed !!!!!!!!!!!
{
	char out;
	int i=0;
	// enc[i]=rv_search(enc[i]) use ascii !!!!!!!
	while(enc[i])
	{
		out = (enc[i] << (2*(i%3)+2)) + (enc[i+1] >> (4-2*(i%3)));
		i++;
	}
}
int main(void)
{
	char *output,input[256]={0};
	int i=0;
	gets(input);
	encode(input);
	
	return 0;
}


