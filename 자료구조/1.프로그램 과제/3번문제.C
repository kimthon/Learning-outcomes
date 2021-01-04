#include<stdio.h>
int main()
{
    int i;

    printf("*조건\n");
    printf("-자동차는 1,2번 주유기를 번갈아 들어간다.\n");
    printf("-두 주유기는 같은 알고리즘으로 움직인다.\n\n");

    printf("1.자동차가 들어온다\n");
    start:
    printf("좌측 차수가 우측의 차수보다 많은가?(y/n)");
    fflush(stdin);
    scanf("%c",&i);

    if (i=='y')
    {
        printf("우측 안내...\n");
    }
    else if(i=='n')
    {
        printf("좌측 안내\n");
    }
    else{
        printf("잘못된 입력입니다.\n");
        printf("다시 입력해주십시오.\n");
        goto start;
    }

    Wating_Line:
    printf("주유차례입니까?(y/n)");
    fflush(stdin);
    scanf("%c",&i);

    if (i=='y')
    {
        printf("주유한다.\n");
    }
    else if(i=='n')
    {
        time:

        printf("앞자리가 비었는가?(y/n)");
        fflush(stdin);
        scanf("%c",&i);

            if (i=='y')
        {
            printf("앞자리로 이동\n");
        }
            else if(i=='n');
            else
         {
             printf("잘못된 입력입니다.\n");
             printf("다시 입력해주십시오.\n");
            goto time;
        }
        printf("대기중....\n");
        goto Wating_Line;
    }
    else{
        printf("잘못된 입력입니다.\n");
        printf("다시 입력해주십시오.\n");
        goto Wating_Line;
    }
    return 0;
}
