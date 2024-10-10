#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//struct of the circle, contains point and radius
struct circle {
    int x, y;
    int radius;
};

//struct of the point, contains x and y
struct point {
    int x, y;
};

//get points from file and save in array. increase capacity by 2 to get smaller time complexity.
struct point* get_points(char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    
    int cap = 1;
    struct point *points = malloc(cap * sizeof(struct point));
    *size = 0;
    int x, y;

    while (fscanf(file, "%d %d", &x, &y) == 2) {
        if (*size >= cap) {
            cap *= 2;
            points = realloc(points, cap * sizeof(struct point));
        }
        points[*size].x = x;
        points[*size].y = y;
        (*size)++;
    }
    points = realloc(points, *size * sizeof(struct point));
    fclose(file);
    return points;
}

//calculate if point is in radius of circle. using pythagorean theorem
bool is_in_radius(struct point point, struct circle circle) {
    return (point.x - circle.x) * (point.x - circle.x) + (point.y - circle.y) * (point.y - circle.y) <= circle.radius * circle.radius;
}

//main function
int main(int argc, char *argv[]) {

    if (argc != 2) {
        return 1;
    }


    int points_size;
    struct point *points = get_points(argv[1], &points_size);
    if (points == NULL) {
        return 1;
    }

    struct circle circle;

    while(scanf("%d %d %d", &circle.x, &circle.y, &circle.radius) == 3) {
        int count = 0;
        for (int i = 0; i < points_size; i++) {
            if (is_in_radius(points[i], circle)) {
                count++;
            }
        }
        printf("%d\n", count);
    }

    free(points);
    return 0;
}
