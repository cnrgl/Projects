#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#define M 7//satır
#define N 10//sütun
int main()
{ 
   char in1,in2;
     #ifdef debug
     #undef M
     #undef N
     
     int M,N;
     scanf("%d %d",&M,&N);
  
     int x,y;
     char **Lab;
     Lab=malloc(M*sizeof(char*));
     for(x=0;x<M;x++)
     Lab[x]=malloc(N*sizeof(char));
     for(x=0;x<M;x++)
	 for(y=0;y<N;y++)
     scanf("%c",Lab+x*y);
	 #else
	 char Lab[M][N]=
	{{'0','0','1','0','P','0','e','0','p','0'},
	 {'1','0','p','e','1','E','1','0','1','0'},
	 {'1','G','1','0','0','0','1','0','0','K'},
	 {'0','0','P','0','p','0','0','0','1','0'},
	 {'0','1','1','0','K','C','1','0','1','0'},
	 {'0','0','e','0','1','0','p','0','0','0'},
	 {'0','1','p','0','1','K','E','p','1','0'}
	 };
    #endif
	int i,j,e=0,p=0,h,w;//gerekli değişkenler tanımlandı
	for(h=0;h<M;h++)
	for(w=0;w<N;w++)
	{//Giriş bulundu
		if(Lab[h][w]=='G')
		{
			i=h;
			j=w;
			break;
			break;
		}
	}
	system("title Karsit Madde Labirenti");
	system("color 0e");
	char in;//kontrol tutucu değişken

	while(!(Lab[i][j]=='C')&&!(Lab[i][j]=='K'))//main loop
{
/////////////////////////////////////////////////////////	
///////////////////grafik bölümü/////////////////////////
	printf("%c",218);
	for(w=0;w<N-1;w++)
	{
	printf("%c",196);
	printf("%c",194);
}
printf("%c",196);
printf("%c\n",191);
/////////////////////orta/////////////////////////
	for(h=0;h<M-1;h++){
		for(w=0;w<N;w++)
		{
			printf("%c",179);
				if((h==i)&&(w==j)){
					printf("\033[1;31m");
				printf("%c",'X');
			printf("\033[0m");
			}
				else
			printf("%c",Lab[h][w]);        
		}
		printf("%c",179);
    printf("\n%c",195);
	printf("%c",196);
	for(w=0;w<N-1;w++){
	printf("%c",197);
	printf("%c",196);		
}
	printf("%c\n",180);
}
	for(w=0;w<N;w++)
		{
			printf("%c",179);
	if((i==h)&&(j==w)){
		printf("\033[1;31m");
			printf("%c",'X');
			printf("\033[0m");
	}
		else{
	    	 if(Lab[h][w]==1)
			printf("1");
			else if(Lab[h][w]==0)
			printf("0");
			else
			{
			printf("%c",Lab[h][w]);
        }
		}}
		printf("%c",179);
///////////////alt///////////////////////////  	
   printf("\n%c",192);
   for(w=0;w<N-1;w++){
   printf("%c",196);
printf("%c",193);
}
printf("%c",196);
   printf("%c", 217);
////////////////.....grafik bölümü son....///////////////////////////
/////////////////////////////////////////////////////
		in=getch();
		if(in==27)//escape 
		{
			printf("\n oyundan cikilsin mi?(e)(secenekler-s)");
			in=getch();
			if(in=='e')
			break;
			///////////////sil////////////////////////////
			else if(in=='s')
			{
				printf("\nsecenek gir...(color-r(0~6),line adding-a)");
				in=getch();
				switch(in)
				{
					case 'r':
						printf("colors->red(r),blue(b),gray");
						scanf("%c%c",&in1,&in2);
						printf("girilen %c%c",(char)in1+(char)in2);
						sleep(2);
						system("color");
						/*switch(in)
						{
							case 'k':
						printf("\n033[0;3m");	
				}*/
					break;
					case 'a':
					
					break;	
				}
			}
			///////////////sil///////////////////
			system("cls");
			continue;
	}
	if(kbhit()){//tuş kontrolü
		in=getch();
		if(in==75)//sol
		{
			if(!(Lab[i][j-1]=='1')&&(j>0))
			{
			j-=1;
			Lab[i][j+1]='0';
		}
		}
		else if(in==72)//yukarı
		{
			if(!(Lab[i-1][j]=='1')&&(i>0))
			{
				i-=1;
				Lab[i+1][j]='0';
			}
		}
		else if(in==77)//sağ
		{
			if(!(Lab[i][j+1]=='1')&&(j<N-1))
			{
	        j+=1;	
	        Lab[i][j-1]='0';
			}
		}
		else if(in==80)//aşağı
		{
			if(!(Lab[i+1][j]=='1')&&(i<M-1))
			{
            i+=1;
            Lab[i-1][j]='0';
			}
		}
		else
		{
			printf("\n yanlis tus girildi...\n");
			sleep(2);
		}
	}
		//bulunulan konumdaki (varsa)proton ve elektron toplanır...
		if((Lab[i][j]=='P'))p++;
		else if((Lab[i][j]=='p'))p--;
		else if((Lab[i][j]=='E'))e--;
		else if((Lab[i][j]=='e'))e++;
		sleep(0.5);//resolution
		system("cls");
		//puanlama
			if((e<0)&&(p<0))
	{
		 if(e>p)
		 h=e;
		 else 
		 h=p;
		 printf("toplanan=%d\n",-h);
}
		else if((e>0)&&(p>0))
		 {
		 if(e>p)
          h=p;
		 else 
		   h=e;
		   printf("toplanan=%d\n",h);
	}
	printf("elektron=%d proton=%d\n",e,p);
	}
if((Lab[i][j]=='C'))//c çıkışı için
{
printf("kazandiniz...(toplanan=%d)",h);

}
else if(Lab[i][j]=='K')//karadelik için
{
	printf("BOOM!!!");
}
	return 0;
}
