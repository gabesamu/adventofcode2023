#include <stdio.h>
#include <ctype.h>


typedef struct {
    int position;
    char text[150];
} Line;


typedef struct {
    int time;
    int record;
} Race;

typedef struct {
    long long time;
    long long record;
} BigRace;

int add_digit(int num, int digit) {
    return (num * 10) + digit;
}

long long add_digit_big(long long num, int digit) {
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

void parse_input_multiple_races(Race *races) {
    Line line;
    FILE *fp = fopen("input.txt", "r");
    int idx = 0;

    //get time line
    fgets(line.text, 150, fp);
    while(line.text[line.position] != '\n') {
        if (isdigit(line.text[line.position])) {
            int num = parse_number(&line);
            races[idx++].time = num;
        }
        else line.position++;

    }

    //get record line
    idx = 0;
    fgets(line.text, 150, fp);
    line.position = 0;
    while(line.text[line.position] != '\n') {
        if (isdigit(line.text[line.position])) {
            int num = parse_number(&line);
            races[idx++].record = num;
        }
        else line.position++;
    }

    fclose(fp);
}

void parse_input_as_one_race(BigRace *race) {
    Line line;
    FILE *fp = fopen("input.txt", "r");
    int idx = 0;
    long long num = 0;

    //get time line
    fgets(line.text, 150, fp);
    while(line.text[line.position] != '\n') {
        if (isdigit(line.text[line.position])) {
            num = add_digit_big(num, line.text[line.position] - '0');
        }
        line.position++;
    }
    race->time = num;

    //get record line
    idx = 0;
    num = 0;
    fgets(line.text, 150, fp);
    line.position = 0;
    while(line.text[line.position] != '\n') {
        if (isdigit(line.text[line.position])) {
            num = add_digit_big(num, line.text[line.position] - '0');
        }
        line.position++;
    }

    race->record = num;
    fclose(fp);
}

int calculate_ways_to_win(Race *race) {
    int time = race->time;
    int wins = 0;
    for (int i = 0; i < time; i++) {
        int distance = i * (time - i);
        if (distance > race->record) wins++;
    }

    return wins;
}

int calculate_ways_to_win_big_race(BigRace *race) {
    long long time = race->time;
    int wins = 0;
    for (int i = 0; i < time; i++) {
        long long distance = i * (time - i);
        if (distance > race->record) wins++;
    }

    return wins;
}

void part1_script() {
    int total = 1;
    Race races[4];

    parse_input_multiple_races(races);

    for (int i = 0; i < 4; i++) {
        total *= calculate_ways_to_win(&races[i]);
    }
    printf("Part 1: %d\n", total);
}

void part2_script() {
    BigRace race;
    parse_input_as_one_race(&race);
    printf("time:   %lld\n", race.time);
    printf("record:   %lld\n", race.record);
    printf("Part 2: %d\n", calculate_ways_to_win_big_race(&race));

}

int main() {
    part1_script();
    part2_script();
    return 0;
}
