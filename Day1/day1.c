#include <stdio.h>
#include <ctype.h>
#include <string.h>



typedef struct {
    int position;
    char text[150];
} Line;

int add_digit(int num, int digit) {
    return (num * 10) + digit;
}


void move_position_to_end(Line *line) {
    while (line->text[line->position] != '\n') line->position++;
}


int find_first_digit(Line *line) {
    while (!isdigit(line->text[line->position])) line->position++;
    return line->text[line->position] - '0';
}

int find_last_digit(Line *line) {
    move_position_to_end(line);
    while (!isdigit(line->text[line->position])) line->position--;
    return line->text[line->position] - '0';
}

// Reverse strstr that returns the index of the last character of the substring
int reverse_strstr(char *str, const char *substr) {
    int str_len = strlen(str);
    int substr_len = strlen(substr);
    for (int i = str_len - substr_len; i >= 0; i--) {
        if (strncmp(&str[i], substr, substr_len) == 0) {
            return i + substr_len;
        }
    }
    return -1;
}

// Edited strstr that returns the index of the last character of the substring
int edited_strstr(char *str, const char *substr) {
    int str_len = strlen(str);
    int substr_len = strlen(substr);
    for (int i = 0; i < str_len - substr_len; i++) {
        if (strncmp(&str[i], substr, substr_len) == 0) {
            return i + substr_len - 1;
        }
    }
    return -1;
}


void part1_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, 150, fp) != NULL) {
        line.position = 0;
        int num = 0;
        num = add_digit(num, find_first_digit(&line));
        num = add_digit(num, find_last_digit(&line));
        sum += num;
    }
    printf("Part 1: %d\n", sum);
    fclose(fp);
}

void part2_script() {
    const char* digits[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int ints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int numDigits = 18;

    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, 150, fp) != NULL) {
        line.position = 0;
        int num = 0;
        int first = 150;
        for (int i = 0; i < numDigits; i++) {
            int pos = edited_strstr(line.text, digits[i]);
            if(pos != -1 && pos < first) {
                first = pos;
                num = ints[i];
            }
        }

        int last = 0;
        int idx = 0;
        for (int i = 0; i < numDigits; i++) {
            int pos = reverse_strstr(line.text, digits[i]);
            if(pos != -1 && pos > last) {
                last = pos;
                idx = i;
            }
        }
        num = add_digit(num, ints[idx]);

        sum += num;
    }

    printf("Part 2: %d\n", sum);
}

int main() {
    part1_script();
    part2_script();
    return 0;
}
