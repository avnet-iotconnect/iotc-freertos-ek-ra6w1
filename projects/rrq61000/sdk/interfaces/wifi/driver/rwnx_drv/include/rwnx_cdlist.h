/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef _RWNX_CDLIST_H
#define _RWNX_CDLIST_H

#include "rwnx_types.h"

#define CDLIST_NODE_INIT(x) \
        { .next = &(x), .prev = &(x) }

#define CDLIST_DECLARE(x) \
        struct cdlist_node x = CDLIST_NODE_INIT(x)

static inline void cdlist_init(struct cdlist_node *head)
{
    head->next = head;
    head->prev = head;
}

static inline void cdlist_add_between(struct cdlist_node *new_node,
                                      struct cdlist_node *prev,
                                      struct cdlist_node *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

static inline void cdlist_add(struct cdlist_node *new_node,
                              struct cdlist_node *head)
{
    cdlist_add_between(new_node, head, head->next);
}

static inline void cdlist_del_between(struct cdlist_node *prev,
                                      struct cdlist_node *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void cdlist_del_entry(struct cdlist_node *node)
{
    cdlist_del_between(node->prev, node->next);

    node->next = NULL;
    node->prev = NULL;
}


#define cdlist_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define cdlist_first_entry(head, type, member) \
    cdlist_entry((head)->next, type, member)

#define cdlist_last_entry(head, type, member) \
    cdlist_entry((head)->prev, type, member)

#define cdlist_prev_entry(pos, member) \
    cdlist_entry((pos)->member.prev, __typeof(*(pos)), member)

#define cdlist_for_each_entry(cur, head, member) \
    for (cur = cdlist_entry((head)->next, __typeof(*cur), member); \
         &cur->member != (head); \
         cur = cdlist_entry(cur->member.next, __typeof(*cur), member))

#define cdlist_for_each_entry_safe(cur, n, head, member) \
    for (cur = cdlist_entry((head)->next, __typeof(*cur), member), \
         n   = cdlist_entry(cur->member.next, __typeof(*cur), member); \
         &cur->member != (head); \
         cur = n, n = cdlist_entry(n->member.next, __typeof(*n), member))

static inline void cdlist_del(struct cdlist_node *node)
{
    cdlist_del_entry(node);
}

static inline void cdlist_del_init(struct cdlist_node *node)
{
    cdlist_del_entry(node);
    cdlist_init(node);
}

static inline int cdlist_empty(const struct cdlist_node *head)
{
    return (head->next == head);
}

static inline void cdlist_add_tail(struct cdlist_node *new_node,
                                   struct cdlist_node *head)
{
    cdlist_add_between(new_node, head->prev, head);
}

#endif /* _RWNX_CDLIST_H */
