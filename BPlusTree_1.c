//
// Created by 陈一超 on 2019/12/4.
//

#include "BPlusTree_1.h"
#include <stdio.h>
#include <stdlib.h>

void insertKeyAndRidToPage(btree_node *node, int data, btree_node *tempRid, bool insertIfTrue);

void setNodeInfo(btree_node *node, btree_node *brother, btree_node *parent, bool is_leaf, int keyNum);

void copyKeysAndRidsToNode(btree_node *ori_btree_node, const int *key, int num, btree_node **rids);


btree_node *btree_node_new() {
    btree_node *node = (btree_node *) malloc(sizeof(btree_node));
    if (NULL == node) {
        return NULL;
    }
    for (int i = 0; i < N; ++i) {
        node->keys[i] = 0;
    }
    for (int i = 0; i < N; ++i) {
        node->rids[i] = NULL;   // 初始化时 是叶子节点则 为空
    }
    node->keyNum = 0;
    node->is_leaf = 1;
    node->parent = NULL;
    node->brother = NULL;
    return node;
}

btree_node *btree_node_create() {
    btree_node *node = btree_node_new();
    if (NULL == node) {
        return NULL;
    }
    node->parent = node;
    node->brother = node;
    return node;
}

btree_node *insert(btree_node *node, int target) {

    btree_node *tempNode = node;
    while (!tempNode->is_leaf) {
        btree_node tempRid;
        insertKeyAndRidToPage(tempNode, target, &tempRid, false);
        tempNode = &tempRid;
    }
    insertKeyAndRidToPage(tempNode, target, NULL, true);

    while (tempNode->keyNum == N + 1) {   //超过时
        int keyNum = tempNode->keyNum;
        // 进行分裂 操作
        btree_node *newLeafNode = NULL;
        newLeafNode = btree_node_new();
        int divide1 = keyNum >> 1;
        int divide2 = keyNum - divide1;
        if (tempNode->parent == NULL) {  // 分裂节点为 根节点
            //生成新的根节点
            btree_node *newRootNode = btree_node_new();
            setNodeInfo(newRootNode, NULL, NULL, false, 0);  // 设置根节点信息
            setNodeInfo(newLeafNode, tempNode->brother, newRootNode, tempNode->is_leaf, divide2); //设置新分裂的节点信息
            setNodeInfo(tempNode, newLeafNode, newRootNode, tempNode->is_leaf, divide1); //设置原节点信息

            copyKeysAndRidsToNode(newLeafNode, tempNode->keys + divide1 * sizeof(int),
                                  divide2,
                                  tempNode->rids + divide1 * sizeof(btree_node));

            // 下面向根节点插入 字节点的信息
            insertKeyAndRidToPage(newRootNode, tempNode->keys[0], NULL, true);  // 根节点将各个字节点的第一个数据添加到
            insertKeyAndRidToPage(newRootNode, newLeafNode->keys[0], NULL, true);// 另一个

            return newRootNode; // 返回新的根节点
        } else { //  分裂节点不是根节点
            btree_node *parentNode = tempNode->parent;
            setNodeInfo(newLeafNode, tempNode->brother, parentNode, tempNode->is_leaf, divide2);  // todo 第一处不一样 //右兄弟
            setNodeInfo(tempNode, newLeafNode, parentNode, tempNode->is_leaf, divide1);

            copyKeysAndRidsToNode(newLeafNode, tempNode->keys + divide1 * sizeof(int),
                                  divide2,
                                  tempNode->rids + divide1 * sizeof(btree_node));
            insertKeyAndRidToPage(parentNode, newLeafNode->keys[0], NULL, true);

            tempNode = parentNode; // 令 其指向父节点 向上递归
        }
    }
    return node; //返回原根节点
}

void findKeyAndRidForDelete(btree_node *node, int target, btree_node *temp_rid, bool *existence);

void getFromBrother(btree_node *node, int *status);

btree_node *findLeftBrother(btree_node *node);

btree_node *delete(btree_node *root, int target) { // 删除一个符合条件的值
    btree_node *node = root;
    while (!node->is_leaf) {
        bool existence = false;
        btree_node temp_rid;
        findKeyAndRidForDelete(node, target, &temp_rid, &existence);
        if (existence) {
            node = &temp_rid;
        } else {
            printf("warning!!! target is not existence!");
            return root;
        }
    }
    int position = 0;
    int flag = 0;
    for (position = 0; position < node->keyNum; ++position) {
        if (node->keys[position] == target) {
            flag = 1;
            break;
        }
        if (node->keys[position] > target) {
            flag = 0;
            break;
        }
    }
    if (flag == 1) {   // 说明找到了关键字
        for (int i = position; i < node->keyNum - 1; ++i) {
            node->keys[i] = node->keys[i + 1];
        }
        node->keyNum--;
        int keyNum = node->keyNum;
        while (node->parent != NULL) {    //说明不是根节点
            if (keyNum < (N >> 1)) { // B+树的非根节点的分支树必须大于threshold，需要向兄弟节点借一个节点，或者与兄弟节点进行合并
                getFromBrother(node,)
            }

        }
    }
}

