#include<stdio.h>
int main()
{
    int i;

    printf("*조건\n");
    printf("-차량은 직진,좌회전 우회전만 할수있다.\n");
    printf("-차량신호는 적색 → 청색,청색 좌회전 → 적색 순서이다\n\n");

    printf("1.차량이 정지선에 멈춘다.\n");
    start:
    printf("운전 방향이 직진 혹은 좌회전인가?(y/n)");
    fflush(stdin);
    scanf("%c",&i);

    if(i=='y')
    {
        Wating:
        printf("대기한다....\n");
        Re:
        printf("차량신호등이 청색인가?(y/n)");
        fflush(stdin);
        scanf("%c",&i);

        if(i=='y')
        {
            printf("직진 혹은 좌회전 한다.");
        }
        else if (i=='n')
        {
            goto Wating;
        }
        else{
            printf("잘못된 입력입니다.\n");
            printf("다시 입력해주십시오.\n");
            goto Re;
        }
    }
    else if (i=='n')
    {
        printf("우회전 한다.");
    }
    else{
            printf("잘못된 입력입니다.\n");
            printf("다시 입력해주십시오.\n");
            goto start;
    }
    return 0;
}
