//
// Created by chaya on 12/31/2023.
//
#include <stdio.h>

int main(void) {
    for (int i = 0; i <6; i++)
        printf("%c %c %c\n", '0'+i, 'A'+i, 'a'+i);
    for (int i = 6; i <= 9; i++)
        printf("%c\n", '0'+i);
}//end main