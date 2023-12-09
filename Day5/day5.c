#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
    int position;
    char text[1024];
} Line;

typedef struct {
    long long start;
    long long end;
} Range;

typedef struct {
    long long dest_start;
    long long source_start;
    long long length;
} MapRange;

typedef struct {
    MapRange map_ranges[50];
} Map;

/*======================  Helper Functions  ======================*/

bool is_map_header(Line *line) {
    while (line->text[line->position] != '\n') {
        if (line->text[line->position] == ':') return true;
        line->position++;
    }
    return false;
}

bool is_empty_line(Line *line) {
    return line->text[line->position] == '\n';
}

bool is_in_map_range(long long num, MapRange range) {
    return num >= range.source_start && num < range.source_start + range.length;
}

bool compare_map_ranges(MapRange range1, MapRange range2) {
    return range1.dest_start == range2.dest_start && range1.source_start == range2.source_start && range1.length == range2.length;
}

/*======================  Parsing and data structure populating  ======================*/

long long parse_number(Line *line) {
    char *end;
    long long num = strtoll(&line->text[line->position], &end, 10);
    line->position += end - &line->text[line->position];
    return num;
}

void parse_seeds(Line *line, long long *seeds) {
    int seed_idx = 0;
    while (line->text[line->position] != '\n') {
        if (isdigit(line->text[line->position])) {
            seeds[seed_idx] = parse_number(line);
            seed_idx++;
        }
        else line->position++;
    }
}

Range parse_seed_range(Line *line) {
    Range range;

    while(!isdigit(line->text[line->position])) line->position++;
    range.start = parse_number(line);

    while(!isdigit(line->text[line->position])) line->position++;
    range.end = parse_number(line) + range.start - 1;
    return range;
}

MapRange parse_map_range(Line *line) {
    MapRange range;

    while(!isdigit(line->text[line->position])) line->position++;
    range.dest_start = parse_number(line);

    while(!isdigit(line->text[line->position])) line->position++;
    range.source_start = parse_number(line);

    while(!isdigit(line->text[line->position])) line->position++;
    range.length = parse_number(line);
    return range;
}

void parse_mapping(FILE *fp, Map *mapping) {
    Line line;
    int range_idx = 0;
    while (fgets(line.text, sizeof(line.text), fp) != NULL) {
        line.position = 0;
        if (is_empty_line(&line)) break;
        mapping->map_ranges[range_idx] = parse_map_range(&line);
        range_idx++;
    }
}

void parse_seed_ranges(Line *line, Range *seed_ranges) {
    int seed_idx = 0;
    while (line->text[line->position] != '\n') {
        if (isdigit(line->text[line->position])) {
            seed_ranges[seed_idx] = parse_seed_range(line);
            seed_idx++;
        }
        else line->position++;
    }
}

/*======================  Mapping functions  ======================*/

MapRange get_map_range(long long num, Map *mapping) {
    for (int i = 0; i < 50; i++) {
        if (mapping->map_ranges[i].length == 0) break;
        if (is_in_map_range(num, mapping->map_ranges[i])) return mapping->map_ranges[i];
    }
    return (MapRange) {0, 0, 0};
}

long long get_dest_num(long long num, Map *mapping) {
    MapRange range = get_map_range(num, mapping);
    return range.dest_start + (num - range.source_start);
}

long long process_seed_to_location(long long seed, Map *mappings) {
    long long num = seed;
    for (int j = 0; j < 7; j++) {
        num = get_dest_num(num, &mappings[j]);
    }
    return num;
}

int apply_map_to_ranges(Map *mapping, Range *ranges, int ranges_size) {
    for(int i = 0; i < ranges_size; i++) {
        Range range = ranges[i];
        MapRange dest_map_range = get_map_range(range.start, mapping);
        if (dest_map_range.length == 0) {
            continue;
        }
        long long source_end = dest_map_range.source_start + dest_map_range.length;



        if (range.end > source_end) {
            ranges[ranges_size] = (Range) {source_end, range.end};
            ranges_size++;
            range.end = source_end;
        }

        ranges[i] = (Range) {dest_map_range.dest_start + (range.start - dest_map_range.source_start), dest_map_range.dest_start + (range.end - dest_map_range.source_start)};

    }

    return ranges_size;
}

/*======================  Debugging Functions  ======================*/

void display_seeds(long long *seeds) {
    for (int i = 0; i < 20; i++) {
        printf("%lld\n", seeds[i]);
    }
}

void display_seed_ranges(Range *seed_ranges) {
    for (int i = 0; i < 10; i++) {
        printf("start: %lld, end: %lld\n", seed_ranges[i].start, seed_ranges[i].end);
    }
}

void display_mapping(Map *mapping) {
    for (int i = 0; i < 50; i++) {
        if(mapping->map_ranges[i].length == 0) break;
        printf("dest_start: %lld, source_start: %lld, length: %lld\n", mapping->map_ranges[i].dest_start, mapping->map_ranges[i].source_start, mapping->map_ranges[i].length);
    }
}

void part1_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line = {0};
    long long seeds[20];
    Map mappings[7];
    memset(mappings, 0, sizeof(mappings));

    fgets(line.text, sizeof(line.text), fp);
    parse_seeds(&line, seeds);

    int map_idx = 0;
    while (fgets(line.text, sizeof(line.text), fp) != NULL) {
        line.position = 0;
        if (is_map_header(&line)) {
            parse_mapping(fp, &mappings[map_idx]);
            map_idx++;
        }
    }

    long long closest_location = __LONG_LONG_MAX__;
    for (int i = 0; i < 20; i++) {
        long long num = process_seed_to_location(seeds[i], mappings);
        if (num < closest_location) closest_location = num;
    }

    printf("Part 1: %lld\n", closest_location);
    fclose(fp);
}

void part2_script() {
    FILE *fp = fopen("input.txt", "r");
    Line line = {0};
    Range seed_ranges[100];
    memset(seed_ranges, 0, sizeof(seed_ranges));
    Map mappings[7];
    memset(mappings, 0, sizeof(mappings));



    fgets(line.text, sizeof(line.text), fp);
    parse_seed_ranges(&line, seed_ranges);

    int map_idx = 0;
    while (fgets(line.text, sizeof(line.text), fp) != NULL) {
        line.position = 0;
        if (is_map_header(&line)) {
            parse_mapping(fp, &mappings[map_idx]);
            map_idx++;
        }
    }

    int ranges_size = 10;

    for(int i = 0; i < 7; i++) {
        ranges_size = apply_map_to_ranges(&mappings[i], seed_ranges, ranges_size);
    }

    long long closest_location = __LONG_LONG_MAX__;
    for (int i = 0; i < ranges_size; i++) {
        Range range = seed_ranges[i];
        if (range.start < closest_location) closest_location = range.start;
    }

    printf("Part 2: %lld\n", closest_location);
    fclose(fp);
}

int main () {
    part1_script();
    part2_script();
    return 0;
}
