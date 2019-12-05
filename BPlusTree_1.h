//
// Created by 陈一超 on 2019/12/4.
//

#ifndef BPLUSTREE_BPLUSTREE_1_H
#define BPLUSTREE_BPLUSTREE_1_H

#include <stdbool.h>

#define N 5

typedef struct BPlusTreeNode {
    bool is_leaf;
    int keyNum;
    struct BPlusTreeNode *parent;
    struct BPlusTreeNode *brother;
    struct BPlusTreeNode *rids[N];
    int keys[N];
} btree_node;

btree_node *btree_node_new();

btree_node *btree_node_create();

btree_node *insert(btree_node *root, int target);

btree_node *delete(btree_node *root, int target); // 删除一个符合条件的值

void btree_print(btree_node *root);

#endif //BPLUSTREE_BPLUSTREE_1_H
