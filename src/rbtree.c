#include "include/rbtree.h"

#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

RBNode *node_new(Data *data) {
    RBNode *new_node = malloc(sizeof(RBNode));
    if (NULL == new_node) {
        die("malloc new_node");
    }

    new_node->data.buffer = malloc(data->buffer_type);
    if (NULL == new_node->data.buffer) {
        free(new_node);
        die("malloc new_node data");
    }

    memcpy(new_node->data.buffer, data->buffer, data->buffer_type);
    new_node->left = new_node->right = new_node->parent = NULL;
    new_node->color = RED;

    return new_node;
}

RBTree *rbtree_new() {
    RBTree *new_tree = malloc(sizeof(RBTree));
    if (NULL == new_tree) {
        die("malloc new_tree");
    }

    new_tree->root = NULL;
    new_tree->size = 0;

    return new_tree;
}

/*
 * 总共需要断开三个地方的指针
 *     pp -> @                              pp -> @
 *            \                                    \
 *     node -> @                                    @ <- pr
 *            / \           --------->             / \
 *           @   @ <- pr                  node -> @   @
 *              / \                              / \
 *             @   @                            @   @
 *
 *-------------------------------------------------------------

 *        pp -> @                               pp -> @
 *             /                                     /
 *    node -> @             --------->              @ <- pr
 *           / \                                   / \
 *          @   @ <- pr                   node -> @   @
 *             / \                               / \
 *            @   @                             @   @
 *
 */
void left_rotate(RBTree *tree, RBNode *node) {
    if (node) {
        RBNode *pp = node->parent;
        RBNode *pr = node->right;

        node->right = pr->left;
        if (pr->left) {
            pr->left->parent = node;
        }

        pr->parent = pp;
        if (pp) {
            if (node == pp->left) {
                pp->left = pr;
            } else {
                pp->right = pr;
            }
        } else {
            tree->root = pr;
        }
        node->parent = pr;
        pr->left = node;
    }
}

/*
 * 总共需要断开三个地方的指针
 *       pp -> @                                pp -> @
 *              \                                      \
 *               @ <- node                      node -> @
 *              / \           --------->               / \
 *       pl -> @   @                                  @   @ <- pl
 *            / \                                        / \
 *           @   @                                      @   @
 *
 *----------------------------------------------------------------

 *           pp -> @                               pp -> @
 *                /                                     /
 *               @ <- node    --------->       node -> @
 *              / \                                   / \
 *       pl -> @   @                                 @   @ <- pl
 *            / \                                       / \
 *           @   @                                     @   @
 *
 */
void right_rotate(RBTree *tree, RBNode *node) {
    if (node) {
        RBNode *pp = node->parent;
        RBNode *pl = node->left;

        /* 第一处指针 */
        node->left = pl->right;
        if (pl->right) {
            pl->right->parent = node;
        }

        /* 第二处指针 */
        pl->parent = pp;
        if (pp) {
            if (node == pp->left) {
                pp->left = pl;
            } else {
                pp->right = pl;
            }
        } else {
            tree->root = pl;
        }

        /* 第三处指针 */
        node->parent = pl;
        pl->right = node;
    }
}

/*
 * 找前驱结点(小于当前结点的最大值)
 */
RBNode *precursor(RBNode *node) {
    if (node) {
        /* 当前节点有左孩子 */
        RBNode *ptr = node->left;
        if (ptr) {
            while (ptr->right) {
                ptr = ptr->right;
            }
            return ptr;
        }
        /* 当前节点没有左孩子 */
        else {
            RBNode *current = node;
            RBNode *parent = current->parent;
            while (parent && current == parent->left) {
                current = parent;
                parent = parent->parent;
            }

            return parent;
        }

        return ptr;
    }

    return NULL;
}

/*
 * 找后继结点(大于当前结点的最小值)
 */
RBNode *successor(RBNode *node) {
    if (node) {
        RBNode *ptr = node->right;
        if (ptr) {
            while (ptr->left) {
                ptr = ptr->left;
            }
            return ptr;
        } else {
            RBNode *current = node;
            RBNode *parent = node->parent;
            while (parent && current == parent->right) {
                current = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    return NULL;
}

void rbtree_insert(RBTree *tree, RBNode *node, CMP *cmp) {
    if (!tree || !node) return;

    /* 第一个结点 */
    if (!tree->root) {
        tree->root = node;
    }
    /* 中序为升序插入 */
    else {
        RBNode *p = tree->root;
        RBNode *pp = p;
        int ret;
        while (p) {
            ret = cmp(node->data.buffer, p->data.buffer);
            pp = p;
            if (ret < 0) {
                p = p->left;
            } else {
                p = p->right;
            }
        }

        if (ret < 0) {
            pp->left = node;
        } else {
            pp->right = node;
        }
        node->parent = pp;
    }

    tree->size++;
}

void fix_after_insert(RBTree *tree, RBNode *node) {
    if (!tree || !node) {
        return;
    }
    /* 插入结点的 parent 节点的颜色是红色才需要调整 */
    while (color_of(node->parent) == RED && node != tree->root) {
        /* node 结点的 parent 结点是 grandparent 的 left 结点 */
        if (node->parent == node->parent->parent->left) {
            if (color_of(node->parent->parent->right) == RED) {
                set_color(node->parent, BLACK);
                set_color(node->parent->parent->right, BLACK);
                set_color(node->parent->parent, RED);
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                set_color(node->parent, BLACK);
                set_color(node->parent->parent, RED);
                right_rotate(tree, node->parent->parent);
            }
        }
        /* node 结点的 parent 是 grandparent 的 right 结点 */
        else {
            if (color_of(node->parent->parent->left) == RED) {
                set_color(node->parent, BLACK);
                set_color(node->parent->parent->left, BLACK);
                set_color(node->parent->parent, RED);
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                set_color(node->parent, BLACK);
                set_color(node->parent->parent, RED);
                left_rotate(tree, node->parent->parent);
            }
        }
    }

    set_color(tree->root, BLACK);
}

void set_color(RBNode *node, Color color) {
    if (node) {
        node->color = color;
    }
}

Color color_of(RBNode *node) {
    return (node ? node->color : BLACK);  // 空结点颜色为黑色
}

RBNode *left_of(RBNode *node) {
    return (node ? node->left : NULL);
}

RBNode *right_of(RBNode *node) {
    return (node ? node->right : NULL);
}

RBNode *parent_of(RBNode *node) {
    return (node ? node->parent : NULL);
}
