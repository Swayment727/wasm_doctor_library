#include <stdlib.h>

struct node {
        int x;
        struct node *next;
};

int
main(void)
{
        struct node node_b = {.x = 2, .next = NULL};
        struct node node_a = {.x = 1, .next = &node_b};

        int x = 0;
        int accumulator = 0;
        struct node *current_node = &node_a;
        while (x <= 2) {
                // null pointer dereference
                accumulator += current_node->x;
                ++x;
                // second null pointer dereference
                current_node = current_node->next;
        }

        return 0;
}
