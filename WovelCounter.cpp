#include <stdio.h>
#include <string.h>
#define low 32
int main() {
char vowels[5]={'A','E','I','O','U'};
char *st;
int count,i;
//dont enter string which contain spaces
scanf("%s",st);

for(i=0;i<strlen(st);i++)
{
    if((vowels[0]==st[i])||(vowels[1]==st[i])||(vowels[2]==st[i])||(vowels[3]==st[i])||(vowels[4]==st[i]))
   { count++;
    continue;
    }
  else if((vowels[0]+low==st[i])||(vowels[1]+low==st[i])||(vowels[2]+low==st[i])||(vowels[3]+low==st[i])||(vowels[4]+low==st[i]))
    count++;
    
}
printf("%d",count);
    return 0;
}
