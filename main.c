#include <stdio.h>
#include "BPlusTree.h"

int main() {
    btree_node *root = btree_node_create();

    int arrnum[] = {10, 2, 3, 4, 5, 9, 8, 7, 6, 1};
    for (int i = 0; i < sizeof(arrnum) / sizeof(int); ++i) {
        root = btree_insert(root, arrnum[i]);
    }
    btree_inorder_print(root);
    printf("\n");
    btree_level_display(root);
    printf("Hello, World!\n");
    return 0;
}