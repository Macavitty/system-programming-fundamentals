#include "letters.h"

void reverse_order_algo(char arr[LETTERS_N]){
    for (int i = 0; i < LETTERS_N / 2; i++) {
        char tmp = arr[i];
        arr[i] = arr[LETTERS_N - 1 - i];
        arr[LETTERS_N - 1 - i] = tmp;
    }
}
void change_case_algo(char arr[LETTERS_N]){
    for (int i = 0; i < LETTERS_N; ++i) {
        char c = arr[i];
        if (c >= 'A' && c <= 'Z') {
            arr[i] = (char) (arr[i] + 32);
        } else if (c >= 'a' && c <= 'z') {
            arr[i] = (char) (arr[i] - 32);
        }
    }
}