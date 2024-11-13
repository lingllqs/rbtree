#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rbtree.h"

#define N 15

struct cls {
    int num;
};

void print(Data *d) {
    struct cls *c = (struct cls *)(d->buffer);
    printf("%-3d ", c->num);
}

void print_node(RBNode *node) {
    struct cls *buffer = (struct cls *)(node->data.buffer);
    if (node->color == RED) {
        printf("\x1b[;41m%-3d\x1b[0m ", buffer->num);
    } else {
        printf("\x1b[;40m%-3d\x1b[0m ", buffer->num);
    }
}

int mycmp(Data *s, Data *d) {
    int x = ((struct cls *)(s->buffer))->num;
    int y = ((struct cls *)(d->buffer))->num;
    // printf("x: %d y: %d \n", x, y);
    return x - y;
}

int main() {
    RBTree *rbtree = rbt_rbtree_new();

    struct cls stu[N];
    for (int i = 0; i < N; i++) {
        stu[i].num = i + 1;
    }

    // RBNode *rbnode[N];

    // Data data;
    // data.buffer_type = sizeof(struct cls);
    // data.buffer = malloc(data.buffer_type);

    printf("插入:\n");
    for (int i = 0; i < N; i++) {
        // memcpy(data.buffer, &stu[i], sizeof(struct cls));
        // rbnode[i] = rbt_rbnode_new(&data);
        // rbt_insert_node(rbtree, rbnode[i], mycmp);
        Data *data2 = rbt_data_new(&stu[i], sizeof(struct cls));
        rbt_insert_data(rbtree, data2, mycmp);
        rbt_free_data(data2);
    }

    putchar(10);

    struct cls del_stu = {.num = 7};
    Data del_data;
    del_data.buffer_type = sizeof(struct cls);
    del_data.buffer = malloc(del_data.buffer_type);
    memcpy(del_data.buffer, &del_stu, sizeof(struct cls));

    // RBNode *ser = rbn_search(rbtree, &del_data, mycmp);
    // if (ser) {
    //     RBNode *ps = precursor(ser);
    //     RBNode *ss = successor(ser);
    //     printf("-- ");
    //     if (ps) print(&ps->data);
    //     if (ps) print(&ss->data);
    //     printf("--\n");
    // }

    // printf("删除:\n");
    // rbt_delete(rbtree, &del_data, mycmp);
    // printf("del_stu.num = %d\n", del_stu.num);

    printf("前序遍历: ");
    rbt_preorder_traversal(rbtree->root, print_node);
    putchar(10);
    printf("中序遍历: ");
    rbt_inorder_traversal(rbtree->root, print_node);
    putchar(10);
    printf("后序遍历: ");
    rbt_postorder_traversal(rbtree->root, print_node);
    putchar(10);
    printf("层序遍历: \n");
    rbt_levelorder_traversal(rbtree, print_node);
    putchar(10);

    printf("Depth of rbtree: %d\n", rbt_depth(rbtree->root));

    // free(data.buffer);
    free(del_data.buffer);

    rbt_delete_tree(rbtree);

    return 0;
}
