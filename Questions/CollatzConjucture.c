#include <stdio.h>
int main(void) {
  int number;
  unsigned long it=0;
  scanf("%d",&number);
  
  while(!(number==1))
  {
    number=number%2==0?number/2:number*3+1; 
    it++; 
    printf("%d\n",number);
  }
  
  printf("it=%d",it);
  return 0;
}
