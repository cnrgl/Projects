#include <stdio.h>
#include "enctools.h" 
#define phi(x) (x-1) // euler phi/totient function reduced for primes form
#define car(n) lcm(n1,n2) 
//#define detail // for detailed output use gcc complier with -D [macro] option to define "detail" macro -- otherwise header detail not seen

typedef struct{
	uint64_t l_priv;
	double d_priv;
}priv;


int main(int argc,char **argv)
{
	
	uint64_t n,c[256]={-1}; // can be use long double
	unsigned long int n1,n2;
	char *file = "./test.txt"; // primes file that contain generated primes in advanced 
	priv d;
	
	int public_exp = 17, i=-1, k=0; // gcd(e, phi(n)) = 1;
	char message[256];
	
	if(argc>1)
		file = argv[1];
		
	srand((unsigned)time(0)); // seed for randomizing process
	
	do{
		n1=get_prime(rand(),file);
		do{n2=get_prime(rand(),file);}while(n2==n1); // find two distinc primes 
		n=n1*n2;
	}while(n>UINT64_MAX);
	
	if(n1<n2)
		swap(n1,n2);
		
	printf("Modul N(HEX) = %d(%X)\n",n,(int)n);
	printf("Public key = %d \n",public_exp);
	printf("enter text for the encryption n(MAX 256) =");	
	gets(message);
	printf("encrypted message is : ");

	while(message[++i]) // read until NULL(0x00)
	{
		// printf("%c",message[i]); // debug 1 
		c[i] = fast_mod(message[i], public_exp, n);
		printf("%d ",c[i]);
	}
	
	printf("\nDecrypt ?(press any key) :");
	getch();
	do{

		if(k>10000){
			printf("\n k exceeded the 10000 !!!\n");
			return -1;
		}

		k++;	
		d.d_priv = ((k*phi(n1)*phi(n2)+1)/(double)public_exp);
		
		d.l_priv=(uint64_t)d.d_priv;
		//printf("%lf ",d.d_priv); // k debug
		//printf(" %d ",d.l_priv);
	
	}while((d.d_priv-d.l_priv));
	
	#ifdef detail
	printf("\nprivate key= %d", d.l_priv);
	printf("\nk=%d",k); // get k
	#endif
	
	printf("\ndecrypted text is = ");
	i=-1;
	while(c[++i])
		printf("%c",(char)(fast_mod(c[i], d.l_priv, n)));	
	
	getch();

	return 0;
}
