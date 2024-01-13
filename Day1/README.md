# Day 1: Trebuchet - [problem](https://adventofcode.com/2023/day/1)
[Link to code](https://github.com/gabesamu/adventofcode2023/blob/main/Day1/day1.c)
<br>

## Input Analysis
A file full of lines, each line consists of alphanumric characters with no spaces.


## Part 1 Core Problem
Get the first and last digit character of each line, and combine them into one number. Them sum all the numbers together.


## Solving
The main work to be done here is a linear scan of each line, searching for the first and last digits encountered.
The rest is just getting input and adding numbers.


```c
typedef struct {
    int position;
    char text[150];
} Line;


int add_digit(int num, int digit) {
    return (num * 10) + digit;
}

int find_first_digit(Line *line) {
    while (!isdigit(line->text[line->position])) line->position++;
    return line->text[line->position] - '0';
}

int find_last_digit(Line *line) {
    move_position_to_end(line);
    while (!isdigit(line->text[line->position])) line->position--;
    return line->text[line->position] - '0';


```

### Driver Code
Grab each line, get the first and last digit, add them together, and add that to the running sum.

```c
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

```


## Part 2 Core Problem
Get the first and last number of each line, and combine them into one number. Them sum all the numbers together.
This time the number can be represented as a the word.


## Solving
The work here is the same as the first part, but in order to handle the words, instead of searching for digits, just search for substrings.

In order to search for the substrings in the right position, the last index of the word is needed incase a digit appears in the middle of a word. To do this I wrote two modified versions of stdlib strstr, which return the index of the last character of the substring instead of the first. One starts the search at the front, the other starts from the back. Now with these I just search the line for each substring that can represent a number, returning the corresponding integer.

```c

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

int find_first_num_substr(Line *line) {
    const char* digits[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int ints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int numDigits = 18;

    int first = 150;
    int num = 0;
    for (int i = 0; i < numDigits; i++) {
        int pos = edited_strstr(line->text, digits[i]);
        if(pos != -1 && pos < first) {
            first = pos;
            num = ints[i];
        }
    }
    return num;
}

int find_last_num_substr(Line *line) {
    const char* digits[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int ints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int numDigits = 18;

    int last = 0;
    int num = 0;
    for (int i = 0; i < numDigits; i++) {
        int pos = reverse_strstr(line->text, digits[i]);
        if(pos != -1 && pos > last) {
            last = pos;
            num = ints[i];
        }
    }
    return num;
}
```

### Driver Code
Same as part 1, using the new number finding functions.
```c
void part2_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line;
    int sum = 0;

    while (fgets(line.text, 150, fp) != NULL) {
        line.position = 0;
        int num = 0;

        num = add_digit(num, find_first_num_substr(&line));
        num = add_digit(num, find_last_num_substr(&line));

        sum += num;
    }

    printf("Part 2: %d\n", sum);
}
```
