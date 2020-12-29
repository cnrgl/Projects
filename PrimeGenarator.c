/*
	Name: Ahmet Caner Gül
	Copyright: 
	Author: Caner
	Date: 11.06.19 19:07
	Description: prime genarator
*/
// Program will create new text file for writing the genarated primes if there is no such a same named file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define debug1
//#define debugf
//#define debugp
#define g_pf//non recursive form
//#define l_num_stack//stack rd
unsigned long pow(int n,int p)
{
	if(p>0)
	return pow(n,p-1)*n;
	else
	return 1;
}
#ifdef l_num_stack
#define s 6
int _remain(int *num,int *rem)//TAMAMLANMADI...................
{
	int r,n1,l1=sizeof(num)/sizeof(int);
	for(i=0;i<l1;i++)
	{
		if(i%s==0)n1++;//delimater
	num[n1-1]+=pow(10,i)*num[i];
	}
	for(i=0;i<n1;i++){
	for(j=0;j<n2;j++)
	{
		r=num[i]%
		//bölüneni bölenin her bir parçasına(j) böleriz...
	}
}
}
#endif
#ifndef g_pf
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
#if defined(g_pf)&&!defined(l_num_stack)
int is_prime(unsigned long s,unsigned long ni)
{
	int r=1;
	unsigned long i;
	if(s%2==0)r=0;
	for(i=3;i<ni;i=i+2)
	{
		if(s%i==0)r=0;
		#ifdef debugp
		printf("%d remainder %d=%d\n",s,i,s%i);
		#endif
	}
	return r;
}
#endif
unsigned long *image;//prime images
int size;
unsigned long up_prime(unsigned long sn,int it)
{
	unsigned long s=sn;//it is the number of prime we want to find
	if(it)
	{
	while(!is_prime(s,s/2))s++;
		printf("prime#%d=%d\n",it,s);//writing the current prime
			image[size-it]=s;
			s++;//is number we check 
	return up_prime(s,--it)+1;
}
return it;
}
int main()
{
	int s=0,it=0,i,dsize;
	char bufc=NULL,*bufd;
	const char cnt='-';
	system("mode con:cols=30 lines=30");//resize window
	FILE *fil=fopen("prime.txt","r");
	    if(!fil){
        	fclose(fil);
        	fil=fopen("prime.txt","a+");
        	printf("new file opened with appendix mode...\n");
        	fwrite(&cnt,sizeof(char),1,fil);
        	bufc='2';
        	fwrite(&bufc,sizeof(char),1,fil);
        	if(!fil){
        	perror("error while opening\n");
            return -1;}
			}
			#ifdef debugf
	    else
	        printf("file opened for reading successed...\n");
	        #endif
	    fpos_t fpos;
	    fseek(fil,-1,SEEK_END);
		fgetpos(fil,&fpos);//*************
		#ifdef debug1
		printf("pos=%d\n",fpos);///*************
		#endif
		//reading last prime from file ,read until reached '-' character....
		while(++it)
		{
		fread(&bufc,sizeof(bufc),1,fil);
		if(bufc==cnt)
		break;
		fgetpos(fil,&fpos);
		s+=pow(10,it-1)*atoi(&bufc);//it=1(starting value)
        #ifdef debug1
		printf("character#%d=%d\n",fpos,atoi(&bufc));
		#endif
			if(fseek(fil,-2,SEEK_CUR))
			{
				perror("seek failure...");
				printf("fseekfailure in file %s at line %d\n",__FILE__,__LINE__-5);
				return -1;
			}
		}
	//end reading...
printf("last prime is=%d\n",s);

fclose(fil);
	fil=fopen("prime.txt","a+");
	    if(!fil){
        	perror("error while opening\n");
            return -1;}
     #ifdef debugf
	    else
	        printf("file opened for writing successed...\n");
      #endif
	printf("input an iteration count=");
	scanf("%d",&it);
	size=it;
	image=(int*)malloc(it*sizeof(int));
	time_t start,end;//calculation time
	time(&start);
    if (up_prime(s,it)==size){
    time(&end);
	printf("iteration has done...\n");}  
	else
	printf("iteration error!!!");
    #ifdef debug1
     for(i=0;i<size;i++)
   printf("s%d=%d\n",size,image[i]);
     #endif
	 bufd=malloc(sizeof(char));//allocation adress with null value 
    //writing on the file...
     for(i=0;i<size;i++) 
	     {
	     	 fwrite(&cnt,sizeof(char),1,fil);
             dsize=(int)log10(image[i])+1;
			 bufd=realloc(bufd,dsize*sizeof(char));
             itoa(image[i],bufd,10);//radix=10 for decimal...
             #ifdef debug1
	          printf("#%d",dsize);
		 	  #endif
		 	  #ifndef debug1
		 	  printf("#");
		 	  #endif
			   fwrite(bufd,dsize*sizeof(char),1,fil);
		 }
		 printf("\nwriting has done...in %.2lf seconds ",difftime(end,start));
		 getch();
    fclose(fil);
	return 0;
}
