#include "include/rbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

Data *data_new(void *buffer, int buffer_type) {
    Data *new_data = malloc(sizeof(Data));
    if (NULL == new_data) {
        die("malloc new_data");
    }

    new_data->buffer = malloc(buffer_type);
    if (NULL == new_data->buffer) {
        die("malloc new_data buffer");
    }

    memcpy(new_data->buffer, buffer, buffer_type);
    new_data->buffer_type = buffer_type;

    return new_data;
}

RBNode *rbnode_new(Data *data) {
    RBNode *new_node = malloc(sizeof(RBNode));
    if (NULL == new_node) {
        die("malloc new_node");
    }

    new_node->data.buffer = malloc(data->buffer_type);
    if (NULL == new_node->data.buffer) {
        free(new_node);
        die("malloc new_node data");
    }
    new_node->data.buffer_type = data->buffer_type;
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
    // printf("before precursor\n");
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
            RBNode *c = node;       // 当前结点
            RBNode *p = c->parent;  // 当前结点的 parent 结点
            while (p && c == p->left) {
                c = p;
                p = p->parent;
            }
            // printf("second situation in precursor\n");

            return p;
        }

        return ptr;
    }
    // printf("end of precursor\n");

    return NULL;
}

/*
 * 找后继结点(大于当前结点的最小值)
 */
RBNode *successor(RBNode *node) {
    // printf("before successor\n");
    if (node) {
        RBNode *ptr = node->right;
        if (ptr) {
            while (ptr->left) {
                ptr = ptr->left;
            }
            return ptr;
        } else {
            RBNode *c = node;
            RBNode *p = node->parent;
            while (p && c == p->right) {
                c = p;
                p = p->parent;
            }
            // printf("second situation in successor\n");
            return p;
        }
    }

    // printf("end of successor\n");
    return NULL;
}

