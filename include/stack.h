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

#include	<assert.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

struct elem {
	int id;	/* 4 + '\0' */
	int stocks;
	int qty;
	double price;
	double cost;
	char name[50];
};

typedef struct stack_t {
	int cnt;
	struct elem *head;	/* static */
	struct elem *tail;	/* static */
	struct elem *ptr;	/* dynamic */
} stack_t;

extern int stack_empty(stack_t *);
extern stack_t *stack_create(int);
extern void stack_push(stack_t *, struct elem *);
extern struct elem *stack_pop(stack_t *);
extern void stack_free(stack_t *);
void stack_reset(stack_t *);
extern int stack_cnt(stack_t *);
extern struct elem *stack_get_one(stack_t *, int);
extern void *stack_delete_one(stack_t *, int);

#endif
