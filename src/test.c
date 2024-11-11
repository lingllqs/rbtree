#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/rbtree.h"

#define N 10

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
    RBTree *rbtree = rbtree_new();

    struct cls stu[N];
    for (int i = 0; i < N; i++) {
        stu[i].num = i + 1;
    }
    // for (int i = 0; i < N; i++) {
    //     printf("stu[%d].num = %d\n", i, stu[i].num);
    // }

    RBNode *rbnode[N];

    Data data;
    data.buffer_type = sizeof(struct cls);
    data.buffer = malloc(data.buffer_type);

    printf("插入:\n");
    for (int i = 0; i < N; i++) {
        memcpy(data.buffer, &stu[i], sizeof(struct cls));
        rbnode[i] = rbnode_new(&data);
        rbtree_insert(rbtree, rbnode[i], mycmp);
    }

    // rbt_pre_for_each(rbtree->root, print);
    // putchar(10);
    rbt_in_for_each(rbtree->root, print);
    putchar(10);
    // rbt_post_for_each(rbtree->root, print);
    // putchar(10);
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

    printf("删除:\n");
    rbtree_delete(rbtree, &del_data, mycmp);
    printf("del_stu.num = %d\n", del_stu.num);

    // rbt_preorder_traversal(rbtree->root, print_node);
    // putchar(10);
    rbt_inorder_traversal(rbtree->root, print_node);
    putchar(10);
    // rbt_postorder_traversal(rbtree->root, print_node);
    // putchar(10);

    free(data.buffer);
    free(del_data.buffer);

    return 0;
}
