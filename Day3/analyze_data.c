#include <stdio.h>



int main() {
    int row_len = 0;
    int col_len = 0;

    for (int c; (c = getchar()) != '\n';) {
        row_len++;
    }
    col_len++;
    for (int c; (c = getchar()) != EOF;) {
        if (c == '\n') {
            col_len++;
        }
    }

    printf("row_len: %d\n", row_len);
    printf("col_len: %d\n", col_len);
}
