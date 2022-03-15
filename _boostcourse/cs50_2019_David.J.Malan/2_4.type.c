// 라이브러리 stdio.h 를 참고하고 cs50.h를 링크한다.

#include <stdio.h>

#include <cs50.h>

// 사용자가 입력한 정수 n 이 짝수인지 홀수인지 나머지를 활용하여 확인

int main(void){

    int n = get_int("n == \n");

    // 사용자가 입력한 n을 2로 나누었을 때 나머지가 0일 경우 짝수로 나타낸다.

    if(n % 2 == 0){

        printf("even\n");

    }
    // 사용자가 입력한 n을 2로 나누었을 때 나머지가 0이 아닐 경우 홀수로 나타낸다.

else{

        printf("odd\n");

    }

}