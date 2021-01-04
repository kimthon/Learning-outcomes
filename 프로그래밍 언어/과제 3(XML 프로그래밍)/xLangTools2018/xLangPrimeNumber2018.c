#include<stdio.h>
//Author: Su-Hyun Lee
int main(){
int n;
scanf("%d", &n);
return recursive(n);
}
int recursive(int n){
if (n == 1)
{
return 0;
}
else {
if (prime(n) == 1)
{
printf("%d\n",n);
}
return recursive(n - 1);
}
}
int prime(int i){
int p = 1;
int j = 2;
while ( j < i ) 
{
if (i % j == 0)
{
p = 0;
}
j = j + 1;
}
return p;
}
