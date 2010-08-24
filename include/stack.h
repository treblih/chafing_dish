/*
 * =====================================================================================
 *
 *       Filename:  stack.h
 *
 *    Description:  define struct stack_n, stack_t
 *
 *        Version:  1.0
 *        Created:  27.02.10 17:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef		STACK_H
#define		STACK_H

typedef struct stack_n {
	float x;
	struct stack_n *link;
} stack_n;

typedef struct stack_t {
	int cnt;
	stack_n *head;
} stack_t;

extern int stack_empty(stack_t *);
extern stack_t *stack_create();
extern void stack_push(stack_t *, float);
extern float stack_pop(stack_t *);
extern void stack_free(stack_t *);

#endif
