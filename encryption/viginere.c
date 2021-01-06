#include <stdio.h>

int main(int argc, char **argv)
{
	char keyword[256]; //keyword must be lowercase
	char plain_text[5096],enc;
	char freq[26]={0}; //C99 -- reset to array or use memset(freq,0,26)
	
	if(argc >= 2)
		printf("keyword is = %s", argv[1]);
	else
	{
		printf("please give a keyword = ");
		gets(keyword);
	}
	
	int i=0,klen;
	
	// check keyword is lowercase and find length of keyword
	while(keyword[i]!='\0')
	{
		if((keyword[i]>64) && (keyword[i]<91))
			keyword[i]+=32;
		else if((keyword[i]<97) || (keyword[i] > 122))
		{	
			printf("0"); // 0 for unknown
			i++;
		    continue;
		}
		i++;
	}
	
	klen=i;
	i=0;
	printf("Give a text to encryption :\n");
	gets(plain_text);	
	printf("Encrypted text:\n");
	
	// exact enc part
	while(plain_text[i]!='\0')	
	{
		if((plain_text[i]>64) && (plain_text[i]<91))
			plain_text[i]+=32;
		else if((plain_text[i]<97) || (plain_text[i] > 122))
		{	
			printf(" "); // space for unknown
			i++;
	    	continue;
		}
		
		enc = ((plain_text[i]-97+keyword[i%klen])%26)+97;
		printf("%c", enc);	
		freq[enc-97]++;
		i++;
	}
	
	// analyzing...
	printf("\nfingerprint frequency :\n");
	for(i=0;i<26;i++) 
		printf("%c ",(char)(97+i));
	printf("\n");
	
	klen=26;
	while(klen){
		klen=26;
		for(i=0;i<26;i++) 
		{
			if(!freq[i])
			{			
				klen--;
				printf("  ");
				continue;
			}
			printf("* ", freq[i]--);
		}
		printf("\n");
	}
	
	return 0;
}
