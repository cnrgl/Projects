/*
	Name: Ahmet Caner Gül
	Author: Caner
	Date: 11.06.19 19:07
	Description: prime generator
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#define debug1
//#define debugf
//#define debugp
#define g_pf//non recursive form


unsigned long pown(int n,int p)
{
	if(p>0)
	return pown(n,p-1)*n;
	else
	return 1;
}


#ifndef g_pf //fix
int is_prime(unsigned long s,unsigned long n)
{
	#ifdef debugp
	printf("%d remainder %d=%d\n",s,n,s%n);
	#endif
   if(n>2&&(s%n==0))
	return 0;	
   else if(n>2)
	return is_prime(s,n-1);
   else
	return 1;	
}
#endif
#if defined(g_pf)
int is_prime(unsigned long s)
{
	int r=1;
	unsigned long ni = sqrt(s);
	register unsigned long i;
	
	if(s%2==0)r=0;
	for(i=3;i<ni;i=i+2)
	{
		if(s%i==0)r=0;
	}
	return r;
}
#endif


unsigned long up_prime(unsigned long s)
{
	//unsigned long s=sn;
	while(!is_prime(s))s++;//sqrt or divide by 2 is usable !!!
	return s;
}


int main(void)
{
	int s=0,it=0,i,dsize;
	char bufc=NULL,*bufd;
	const char cnt='-';
	system("mode con:cols=30 lines=30");//resize window
	FILE *file=fopen("prime.txt","r");
	
	if(!file){
       	fclose(file);
        file=fopen("prime.txt","a+");
        printf("new file opened with appendix mode...\n");
        fwrite(&cnt,sizeof(char),1,file);
        bufc='2';
        fwrite(&bufc,sizeof(char),1,file);
    
	    if(!file){
        	perror("error while opening\n");
           	return -1;
		}
	}
		
		#ifdef debugf
	    else
	        printf("file opened for reading successed...\n");
	    #endif
	        
	fpos_t fpos;
	fseek(file,-1,SEEK_END);
	fgetpos(file,&fpos);//*************
		
	#ifdef debug1
		printf("pos=%d\n",fpos);///*************
	#endif
		
	//reading last prime from file, read until reached '-' character from end of the file....
	while(++it)
	{
		fread(&bufc,sizeof(bufc),1,file);
		if(bufc==cnt)
			break;
		fgetpos(file,&fpos);
		s+=pown(10,it-1)*atoi(&bufc);//it=1(starting value)
       	#ifdef debug1
		printf("character#%d=%d\n",fpos,atoi(&bufc));
		#endif
		if(fseek(file,-2,SEEK_CUR))
		{
			perror("seek failure...");
			printf("fseekfailure in file %s at line %d\n",__FILE__,__LINE__-5);
			return -1;
		}
	}
	//end reading...
	printf("last prime is=%d\n",s);
	
	fclose(file);
	file=fopen("prime.txt","a+");
	if(!file){
       	perror("error while opening\n");
        return -1;
	}
     
    /// START CALC.  
	printf("input an iteration count=");
	scanf("%d",&it);
	time_t start,end;//calculation time
	time(&start);
    
	bufd=malloc(sizeof(char));
    //writing on the file...
    
	for(i=0;i<it;i++) 
	{
		fwrite(&cnt,sizeof(char),1,file);
             
	        s = up_prime(s);
    		printf("prime#%d=%d\n",i+1,s);//writing the current prime
    	
		//write number into file	
		dsize=(int)log10(s)+1;
		bufd=realloc(bufd,dsize*sizeof(char));
        	itoa(s,bufd,10);//radix=10 for decimal...
			   
		fwrite(bufd,dsize*sizeof(char),1,file);
		s++;//one plus of last prime is our first number
	}
		
	time(&end);
		 
	printf("\nwriting has done...in %.2lf seconds ",difftime(end,start));
	getch();
		 
    	fclose(file);
	return 0;
}
