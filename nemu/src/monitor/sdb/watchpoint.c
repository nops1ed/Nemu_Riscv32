/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
	word_t stored_expr;	
	word_t val;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp() {
	if (!free_) {
		printf("watchpoint: wp allocation failed\n");
		assert(0);
	}
	WP *_tmp;
	for (_tmp = head; _tmp -> next != NULL ; _tmp = _tmp -> next);	
	_tmp -> next = free_;
	free_ = free_ -> next;
	_tmp -> next -> next = NULL;
	return _tmp -> next;
}

void free_wp(WP *wp) {
	if (!head) {
		printf("watchpoint: wp free failed\n");
		return;
	}
	WP *_tmp = head, *_pre = NULL;
	//We should always free the last element of head list to keep the sequence of all WPs
	while(_tmp -> next)
	{
		_tmp = _tmp -> next;
		_pre = _pre ? _pre -> next : head;
	}
	_tmp -> next = free_;
	free_ = _tmp;
	if (!_pre) head = _pre;
	else _pre -> next = NULL;
}