RBNode *rbn_search(RBTree *tree, Data *data, CMP *cmp) {
    RBNode *p = tree->root;
    while (p) {
        int ret = cmp(data, &p->data);
        if (ret < 0) {
            p = p->left;
        } else if (ret > 0) {
            p = p->right;
        } else {
            return p;
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
            // printf("before second node cmp\n");
            ret = cmp(&node->data, &p->data);
            // printf("after second node cmp\n");
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

    // printf("before fix_after_insert\n");
    fix_after_insert(tree, node);
    // printf("after fix_after_insert\n");
}

void fix_after_insert(RBTree *tree, RBNode *node) {
    // printf("begin fix_after_insert\n");
    if (!tree || !node) {
        return;
    }
    /* 插入结点的 parent 节点的颜色是红色才需要调整 */
    while (color_of(node->parent) == RED && node != tree->root && node) {
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
    // printf("endof fix_after_insert\n");
}

struct clss {
    int num;
};
void print_data(Data *d) {
    struct clss *c = (struct clss *)(d->buffer);
    printf("%-3d ", c->num);
}
void rbtree_delete(RBTree *tree, Data *data, CMP *cmp) {
    RBNode *node = rbn_search(tree, data, cmp);
    // RBNode *pnode = node;
    if (!node || !tree) return;

    /* 转换成删除前驱结点或后继结点的形式 */
    if (node->left && node->right) {
        RBNode *xnode = precursor(node);  // 获得前驱结点
        xchg_data(xnode, node);
        // void *ret = memmove(node->data.buffer, xnode->data.buffer, xnode->data.buffer_type);
        // if (ret) {
        //     printf("memmove success\n");
        // }
        // print_data(&node->data);
        // printf("\n");
        node = xnode;
        // print_data(&xnode->data);
        // printf("\n");
        // print_data(&pnode->data);
        // printf("\n");
    }

    /* 获取实际删除结点的 child 结点的情况 */
    RBNode *m_node = node->left ? node->left : node->right;

    /* 前驱或者后继节点有 child 节点 */
    if (m_node) {
        m_node->parent = node->parent;
        if (node == tree->root) {
            tree->root = m_node;
        } else {
            if (node == node->parent->left) {
                node->parent->left = m_node;
            } else {
                node->parent->right = m_node;
            }
        }

        free_rbnode(node);
        fix_after_delete(tree, m_node);
    }
    /* 前驱或者后继节点没有 child 节点 */
    else {
        /* 根节点 */
        if (node == tree->root) {
            tree->root = NULL;
        }
        /* 叶子节点 */
        else {
            fix_after_delete(tree, node);
        }

        if (node->parent != NULL) {
            if (node == node->parent->left) {
                node->parent->left = NULL;
            } else if (node == node->parent->right) {
                node->parent->right = NULL;
            }
        }
        free_rbnode(node);
    }

    tree->size--;
}

void fix_after_delete(RBTree *tree, RBNode *node) {
    if (!tree || !node) return;

    while (node != tree->root && color_of(node) == BLACK) {
        if (node == node->parent->left) {
            /* 获取 sibling 结点 */
            RBNode *sib_node = node->parent->right;
            if (color_of(sib_node) == RED) {
                set_color(sib_node, BLACK);
                set_color(node->parent, RED);
                left_rotate(tree, node->parent);
                sib_node = node->parent->right;
            }

            if (color_of(sib_node->left) == BLACK && color_of(sib_node->right) == BLACK) {
                set_color(sib_node, RED);
                node = node->parent;
            } else {
                if (color_of(sib_node->right) == BLACK) {
                    set_color(sib_node->left, BLACK);
                    set_color(sib_node, RED);
                    right_rotate(tree, sib_node);
                    sib_node = node->parent->right;
                }

                set_color(sib_node, color_of(sib_node->parent));
                set_color(node->parent, BLACK);
                set_color(sib_node->right, BLACK);
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        }

        else {
            RBNode *sib_node = node->parent->left;
            if (color_of(sib_node) == RED) {
                set_color(sib_node, BLACK);
                set_color(node->parent, RED);
                left_rotate(tree, node->parent);
                sib_node = node->parent->left;
            }

            if (color_of(sib_node->left) == BLACK && color_of(sib_node->right) == BLACK) {
                set_color(sib_node, RED);
                node = node->parent;
            } else {
                if (color_of(sib_node->left) == BLACK) {
                    set_color(sib_node->right, BLACK);
                    set_color(sib_node, RED);
                    right_rotate(tree, sib_node);
                    sib_node = node->parent->left;
                }

                set_color(sib_node, color_of(sib_node->parent));
                set_color(node->parent, BLACK);
                set_color(sib_node->left, BLACK);
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }

    set_color(node, BLACK);
}

void rbt_pre_for_each(RBNode *root, PRI *pri) {
    if (root) {
        pri(&root->data);
        rbt_pre_for_each(root->left, pri);
        rbt_pre_for_each(root->right, pri);
    }
}

void rbt_in_for_each(RBNode *root, PRI *pri) {
    if (root) {
        rbt_in_for_each(root->left, pri);
        pri(&root->data);
        rbt_in_for_each(root->right, pri);
    }
}

void rbt_post_for_each(RBNode *root, PRI *pri) {
    if (root) {
        rbt_post_for_each(root->left, pri);
        rbt_post_for_each(root->right, pri);
        pri(&root->data);
    }
}

void rbt_preorder_traversal(RBNode *root, PRI_NODE *pri_node) {
    if (root) {
        pri_node(root);
        rbt_preorder_traversal(root->left, pri_node);
        rbt_preorder_traversal(root->right, pri_node);
    }
}

void rbt_inorder_traversal(RBNode *root, PRI_NODE *pri_node) {
    if (root) {
        rbt_inorder_traversal(root->left, pri_node);
        pri_node(root);
        rbt_inorder_traversal(root->right, pri_node);
    }
}

void rbt_postorder_traversal(RBNode *root, PRI_NODE *pri_node) {
    if (root) {
        rbt_postorder_traversal(root->left, pri_node);
        rbt_postorder_traversal(root->right, pri_node);
        pri_node(root);
    }
}

void free_rbnode(RBNode *node) {
    if (node) {
        node->left = node->right = node->parent = NULL;
        free(node->data.buffer);
        free(node);
    }
}

void set_color(RBNode *node, Color color) {
    if (node) {
        node->color = color;
    }
}

void xchg_data(RBNode *xnode, RBNode *ynode) {
    if (!xnode || !ynode) return;

    RBNode *tx = rbnode_new(&xnode->data);
    RBNode *ty = rbnode_new(&ynode->data);

    if (xnode->data.buffer_type > ynode->data.buffer_type) {
        ynode->data.buffer = realloc(ynode->data.buffer, xnode->data.buffer_type);
        memcpy(ynode->data.buffer + ynode->data.buffer_type, xnode->data.buffer, xnode->data.buffer_type - ynode->data.buffer_type);
        ynode->data.buffer_type = xnode->data.buffer_type;

        xnode->data.buffer = realloc(xnode->data.buffer, ynode->data.buffer_type);
        memcpy(xnode->data.buffer, ty->data.buffer, ty->data.buffer_type);
        xnode->data.buffer_type = ty->data.buffer_type;
    } else if (xnode->data.buffer_type < ynode->data.buffer_type) {
        xnode->data.buffer = realloc(xnode->data.buffer, ynode->data.buffer_type);
        memcpy(xnode->data.buffer + xnode->data.buffer_type, ynode->data.buffer, ynode->data.buffer_type - xnode->data.buffer_type);
        xnode->data.buffer_type = ynode->data.buffer_type;

        ynode->data.buffer = realloc(ynode->data.buffer, xnode->data.buffer_type);
        memcpy(ynode->data.buffer, tx->data.buffer, tx->data.buffer_type);
        ynode->data.buffer_type = tx->data.buffer_type;
    } else {
        memcpy(xnode->data.buffer, ynode->data.buffer, xnode->data.buffer_type);
        memcpy(ynode->data.buffer, tx->data.buffer, tx->data.buffer_type);
    }

    free_rbnode(tx);
    free_rbnode(ty);
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
