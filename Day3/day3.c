#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

int add_digit(int num, int digit) { return (num * 10) + digit; }

bool is_valid(int row, int col) {
  return row >= 0 && row < 140 && col >= 0 && col < 141;
}

int get_num_of_digits(int num) {
  int num_of_digits = 0;
  while (num > 0) {
    num /= 10;
    num_of_digits++;
  }
  return num_of_digits;
}

int get_full_number(char (*grid)[141], int row, int col) {
  int full_number = 0;
  while (is_valid(row, col - 1) && isdigit(grid[row][col - 1])) {
    col--;
  }
  while (is_valid(row, col) && isdigit(grid[row][col])) {
    full_number = add_digit(full_number, grid[row][col] - '0');
    col++;
  }
  return full_number;
}

bool is_adjacent_to_symbol(char (*grid)[141], int row, int col) {
  int row_adj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int col_adj[8] = {1, 1, 1, 0, -1, -1, -1, 0};
  for (int i = 0; i < 8; i++) {
    int adj_row = row + row_adj[i];
    int adj_col = col + col_adj[i];

    char adj_symbol = grid[adj_row][adj_col];
    if (is_valid(adj_row, adj_col) && ispunct(adj_symbol) &&
        adj_symbol != '.') {
      return true;
    }
  }
  return false;
}

bool is_valid_part_number(char (*grid)[141], int row, int col) {
  while (is_valid(row, col) && isdigit(grid[row][col])) {
    if (is_adjacent_to_symbol(grid, row, col)) {
      return true;
    }
    col++;
  }
  return false;
}

// 0: no gear
// any other number: gear ratio
int calculate_gear_ratio(char (*grid)[141], int row, int col) {
  int row_adj[8] = {1, 1, 1, 0, -1, -1, -1, 0};
  int col_adj[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int gear_ratio = 1;
  int adj_numbers = 0;
  int prev_num = 0;

  for (int i = 0; i < 8; i++) {
    int adj_row = row + row_adj[i];
    int adj_col = col + col_adj[i];

    char adj_symbol = grid[adj_row][adj_col];
    if (is_valid(adj_row, adj_col) && isdigit(adj_symbol)) {
      int num = get_full_number(grid, adj_row, adj_col);

      if (num == prev_num)
        continue;
      adj_numbers++;
      if (adj_numbers > 2) {
        return 0;
      }
      prev_num = num;
      gear_ratio *= num;
    }
  }

  return adj_numbers == 2 ? gear_ratio : 0;
}

void read_input_into_grid(char (*grid)[141]) {
  FILE *fp = fopen("input.txt", "r");
  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 141; j++) {
      grid[i][j] = fgetc(fp);
    }
  }
  fclose(fp);
}

void display_grid(char (*grid)[141]) {
  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 141; j++) {
      printf("%c", grid[i][j]);
    }
  }
}

void part1_script() {
  char grid[140][141];
  int part_number_sum = 0;

  read_input_into_grid(grid);

  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 141; j++) {
      if (isdigit(grid[i][j]) && is_valid_part_number(grid, i, j)) {
        int part_number = get_full_number(grid, i, j);
        part_number_sum += part_number;
        j += get_num_of_digits(part_number);
      }
    }
  }

  printf("part 1: %d\n", part_number_sum);
}

void part2_script() {
  char grid[140][141];
  long long int gear_ratio_sum = 0;

  read_input_into_grid(grid);

  for (int i = 0; i < 140; i++) {
    for (int j = 0; j < 141; j++) {
      if (grid[i][j] == '*') {
        int gear = calculate_gear_ratio(grid, i, j);
        gear_ratio_sum += (long long int)gear;
      }
    }
  }

  printf("part 2: %lli\n", gear_ratio_sum);
}

int main() {
  part1_script();

  part2_script();
}
