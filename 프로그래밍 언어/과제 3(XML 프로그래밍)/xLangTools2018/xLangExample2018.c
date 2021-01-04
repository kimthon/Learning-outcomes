#include<stdio.h>
//Author: Su-Hyun Lee
int main(){
int a = 1;
int sum = 0;
int n;
scanf("%d", &n);
while ( a <= n ) 
{
sum = sum + a;
a = a + 1;
}
printf("%d\n",sum);
return 0;
}
