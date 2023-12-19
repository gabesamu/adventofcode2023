#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int position;
    char text[512];
} Line;

int add_digit(int num, int digit) {
    return (num * 10) + digit;
}

int parse_number(Line *line) {
    int num = 0;
    while (isdigit(line->text[line->position])) {
        num = add_digit(num, line->text[line->position] - '0');
        line->position++;
    }
    return num;
}

// Parses ID from a line
int parse_id(Line *line) {
    int id = 0;
    while (line->text[line->position] != ':') {
        char ch = line->text[line->position];
        if (isdigit(ch)) id = parse_number(line);
        else line->position++;
    }
    return id;
}

// Parses entire game from a line (returns 1 if valid game, 0 if invalid)
int check_if_valid(Line *line) {
    int curr_int = 0;
    while (line->text[line->position] != '\n') {
        char ch = line->text[line->position];
        if (isdigit(ch)) {
            curr_int = parse_number(line);
        }
        else if ((ch == 'r' && curr_int > 12) || (ch == 'g' && curr_int > 13) || (ch == 'b' && curr_int > 14)) {
            return 0;
        }
        if (ch == 'r' || ch == 'g' || ch == 'b') {
            curr_int = 0;
        }
        line->position++;
    }
    return 1;
}

// Parses game from a line and calculates "power" of game
int calculate_power_of_game(Line *line) {
    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;
    int curr_int = 0;

    while (line->text[line->position] != '\n') {
        char ch = line->text[line->position];
        if (isdigit(ch)) {
            curr_int = parse_number(line);
        }
        else if (ch == 'r') {
            if(curr_int > max_red) max_red = curr_int;
            line->position += 2;
            curr_int = 0;
        }
        else if (ch == 'g') {
            if(curr_int > max_green)max_green = curr_int;
            line->position += 4;
            curr_int = 0;
        }
        else if (ch == 'b') {
            if(curr_int > max_blue)max_blue = curr_int;
            line->position += 3;
            curr_int = 0;
        }
        else line->position++;
    }

    return max_red * max_green * max_blue;
}

void part1_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, sizeof(line.text), fp)) {
        line.position = 0;
        int id = parse_id(&line);
        if (check_if_valid(&line)) {
            sum += id;
        }
    }

    printf("part 1: %d\n", sum);
    fclose(fp);
}

void part2_script(){
    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, sizeof(line.text), fp)) {
        line.position = 0;
        parse_id(&line);
        sum += calculate_power_of_game(&line);
    }

    printf("part 2: %d\n", sum);
    fclose(fp);
}

int main() {
    part1_script();
    part2_script();
    return 0;
}
