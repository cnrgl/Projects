#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
	int mat[4][4],i,j,n=0,A[16],flag=0;

	srand(time(0)); //random seeding 
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			mat[i][j]=rand()%20; //rastgele sayý alýnýyor ve mod 20 ile 0-20 arasý deðerler mat matrixine atanýyor
	
	printf("matris:\n");
	for(i=0;i<4;i++)//matris çýktýsý
	{
		for(j=0;j<4;j++)
			printf("%d ",mat[i][j]);		
		printf("\n");	
	}
	
	
	for(i=0;i<16;i++)
		{
		for(j=0;j<16;j++)//16 boyutlu matris için sýralý tarama yapýlýyor iki deðiþkene ihtiyaç yok
			{	
				if(mat[i/4][i%4]==mat[j/4][j%4])//farklý deðer kontrolü
					flag++;//ayný bulunursa flag arttýrýlýr.		
			}
				
			if(flag==1)//ayný matris içinde ayný sayýdan en fazla 1 tane olmalý
				{
					A[n]=mat[i/4][i%4];
		    		n++;//farklý deðer sayýsý arttý
				}
			flag=0;//flag baþa çekildi
		}
		      
	printf("Farkli elemanlar:\n");
	for(i=0;i<n;i++)
		printf("%d ",A[i]);			
				
				
	return 0;
}

