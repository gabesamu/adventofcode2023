#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


int add_digit(int num, int digit) {
    return (num * 10) + digit;
}

typedef struct {
    int position;
    char text[150];
} Line;

int winning[10];

bool search_winning(int num) {
    for(int i = 0; i < 10; i++) {
        if(winning[i] == num) return true;
    }
    return false;
}

void parse_winning_numbers(Line *line) {
    while(line->text[line->position] != ':') line->position++;

    int winning_idx = 0;
    while(line->text[line->position] != '|') {
        if(isdigit(line->text[line->position])) {
            int num = 0;
            while(isdigit(line->text[line->position])) {
                num = add_digit(num, line->text[line->position] - '0');
                line->position++;
            }
            winning[winning_idx] = num;
            winning_idx++;
        }
        else line->position++;
    }
}

int check_scratchcard_numbers(Line *line) {
    int winning = 0;
    while(line->text[line->position] != '\n') {
        if(isdigit(line->text[line->position])) {
            int num = 0;
            while(isdigit(line->text[line->position])) {
                num = add_digit(num, line->text[line->position] - '0');
                line->position++;
            }
            if(search_winning(num)) winning++;
        }
        else line->position++;
    }
    return winning;
}

int calculate_worth(int winning) {
    int worth = 0;
    for (int i = 0; i < winning; i++) {
        if (worth == 0) worth = 1;
        else worth *= 2;
    }

    return worth;
}

void create_copies(int *card_amount, int card_idx, int copies) {
    int num_copies = card_amount[card_idx];
    for(int i = 0; i < copies; i++) {
        if (card_idx == 192) return;
        card_idx++;
        card_amount[card_idx] += num_copies;
    }
}

void part1_script() {
    FILE *file = fopen("input.txt", "r");
    Line line;
    int points = 0;

    while(fgets(line.text, sizeof(line.text), file) != NULL) {
        line.position = 0;
        parse_winning_numbers(&line);
        int winning = check_scratchcard_numbers(&line);
        points += calculate_worth(winning);
    }
    printf("Part 1 points: %d\n", points);
    fclose(file);
}

void part2_script() {
    FILE *file = fopen("input.txt", "r");
    Line line;
    long long int total_cards = 0;
    int card_amount[192];
    for (int i = 0; i < 192; i++) card_amount[i] = 1;
    int card_idx = 0;

    while(fgets(line.text, sizeof(line.text), file) != NULL) {
        line.position = 0;
        parse_winning_numbers(&line);
        int winning = check_scratchcard_numbers(&line);
        total_cards += card_amount[card_idx];
        create_copies(card_amount, card_idx, winning);
        card_idx++;
    }

    printf("Part 2 cards: %lli\n", total_cards);

    fclose(file);
}

int main() {
    part1_script();
    part2_script();
    return 0;
}
