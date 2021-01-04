#include<stdio.h>
int main()
{
    char i;
    printf(" *조건\n");
    printf(" -신호등은 적색 → 청색 → 청색깜빡이 → 적색\n\n");

    printf("1.적색신호를 발견\n");
    printf("기다리는 중.....\n");
    Wating:
    printf("청색 신호인가?(y/n)");
    fflush(stdin);
    scanf("%c",&i);
     if (i=='y')
    {
        printf("건너는중....\n");
    }
    else if(i=='n')
    {
        printf("기다리는 중....\n");
        goto Wating;
    }
    else{
        printf("잘못된 입력입니다.1\n");
        printf("다시 입력해주십시오.\n");
        goto Wating;
    }
    Blink:
    printf("횡단보도가 깜박이는가?(y/n)");
    fflush(stdin);
    scanf("%c",&i);
    if(i=='y')
    {
        Re:
        printf("깜박이는 시점에 난 절반 이상을 건너왔는가?(y/n)");
        fflush(stdin);
        scanf("%c",&i);
            if (i=='y')
        {
            printf("건너는중....\n");
        }
        else if(i=='n')
        {
            printf("되돌아가는중....\n");
            goto Wating;
        }
        else{
            printf("잘못된 입력입니다.\n");
            printf("다시 입력해주십시오.\n");
            goto Re;
        }

    }
    else{
            printf("잘못된 입력입니다.\n");
            printf("다시 입력해주십시오.\n");
            goto Blink;
        }
    printf("무사 횡단!\n");
    return 0;

}