void getFromLeft(btree_node *node, btree_node *left_node, int *status);

void getFromRight(btree_node *node, btree_node *right_node, int *status);

void getFromBrother(btree_node *node, int *status) {
    btree_node *left_node = NULL;
    left_node = findLeftBrother(node);
    if (left_node == NULL) { // 左兄弟存在，对左兄弟进行处理
        getFromLeft(node, left_node, status);
    } else {   //只能在右兄弟节点进行处理
        getFromRight(node, node->brother, status);

        // todo
        if (*status == 3) { // 向右节点借一个关键字，后递归修改父节点

        }
    }
}

void getFromLeft(btree_node *node, btree_node *left_node, int *status) {
    if (left_node->keyNum > (N >> 1)) {// 可以借一个
        for (int i = 0; i < node->keyNum; ++i) {
            node->keys[i + 1] = node->keys[i];
            node->rids[i + 1] = node->rids[i];// 指针后移
        }
        node->keys[0] = left_node->keys[left_node->keyNum - 1];
        node->rids[0] = left_node->rids[left_node->keyNum - 1];
        node->keyNum++;
        left_node->keyNum--;

        *status = 1;
    } else {   //说明不能借，进行合并
        int left_key_num = left_node->keyNum;
        int key_num = node->keyNum;
        for (int i = 0; i < key_num; ++i) {
            left_node->keys[left_key_num + i] = node->keys[i];
            left_node->rids[left_key_num + i] = node->rids[i];
            node->keyNum = 0;
            left_node->keyNum = left_key_num + key_num;
            left_node->brother = node->brother;
            *status = 2;
        }
    }
}

void getFromRight(btree_node *node, btree_node *right_node, int *status) {
    int key_num = node->keyNum;
    int right_key_num = right_node->keyNum;
    if (right_node->keyNum > (N >> 1)) {
        node->keys[key_num] = right_node->keys[0];
        node->rids[key_num] = right_node->rids[0];
        key_num++;
        node->keyNum = key_num;
        for (int i = 0; i < right_key_num - 1; ++i) {
            right_node->keys[i] = right_node->keys[i + 1];
            right_node->rids[i] = right_node->rids[i + 1];
        }
        right_key_num--;
        right_node->keyNum = right_key_num;
        *status = 3;
    } else {  //进行合并
        for (int i = 0; i < right_key_num; ++i) {
            node->keys[key_num + i] = right_node->keys[i];
            node->rids[key_num + i] = right_node->rids[i];
        }
        key_num = key_num + right_key_num;
        right_key_num = 0;
        node->keyNum = key_num;
        right_node->keyNum = right_key_num;
        *status = 4;
        node->brother = right_node->brother;  //
    }
}

btree_node *findLeftBrother(btree_node *node) {
    btree_node *left_node;
    btree_node *parent_node = node->parent;
    for (int i = 0;; ++i) {
        btree_node *temp_node = parent_node->rids[i];
        if (temp_node == node) {  //
            if (i != 0) {
                i--;
                left_node = parent_node->rids[i];
            } else {
                left_node = NULL;
            }
            return left_node;
        }
    }
}

void findKeyAndRidForDelete(btree_node *node, int target, btree_node *temp_rid, bool *existence) {
    int position = 0;
    int flag = 0;
    for (; position < node->keyNum; ++position) {
        if (node->keys[position] >= target) {
            if (node->keys[position] == target) {
                flag = 1;
            }
            break;
        }
    }
    if (flag == 1) {
        *existence = true;
        temp_rid = node->rids[position];
        return;
    }
    position--;
    if (position < 0)
        *existence = false;
    else {
        *existence = true;
        temp_rid = node->rids[position];
    }

}

void setNodeInfo(btree_node *node, btree_node *brother, btree_node *parent, bool is_leaf, int keyNum) {
    node->parent = parent;
    node->brother = brother;
    node->is_leaf = is_leaf;
    node->keyNum = keyNum;
}

void copyKeysAndRidsToNode(btree_node *des_btree_node, const int *key, int num, btree_node **rids) {
    for (int i = 0; i < num; ++i) {
        des_btree_node->keys[i] = key[i];
        des_btree_node->rids[i] = rids[i];
    }
}

void insertKeyAndRidToPage(btree_node *node, int data, btree_node *tempRid, bool insertIfTrue) {
    int position = 0;
    for (; position < node->keyNum; ++position) {
        if (node->keys[position] > data) {
            break;
        }
    }
    if (insertIfTrue) {      // 此过程是插入过程
        for (int i = node->keyNum - 1; i >= position; --i) {
            node->keys[i + 1] = node->keys[i];
            node->rids[i + 1] = node->rids[i];
        }
        node->keys[position] = data;
        node->rids[position] = tempRid;    // 此时 rid 为叶子节点 指向真实数据的指针， 此时为数据项的叶号
        node->keyNum++;
    } else {                 // 此过程都在向下寻找要插入的叶子结点， 其为查找过程
        position--;
        if (position < 0) {
            position = 0;
            node->keys[0] = data;                  // 修改所指页面的最小关键字
        }
        tempRid = node->rids[position];  // 向下继续寻找
    }
}
