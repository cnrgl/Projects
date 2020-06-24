#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//find different members in a random matrix
int main(void)
{
	int mat[4][4],i,j,n=0,A[16],flag=0;

	srand(time(0)); //random seeding 
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			mat[i][j]=rand()%20; //rastgele sayı alınıyor ve mod 20 ile 0-20 arası değerler mat matrixine atanıyor
	
	printf("matris:\n");
	for(i=0;i<4;i++)//matris çıktısı
	{
		for(j=0;j<4;j++)
			printf("%d ",mat[i][j]);		
		printf("\n");	
	}
	
	
	for(i=0;i<16;i++)
		{
		for(j=0;j<16;j++)//16 boyutlu matris için sıralı tarama yapılıyor iki değişkene ihtiyaç yok
			{	
				if(mat[i/4][i%4]==mat[j/4][j%4])//farklı değer kontrolü
					flag++;//aynı bulunursa flag arttırılır.		
			}
				
			if(flag==1)//aynı matris içinde aynı sayıdan en fazla 1 tane olmalı
				{
					A[n]=mat[i/4][i%4];
		    		n++;//farklı değer sayısı arttı
				}
			flag=0;//flag başa çekildi
		}
		      
	printf("Farkli elemanlar:\n");
	for(i=0;i<n;i++)
		printf("%d ",A[i]);			
				
				
	return 0;
}
