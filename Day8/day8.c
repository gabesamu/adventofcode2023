#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int position;
    char text[512];
} Line;

typedef struct {
    char name[4];
    char left[4];
    char right[4];
} Node;

 void grab_word(char dst[4], Line *line) {
    char word[4];
    int i = 0;
    while (isalpha(line->text[line->position])) {
        word[i++] = line->text[line->position++];
    }
    word[i] = '\0';
    strcpy(dst, word);
}

void parse_node(Node *node, Line *line) {
    while(!isalpha(line->text[line->position])) line->position++;
    grab_word(node->name, line);
    while(!isalpha(line->text[line->position])) line->position++;
    grab_word(node->left, line);
    while(!isalpha(line->text[line->position])) line->position++;
    grab_word(node->right, line);

}

Node *find_node(Node *nodes, int num_nodes, char name[4]) {
    for(int i = 0; i < num_nodes; i++) {
        if (strcmp(nodes[i].name, name) == 0) return &nodes[i];
    }
    return NULL;
}

int walk_path(Node *nodes, int num_nodes, char *path, Node **current_node) {
    int steps = 0;

    while (current_node != NULL && path[steps] != '\n') {
        if (path[steps] == 'L') {
            *current_node = find_node(nodes, num_nodes, (*current_node)->left);
        } else {
            *current_node = find_node(nodes, num_nodes, (*current_node)->right);
        }
        steps++;
    }
    return steps;
}

long long lcm(int nums[6]) {
    long long lcm = nums[0];
    for (int i = 1; i < 6; i++) {
        long long a = lcm;
        long long b = nums[i];
        while (a != b) {
            if (a < b) {
                a += lcm;
            } else {
                b += nums[i];
            }
        }
        lcm = a;
    }
    return lcm;
}

void part1_script() {
    Node nodes[800];
    char directions[512];
    Line line;

    FILE *fp = fopen("input.txt", "r");

    fgets(directions, 512, fp);
    fgets(line.text, 512, fp); // skip empty line
    int num_nodes = 0;
    while (fgets(line.text, 512, fp) != NULL) {
        line.position = 0;
        parse_node(&nodes[num_nodes++], &line);
    }
    fclose(fp);

    int steps = 0;
    Node *current_node = find_node(nodes, num_nodes, "AAA");
    while (strcmp(current_node->name, "ZZZ") != 0) {
        steps += walk_path(nodes, num_nodes, directions, &current_node);
    }
    printf("Steps: %d\n", steps);
}

int main() {
    Node nodes[800];
    char directions[512];
    Line line;

    FILE *fp = fopen("input.txt", "r");

    fgets(directions, 512, fp);
    fgets(line.text, 512, fp); // skip empty line
    int num_nodes = 0;
    while (fgets(line.text, 512, fp) != NULL) {
        line.position = 0;
        parse_node(&nodes[num_nodes++], &line);
    }
    fclose(fp);

    int num_start_nodes = 0;
    Node *current_nodes[6];
    int steps_taken[6];
    for (int i = 0; i < num_nodes; i++) {
        if (nodes[i].name[2] == 'A') {
            current_nodes[num_start_nodes++] = &nodes[i];
        }
    }
    for (int i = 0; i < num_start_nodes; i++) {
        int steps = 0;
        Node *current_node = current_nodes[i];
        while (current_node->name[2] != 'Z') {
            steps += walk_path(nodes, num_nodes, directions, &current_node);
        }
        printf("Steps: %d\n", steps);
        steps_taken[i] = steps;
    }

    printf("Part 2: %lld\n", lcm(steps_taken));

    return 0;
}
