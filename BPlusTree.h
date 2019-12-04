//
// Created by 陈一超 on 2019/11/28.
//

#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include <stdbool.h>

#define M 2
typedef struct BPlusTreeNode {
    int k[2 * M - 1];
    struct BPlusTreeNode *p[2 * M];
    int num;
    bool is_leaf;
    struct BPlusTreeNode *prev;
    struct BPlusTreeNode *next;
} btree_node;


btree_node *btree_node_new();

btree_node *btree_node_create();

//btree_node *btree_split_child(btree_node *parent, int pos, btree_node *child);

btree_node *btree_insert_nonfull(btree_node *root, int target);

btree_node *btree_insert(btree_node *root, int target);

int btree_split_child(btree_node *parent, int pos, btree_node *child);

//void btree_insert_nonfull(btree_node *node, int target);
void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z);

void btree_delete_nonone(btree_node *root, int target);

int btree_search_successor(btree_node *root);

int btree_search_predecessor(btree_node *root);

void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z);

void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z);

btree_node *btree_delete(btree_node *root, int target);

void btree_inorder_print(btree_node *root);

void btree_level_display(btree_node *root);

void Save(btree_node *root);

#endif //BPLUSTREE_BPLUSTREE_H
