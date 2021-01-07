#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	
	char plain_text[5096],enc,keyword;
	char freq[26]={0}; //C99 -- reset to array or use memset(freq,0,26)

	int i=0;
	srand((unsigned)time(0)); // pseudo random generator (time(0) is start time of program) 
	printf("This encryption strategy genaretes a random key for every character\n");
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
		keyword = rand()%26; // one-time key
		enc = ((plain_text[i]-97+keyword)%26)+97;
		printf("%c", enc,keyword);	
		freq[enc-97]++;
		i++;
	}
	
	// analyzing...
	printf("\n\nFingerprint frequency :\n");
	for(i=0;i<26;i++) 
		printf("%c ",(char)(97+i));
	printf("\n");
	
	int klen=26;
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
