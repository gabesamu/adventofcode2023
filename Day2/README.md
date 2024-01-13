# Day 2: Cube Conundrum - [problem](https://adventofcode.com/2023/day/2)


<br>

## Input Analysis
A file full of lines, each line represents a single game.



## Part 1 Core Problem
Check each game to see if its valid, if it is, add its sum to a total and return the total.
A game is valid if the amount of cubes drawn of each color is not above the max amount allowed for that color.


## Solving
Each game is separated into different subsets, which represent different draws. This does not actually matter for finding the solution, however, because all that matters is whether there exists a number of cubes of a color which exceeds the max. If not, then the game is valid.
The work done for solution is just scanning each line checking the cube amounts against their max. Most of the complexity lies in parsing the input properly.


### Data Structures and Parsing
No data structures are needed for this problem either. I use a simple struct to hold a line of input and the current position being looked at.
parse_id scans over the first part of the line, stopping at the colon, returns the id to be added to the running sum.
check_if_valid holds the logic to solve part 1. It scans over the line, parsing the numbers when reached, then checks them against the color after the number. It assumes that the id has already been parsed.

```c

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

int parse_id(Line *line) {
    int id = 0;
    while (line->text[line->position] != ':') {
        char ch = line->text[line->position];
        if (isdigit(ch)) id = parse_number(line);
        else line->position++;
    }
    return id;
}

// Parses entire game from a line and checks validity(returns 1 if valid game, 0 if invalid)
// Must be called Id has been parsed
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

```

### Driver code

Pretty simple justs grabs each line and runs it through the parsing functions. If the game is valid, add its id to the running sum.
```c
void part1_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, sizeof(line.text), fp)) {
        line.position = 0;
        int id = parse_id(&line);
        if(check_if_valid(&line)) sum += id;
    }

    printf("part 1: %d\n", sum);
    fclose(fp);
}
```


## Part 2 Core Problem
part 2 changes the logic of the problem completely. Simplified goal is to find the max num of each color for each game. Then calculate the "power" of the game by multiplying the maxes together. Then sum all the powers together.


## Solving
Again the separation of the game into subsets does not matter. We just need to keep track of the largest number that is on the line for each color.

### Data Structures and Parsing
The only change here is we use a different function than check_if_valid. Also the parsed id is not actually used but the same function can be used just to parse through it.

calculate_power_of_game parses the rest of the line like check_if_valid did, except now we are tracking the max of each color. The maxes are multiplied together and returned.
```c

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

```

### Driver code

Mostly the same as part 1, just add the power of the game to the running sum.
```c
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
```
