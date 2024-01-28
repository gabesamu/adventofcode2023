# Day 3: Gear Ratios - [problem](https://adventofcode.com/2023/day/3)
<br>

## Input Analysis
The input is a grid of symbols and numbers. A "." symbol is empty space and numbers represent part numbers. The rest of the symbols are treated differently between part 1 and 2. The entire input should be read in and stored to be processed. I just stored the grid as a 2d array.



## Part 1 Core Problem
Add up all the "part numbers" in the grid. A number is a part number if it is adjacent to any symbol, not ".".


## Solving
Once the input is read into a 2d array, scan over the array and when a number is found, check if any of its neighbors are a symbol other than ".". If so, add the number to a running sum.

```c
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

```

## Part 2 Core Problem
Add up the gear ratio of each gear in the grid. A gear is any "*" symbol that is adjacent to exactly two numbers. The gear ratio is the product of the two numbers it is adjacent to.


## Solving
Scan over the grid and when a "*" is found, check if it is adjacent to exactly two numbers. If so, multiply the two numbers together and add to a running sum.
Calculate_gear holds the logic for checking for proper adjacency and calculating the gear ratio. 0 is returned if the gear is not valid.

```c

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

```
