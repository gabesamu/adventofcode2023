#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int position;
  char text[150];
} Line;

typedef enum {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
} HandType;

typedef struct {
  int cards[5];
  int bid;
  HandType type;
} Hand;

int part1_map_card_to_int(char card) {
  switch (card) {
  case 'T':
    return 8;
  case 'J':
    return 9;
  case 'Q':
    return 10;
  case 'K':
    return 11;
  case 'A':
    return 12;
  default:
    return card - '2';
  }
}

int add_digit_to_int(int num, int digit) { return (num * 10) + digit; }

void count_cards(int *cards, int *counts) {
  for (int i = 0; i < 5; i++) {
    counts[cards[i]]++;
  }
}

void count_counts(int *counts, int *num_of_counts) {
  for (int i = 0; i <= 5; i++) {
    for (int j = 0; j < 13; j++) {
      if (counts[j] == i)
        num_of_counts[i]++;
    }
  }
}

HandType get_hand_type(int *cards) {
  int counts[13] = {0};
  int num_of_counts[6] = {0};
  count_cards(cards, counts);
  count_counts(counts, num_of_counts);

  if (num_of_counts[5] == 1)
    return FIVE_OF_A_KIND;
  if (num_of_counts[4] == 1)
    return FOUR_OF_A_KIND;
  if (num_of_counts[3] == 1 && num_of_counts[2] == 1)
    return FULL_HOUSE;
  if (num_of_counts[3] == 1)
    return THREE_OF_A_KIND;
  if (num_of_counts[2] == 2)
    return TWO_PAIR;
  if (num_of_counts[2] == 1)
    return ONE_PAIR;
  return HIGH_CARD;
}

void parse_cards(Line *line, int *cards) {
  int idx = 0;
  while (line->text[line->position] != ' ') {
    cards[idx++] = part1_map_card_to_int(line->text[line->position]);
    line->position++;
  }
}

void parse_bid(Line *line, int *bid) {
  line->position++;
  while (isdigit(line->text[line->position])) {
    *bid = add_digit_to_int(*bid, line->text[line->position] - '0');
    line->position++;
  }
}

int parse_input(Hand *hands) {
  Line line;
  FILE *fp = fopen("input.txt", "r");
  int idx = 0;
  while (fgets(line.text, 150, fp) != NULL) {
    line.position = 0;
    parse_cards(&line, hands[idx].cards);
    parse_bid(&line, &hands[idx].bid);
    hands[idx].type = get_hand_type(hands[idx].cards);
    idx++;
  }
  fclose(fp);
  return idx;
}

bool is_lesser_hand(Hand *hand1, Hand *hand2) {
  if (hand1->type < hand2->type)
    return true;
  if (hand1->type > hand2->type)
    return false;
  for (int i = 0; i < 5; i++) {
    if (hand1->cards[i] < hand2->cards[i])
      return true;
    if (hand1->cards[i] > hand2->cards[i])
      return false;
  }
  return false;
}

void sort_hands(Hand *hands, int num_of_hands) {
  for (int i = 0; i < num_of_hands; i++) {
    bool swapped = false;
    for (int j = 0; j < num_of_hands - 1 - i; j++) {
      if (is_lesser_hand(&hands[j], &hands[j + 1])) {
        Hand temp = hands[j];
        hands[j] = hands[j + 1];
        hands[j + 1] = temp;
        swapped = true;
      }
    }
    if (!swapped)
      break;
  }
}

void part1_script() {
  Hand hands[1001];
  memset(hands, 0, sizeof(hands));
  long long sum = 0;
  int num_of_hands = parse_input(hands);
  printf("num of hands: %d\n", num_of_hands);
  sort_hands(hands, num_of_hands);

  int rank = num_of_hands;
  for (int i = 0; i < num_of_hands; i++) {
    int winnings = hands[i].bid * rank--;
    sum += winnings;
  }

  printf("Part 1: %lld\n", sum);
}

int main() {
  part1_script();
  return 0;
}
