// written by A. Caner Gül 
#include <stdio.h>

char b64_table[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};
void encode(char *input,char *enc)
{
	char out,R=0x00;
	int i=0,o=-1;
	printf("encoded text = ");
	while(input[i])
	{
		out = R + (input[i] >> (2*(i%3)+2)); 
		R = (input[i] << (4 - 2*(i%3))) & 63; 
		printf("%c",b64_table[out]);
		enc[++o] =  b64_table[out]; //------ for decoding test
		if((i%3)==2)
		{ 
			printf("%c",b64_table[R]);
			enc[++o] = b64_table[R];   //------ for decoding test
			R=0x00;
		}
		i++;
	}
	//<-- end of the main section encoding -->
	if(i%3==1)
	{
		printf("%c%c%c",b64_table[R],'=','=');
		enc[++o] = b64_table[R];//------ for decoding test
		enc[++o] = '=';//------ for decoding test
		enc[++o] = '=';//------ for decoding test
	}
	else if(i%3==2)
	{
		printf("%c%c",b64_table[R],'=');
		enc[++o] = b64_table[R];//------ for decoding test
		enc[++o] = '=';//------ for decoding test
	}
	//<-- end of the tail section -->	
	enc[++o]=-1;
}


int rev(char ch) // can be made by int(char) --> int_index table
{	// ascii manipulation
	if(0x60 < ch) // lower case
		return (char)(ch - 71); // -97 + 26(upper case)
	if(0x40 < ch) // upper case 
		return (char)(ch - 65);
	if(0x3D == ch) // '=' 
		return 0x00;
	if(0x2F == ch) // slash  
		return (char)(63);
	if(0x2B == ch) // '+'
		return (char)(62);
	if(ch<0) //End of the sequence
		return -1;		
	return (char)(ch + 4); // numbers
}


void decode(char *enc)// read two ch. at one step!!! |enc| >= |2ch| 
{
	char out;
	int i=0;
	
	printf("\ndecoded text = ");
	enc[i] = rev(enc[i]);
	while(enc[i]>0)
	{
		enc[i+1]=rev(enc[i+1]);	
		out = (enc[i] << (2*(i%4)+2)) + (enc[i+1] >> (4-2*(i%4)));
		printf("%c",(char)out);		
		if(i%4 == 2)
			enc[i+2] = rev(enc[(++i)+1]);
		i++;	
	}
}


int main(void)
{
	char enc[400]={0},input[300]={0};
	printf("enter a text to encode (max 300 EN) = ");
	gets(input);
	
	// printf("%c ", (char)(b64_table[rev(input[0])])); // test for the reverser function
	
	encode(input,enc);
	decode(enc);
	
	return 0;
}
