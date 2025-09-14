#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct node {
    int id;
    pid_t pid;
    struct node* parent;
    struct node** children;
    int children_count;
    int children_capacity;
} node;

typedef struct {
    node* head;
    int max_id;
    node* current;
} tree;

node* create_node(int id, pid_t pid, node* parent) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->id = id;
    new_node->pid = pid;
    new_node->parent = parent;
    new_node->children = NULL;
    new_node->children_count = 0;
    new_node->children_capacity = 0;
    return new_node;
}

void add_child(node* parent, tree* t) {
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    t->max_id++;
    node* child = create_node(t->max_id, child_pid, parent);

    if (parent->children_count >= parent->children_capacity) {
        parent->children_capacity = parent->children_capacity == 0 ? 2 : parent->children_capacity * 2;
        parent->children = (node**)realloc(parent->children, parent->children_capacity * sizeof(node*));
    }

    parent->children[parent->children_count++] = child;
}

node* find_by_id(node* current, int id) {
    if (current->id == id) {
        return current;
    }
    for (int i = 0; i < current->children_count; i++) {
        node* found = find_by_id(current->children[i], id);
        if (found != NULL) {
            return found;
        }
    }
    return NULL;
}

void ps_command(tree* t, int target_id) {
    node* target = find_by_id(t->head, target_id);
    if (target == NULL) {
        fprintf(stderr, "Node not found\n");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        char pid_str[16];
        snprintf(pid_str, sizeof(pid_str), "%d", target->pid);
        execlp("ps", "ps", "-p", pid_str, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

void free_tree(node* current) {
    for (int i = 0; i < current->children_count; i++) {
        free_tree(current->children[i]);
    }
    free(current->children);
    free(current);
}

int main() {
    tree t;
    t.max_id = 0;
    t.head = create_node(0, getpid(), NULL);
    t.current = t.head;

    for (int i = 0; i < 7; i++) {
        add_child(t.head, &t);
    }

    ps_command(&t, 7);

    for (int i = 0; i < t.head->children_count; i++) {
        waitpid(t.head->children[i]->pid, NULL, 0);
    }

    free_tree(t.head);
    return 0;
}
