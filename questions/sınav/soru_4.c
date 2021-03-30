#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
	int mat[4][4],i,j,n=0,A[16],flag=0;

	srand(time(0)); //random seeding 
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			mat[i][j]=rand()%20; //rastgele say� al�n�yor ve mod 20 ile 0-20 aras� de�erler mat matrixine atan�yor
	
	printf("matris:\n");
	for(i=0;i<4;i++)//matris ��kt�s�
	{
		for(j=0;j<4;j++)
			printf("%d ",mat[i][j]);		
		printf("\n");	
	}
	
	
	for(i=0;i<16;i++)
		{
		for(j=0;j<16;j++)//16 boyutlu matris i�in s�ral� tarama yap�l�yor iki de�i�kene ihtiya� yok
			{	
				if(mat[i/4][i%4]==mat[j/4][j%4])//farkl� de�er kontrol�
					flag++;//ayn� bulunursa flag artt�r�l�r.		
			}
				
			if(flag==1)//ayn� matris i�inde ayn� say�dan en fazla 1 tane olmal�
				{
					A[n]=mat[i/4][i%4];
		    		n++;//farkl� de�er say�s� artt�
				}
			flag=0;//flag ba�a �ekildi
		}
		      
	printf("Farkli elemanlar:\n");
	for(i=0;i<n;i++)
		printf("%d ",A[i]);			
				
				
	return 0;
}

