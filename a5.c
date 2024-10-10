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

//struct of the point tree, contains point and left and right subtree.
struct point_tree {
    struct point point;
    struct point_tree *left;
    struct point_tree *right;
    int height;
};

//Function Declarations
struct point_tree* build_point_tree(char *filename);
struct point_tree* insert_point(struct point_tree *node, struct point point);
void free_point_tree(struct point_tree *head);
bool is_in_radius(struct point point, struct circle circle);
int get_count(struct point_tree *head, struct circle circle);
int max(int a, int b);
int get_height(struct point_tree *node);
int get_balance(struct point_tree *node);
struct point_tree* right_rotate(struct point_tree *y);
struct point_tree* left_rotate(struct point_tree *x);

//find bigger number
int max(int a, int b) {
    return a > b ? a : b;
}

//get height of the node
int get_height(struct point_tree *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

//get balance of the tree
int get_balance(struct point_tree *node) {
    if (node == NULL) {
        return 0;
    }
    return get_height(node->left) - get_height(node->right);
}

//right rotate AVL tree
struct point_tree* right_rotate(struct point_tree *y) {
    struct point_tree *x = y->left;
    struct point_tree *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

//left rotate AVL tree
struct point_tree* left_rotate(struct point_tree *x) {
    struct point_tree *y = x->right;
    struct point_tree *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}


//build point tree from array of points
struct point_tree* build_point_tree(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    struct point_tree *head = NULL;
    int x, y;

    while (fscanf(file, "%d %d", &x, &y) == 2) {
        struct point point = {x, y};
        head = insert_point(head, point);
    }

    fclose(file);
    return head;
}

//insert point in tree
struct point_tree* insert_point(struct point_tree *node, struct point point) {
    if (node == NULL) {
        struct point_tree *new_node = malloc(sizeof(struct point_tree));
        new_node->point = point;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 1;
        return new_node;
    }

    if (point.x < node->point.x) {
        node->left = insert_point(node->left, point);
    } else {
        node->right = insert_point(node->right, point);
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance(node);

    if(balance > 1 && point.x < node->left->point.x) {
        return right_rotate(node);
    }
    if(balance < -1 && point.x > node->right->point.x) {
        return left_rotate(node);
    }
    if(balance > 1 && point.x > node->left->point.x) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if(balance < -1 && point.x < node->right->point.x) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

//free point tree
void free_point_tree(struct point_tree *head) {
    if (head == NULL) {
        return;
    }
    free_point_tree(head->left);
    free_point_tree(head->right);
    free(head);
}

//calculate if point is in radius of circle. using pythagorean theorem
bool is_in_radius(struct point point, struct circle circle) {
    return (point.x - circle.x) * (point.x - circle.x) + (point.y - circle.y) * (point.y - circle.y) <= circle.radius * circle.radius;
}

//get count of points in radius of circle. Do not go further if next x coordinate is not in the range.
int get_count (struct point_tree *head, struct circle circle) {
    if (head == NULL) {
        return 0;
    }

    int count = 0;
    
    int x = head->point.x - circle.x;

    if(x < -circle.radius) {
        count += get_count(head->right, circle);
    } else if(x > circle.radius) {
        count += get_count(head->left, circle);
    } else {
        if(is_in_radius(head->point, circle)) {
            count++;
        }
        count += get_count(head->left, circle);
        count += get_count(head->right, circle);
    }

    return count;
}

//main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    struct point_tree *tree = build_point_tree(argv[1]);
    if(tree == NULL){
        return 1;
    }
    struct circle circle;

    while(scanf("%d %d %d", &circle.x, &circle.y, &circle.radius) == 3) {
        int count = get_count(tree, circle);
        printf("%d\n", count);
    }

    free_point_tree(tree);
    return 0;
}
