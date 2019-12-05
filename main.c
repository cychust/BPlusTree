#include <stdio.h>
//#include "BPlusTree.h"

#include "BPlusTree_1.h"

int main() {
    btree_node *root = btree_node_create();

//    int arrnum[] = {11, 10, 2, 7, 1, 8, 5, 6, 4, 9, 3};

    int arrnum[] = {20, 10, 3, 23, 25, 11, 30, 29, 12, 7, 4, 1, 24, 9, 2, 13, 6, 5, 21};
//    int arrnum[] = {1, 2, 3, 4, 5, 6};
    for (int i = 0; i < sizeof(arrnum) / sizeof(int); ++i) {
        root = insert(root, arrnum[i]);
    }
    root = delete(root, 7);
//    root = delete(root, 1);
//    root = delete(root, 5);
//    btree_inorder_print(root);
    btree_print(root);
    printf("\n");
//    btree_level_display(root);
    printf("Hello, World!\n");
    return 0;
}