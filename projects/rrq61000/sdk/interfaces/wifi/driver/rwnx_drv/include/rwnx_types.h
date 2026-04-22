/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef _RWNX_TYPES_H
#define _RWNX_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include "co_int.h"

typedef unsigned int __bitwise gfp_t;

typedef struct {
	int counter;
} atomic_t;

/* Circular doubly linked list node */
struct cdlist_node {
    struct cdlist_node *next;
    struct cdlist_node *prev;
};

struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));
#define rcu_head callback_head


#if !defined(__aligned)
#define __aligned(x)    __attribute__((__aligned__(x)))
#endif

#endif /* _RWNX_TYPES_H */
