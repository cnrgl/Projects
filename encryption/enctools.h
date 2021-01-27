#ifndef modular_h 
#define modular_h

#ifndef stdio_h 
#include <stdio.h>
#endif

#include <stdlib.h>
#include <stdint.h>

static uint64_t pown(int n,int p) // just for header usage
{
	if(p>0)
		return pown(n,p-1)*n;
	else
		return 1;
}


static __inline__ uint64_t fast_mod(uint64_t m, uint64_t e, uint64_t n) // for bigger number of mod -- force compiler to make inline
{  
	uint64_t res = 1, mod = m%n;
	
	if(e&0x01)
		res *= mod;
	e = e>>1;
	while(e){
		mod = (mod*mod)%n;
		if(e&0x01)
			res = (res*mod)%n;
		e = e>>1;	 
	}
	return res;
}


uint64_t gcd(uint64_t a, uint64_t b) // must a >= b
{
	uint32_t rem = a%b;
	if(rem)
		gcd(b,rem);
	return b;	
}


uint64_t lcm(uint64_t a, uint64_t b)
{
	unsigned int g = gcd(a,b);
	return (a/g)*(b/g);	
}

uint32_t RPG(int randomness)
{
	
	
	
	
}


void swap(uint64_t *a, uint64_t *b)
{
		register uint64_t temp;
		temp=*a;
		*a=*b;
		*b=temp;
} 
 
 
unsigned long int get_prime(unsigned int randn, const char* file_name) //This function gets random prime from ordered prime list prepeared in advanced
{
	FILE *file= fopen(file_name,"r");
	unsigned long int prime = 0;
	int it=0;
	char bufc=0;
	static int sig=0;
	
	if(!file){
		if(sig)
			printf("WARNING ... prime.txt did not found in %s. Program will use default primes(not recommend)\n", file_name);
		prime = 17+sig;
		return prime;
	}
	
	fpos_t fpos;
	fseek(file,-1,SEEK_END);
	fgetpos(file,&fpos); // get file size
	
	fseek(file,(randn%fpos+3),SEEK_SET); // go to random position (+3) -- for file handling purpose 
	
	// The following block is for file specific
	while(bufc!='-') // set position before the '-' character
		fread(&bufc,sizeof(bufc),1,file);
	
	do	
	{	
		fseek(file,-2,SEEK_CUR);
		fread(&bufc,sizeof(bufc),1,file);
		if(bufc=='-')
			break;	
		prime+=pown(10,it)*(bufc-'0'); // char2digit -- atoi alternative 
	}while(++it);
	// End of the file handling
	#ifdef detail
	printf("prime%d = %d \n",(sig/2)+1,prime);
	#endif
	sig+=2;	
	return prime;
}
#endif
