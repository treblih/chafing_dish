/*
 * =====================================================================================
 *
 *       Filename:  stack.c
 *
 *    Description:  stack manipulation - is_empty; create; free; push; pop
 *
 *        Version:  1.0
 *        Created:  27.02.10
 *       Revision:  
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#include	"stack.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stack_empty
 *  Description:  whether the stack is empty
 * =====================================================================================
 */
int stack_empty(stack_t *stk)
{
	assert(stk);
	return stk->cnt == 0;                                  /* if empty, return 1 */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stack_create
 *  Description:  make a new one, return the start addr
 * =====================================================================================
 */
stack_t *stack_create(int cnt)
{
	stack_t *stk = calloc(1, sizeof (stack_t));
	if (!stk) {
		fprintf(stderr, "Calloc Stack Failure!\n");
		/* exit(EXIT_FAILURE); */
	}
	stk->head = calloc(cnt, sizeof (struct elem));
	stk->tail = stk->ptr = stk->head + cnt;
	/* stk->cnt = 0; */
	return stk;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stack_push
 *  Description:  
 * =====================================================================================
 */
void stack_push(stack_t *stk, struct elem *x)             
{                                                              
	if (stk->ptr == stk->head) {
		/* expand the stack */
		int size = stk->cnt;
		int copy_size = size * sizeof (struct elem);
		struct elem *bak = stk->head;
		stk->head = calloc(size * 2, sizeof (struct elem));
		memcpy((char *)stk + copy_size, bak, copy_size);
		stk->tail = (struct elem *)((char *)stk + copy_size);
		free(bak);
	}
	*--stk->ptr = *x;
	stk->cnt++;                                            /* stack.cnt */
}                                                              

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stack_pop
 *  Description:  return the poped one
 * =====================================================================================
 */
struct elem *stack_pop(stack_t *stk)
{
	if (!stk->cnt) {
		return NULL;
	}
	stk->cnt--;                                            /* stack.cnt */
	return stk->ptr++;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  stack_free
 *  Description:  free stack and all of nodes
 * =====================================================================================
 */
void stack_free(stack_t *stk)
{
	free(stk->head);
	free(stk);
	stk->head = stk->tail = stk->ptr = NULL;
}

void stack_reset(stack_t *stk)
{
	stk->cnt = 0;
	stk->ptr = stk->tail;
}

int stack_cnt(stack_t *stk)
{
	return stk->cnt;
}
