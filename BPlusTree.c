//
// Created by 陈一超 on 2019/11/28.
//

#include "BPlusTree.h"
#include <stdio.h>

#include <stdlib.h>


int btree_node_num = 0;

btree_node *btree_node_new() {
    btree_node *node = (btree_node *) malloc(sizeof(btree_node));
    if (NULL == node) {
        return NULL;
    }
    for (int i = 0; i < 2 * M - 1; ++i) {
        node->k[i] = 0;
    }
    for (int i = 0; i < 2 * M; ++i) {
        node->p[i] = NULL;
    }
    node->num = 0;
    node->is_leaf = true;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

btree_node *btree_node_create() {
    btree_node *node = btree_node_new();
    if (NULL == node) {
        return NULL;
    }
    node->next = node;
    node->prev = node;
    return node;
}

// split children
int btree_split_child(btree_node *parent, int pos, btree_node *child) {
    btree_node *new_child = btree_node_new();
    if (NULL == new_child) {
        return -1;
    }
    new_child->is_leaf = child->is_leaf;
    new_child->num = M - 1;
    for (int i = 0; i < M - 1; ++i) {
        new_child->k[i] = child->k[i + M];
    }

    if (false == new_child->is_leaf) {
        for (int i = 0; i < M; ++i) {
            new_child->p[i] = child->p[i + M];
        }
    }
    child->num = M - 1;   //？？？？？？？ todo why is m-1 not m?
    if (true == child->is_leaf) {
        child->num++;  //if leaf, keep the middle ele, put it in the left
    }

    for (int i = parent->num; i > pos; --i) {
        parent->p[i + 1] = parent->p[i];
    }
    parent->p[pos + 1] = new_child;
    for (int i = parent->num - 1; i >= pos; --i) {
        parent->k[i + 1] = parent->k[i];
    }
    parent->k[pos] = child->k[M - 1];
    parent->num += 1;

    //update link

    if (true == child->is_leaf) {
        new_child->next = child->next;
        child->next->prev = new_child;
        new_child->prev = child;
        child->next = new_child;
    }
    return 1;
}

//btree_node *btree_split_child(btree_node *parent, int pos, btree_node *child);

btree_node *btree_insert_nonfull(btree_node *node, int target) {
    if (true == node->is_leaf) {
        int pos = node->num;
        while (pos >= 1 && target < node->k[pos - 1]) {
            node->k[pos] = node->k[pos - 1];
            pos--;
        }
        node->k[pos] = target;
        node->num += 1;
        btree_node_num += 1; //todo
    } else {
        int pos = node->num;
        while (pos > 0 && target < node->k[pos - 1]) {
            pos--;
        }
        if (2 * M - 1 == node->p[pos]->num) {
            btree_split_child(node, pos, node->p[pos]);
            if (target > node->k[pos]) {
                pos++;
            }
        }
        btree_insert_nonfull(node->p[pos], target);
    }
}

btree_node *btree_insert(btree_node *root, int target) {
    if (NULL == root) {
        return NULL;
    }
    if (2 * M - 1 == root->num) {
        btree_node *node = btree_node_new();
        if (NULL == node) {
            return root;
        }
        node->is_leaf = false;
        node->p[0] = root;
        btree_split_child(node, 0, root);
        btree_insert_nonfull(node, target);
        return node;
    } else {
        btree_insert_nonfull(root, target);
        return root;
    }

}


//void btree_insert_nonfull(btree_node *node, int target);
void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z);

void btree_delete_nonone(btree_node *root, int target);

int btree_search_successor(btree_node *root);

int btree_search_predecessor(btree_node *root);

void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z);

void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z);

btree_node *btree_delete(btree_node *root, int target);

void btree_inorder_print(btree_node *root) {
    if (NULL != root) {
        btree_inorder_print(root->p[0]);
        for (int i = 0; i < root->num; ++i) {
            printf("%d", root->k[i]);
            btree_inorder_print(root->p[i + 1]);
        }
    }
}


void btree_level_display(btree_node *root){
    // just for simplicty, can't exceed 200 nodes in the tree
    btree_node *queue[200] = {NULL};
    int front = 0;
    int rear = 0;

    queue[rear++] = root;

    while(front < rear) {
        btree_node *node = queue[front++];

        printf("[");
        for(int i = 0; i < node->num; i++) {
            printf("%d ", node->k[i]);
        }
        printf("]");

        for(int i = 0; i <= node->num; i++) {
            if(NULL != node->p[i]) {
                queue[rear++] = node->p[i];
            }
        }
    }
    printf("\n");
}

void Save(btree_node *root);
