#ifndef RBTREE_H
#define RBTREE_H

#include <stdint.h>

typedef enum Color {
    RED,
    BLACK
} Color;

typedef struct Data {
    void *buffer;
    uint32_t buffer_type;
} Data;

typedef struct RBNode {
    Data data;
    Color color;
    struct RBNode *parent;
    struct RBNode *left;
    struct RBNode *right;
} RBNode;

typedef struct RBTree {
    RBNode *root;
    uint32_t size;
} RBTree;

typedef int (CMP)(Data *src, Data *dest);

RBNode *rbnode_new(Data *data);
RBTree *rbtree_new();

void left_rotate(RBTree *tree, RBNode *node);   // 左旋
void right_rotate(RBTree *tree, RBNode *node);  // 右旋

RBNode *precursor(RBNode *node);  // 前驱结点(小于当前结点的最大值)
RBNode *successor(RBNode *node);  // 后继绩点(大于当前节点的最小值)

void rbtree_insert(RBTree *tree, RBNode *node, CMP *cmp);
void fix_after_insert(RBTree *tree, RBNode *node);

void set_color(RBNode *node, Color color);

Color color_of(RBNode *node);
RBNode *left_of(RBNode *node);
RBNode *right_of(RBNode *node);
RBNode *parent_of(RBNode *node);

#endif
