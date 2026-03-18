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

typedef int(CMP)(Data *src, Data *dest);
typedef void(PRI)(Data *buf);
typedef void(PRI_NODE)(RBNode *node);

Data *rbt_data_new(void *buffer, int buffer_type);
RBNode *rbt_rbnode_new(Data *data);
RBTree *rbt_rbtree_new();

void rbt_left_rotate(RBTree *tree, RBNode *node);   // 左旋
void rbt_right_rotate(RBTree *tree, RBNode *node);  // 右旋

RBNode *rbt_precursor(RBNode *node);  // 前驱结点(小于当前结点的最大值)
RBNode *rbt_successor(RBNode *node);  // 后继绩点(大于当前节点的最小值)
RBNode *rbt_search_node(RBTree *tree, Data *data, CMP *cmp);

void rbt_insert_node(RBTree *tree, RBNode *node, CMP *cmp);
void rbt_insert_data(RBTree *tree, Data *data, CMP *cmp);
void fix_after_insert(RBTree *tree, RBNode *node);
void rbt_delete_data(RBTree *tree, Data *data, CMP *cmp);
void fix_after_delete(RBTree *tree, RBNode *node);

void rbt_preorder_traversal(RBNode *root, PRI_NODE *pri_node);    // 前序遍历
void rbt_inorder_traversal(RBNode *root, PRI_NODE *pri_node);     // 中序遍历
void rbt_postorder_traversal(RBNode *root, PRI_NODE *pri_node);   // 后序遍历
void rbt_levelorder_traversal(RBTree *tree, PRI_NODE *pri_node);  // 层序遍历

void rbt_free_data(Data *data);
void rbt_free_rbnode(RBNode *node);
void rbt_delete_node(RBNode *node);
void rbt_delete_tree(RBTree *tree);

void rbt_set_color(RBNode *node, Color color);
void xchg_data(RBNode *xnode, RBNode *ynode);

Color color_of(RBNode *node);
RBNode *left_of(RBNode *node);
RBNode *right_of(RBNode *node);
RBNode *parent_of(RBNode *node);
uint32_t rbt_depth(RBNode *root);

#endif
