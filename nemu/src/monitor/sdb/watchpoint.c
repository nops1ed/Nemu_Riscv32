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

#define WP_BUF_MAX 64

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char stored_expr[WP_BUF_MAX];	
	word_t old_val;
	word_t new_val;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
/* Indicate whether wp_pool is initialized */
static bool flag = false;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* Display all watchpoints */
void display_wp (void) {
	if (!head) {
		printf("\nwatchpoint: No watchpoint\n");
		return;
	}
	WP* _tmp = head;
	printf("%5s %20s %10s\n" , "Num" , "Type" , "What");
	while(_tmp) {
		printf("%5d %20s %10s\n" , _tmp -> NO , "hw watchpoint" , _tmp -> stored_expr);
		_tmp = _tmp -> next;
	}
}

WP* new_wp() {
	if (!free_)	return NULL;
	if (!head) {
		head = free_;
		free_ = free_ -> next;
		head -> next = NULL;
		return head;	
	}
	WP *_tmp;
	for (_tmp = head; _tmp -> next != NULL ; _tmp = _tmp -> next);	
	_tmp -> next = free_;
	free_ = free_ -> next;
	_tmp -> next -> next = NULL;
	return _tmp -> next;
}

/* We should not expose any detail to external users
*  So it is better to locate any node we need to delete in our delete function 
*/
void free_wp(int NO) {
	if (!head) {
		printf("\nwatchpoint: wp free failed\n");
		return;
	}
	
	WP *_tmp = head, *_pre = NULL;
	while(_tmp && _tmp -> NO != NO) {
		_tmp = _tmp -> next;
		_pre = _pre ? _pre -> next : head;
	}
	if (_tmp) {
		printf("\nwatchpoint: No node with NO%2d could be deleted\n" , NO);
		return ;
	}
	_tmp -> next = free_;
	free_ = _tmp;
	if (!_pre) head = _pre;
	else _pre -> next = NULL;
}


int sdb_watchpoint_create(char *s) {
  if (!flag) {
	init_wp_pool();
	puts("Initialize successfully\n");
	flag = true;
  } 
  /* Below code detected whether expression is valid instead of creating wp */
  if (sizeof(s) > WP_BUF_MAX) {
    printf("\nwatchpoint: too long expression\n");
    return -1;
  }
  bool success = true;
  word_t val = expr(s , &success);
  if (!success) {
    printf("\nwatchpoint: Bad expression\n");
    return -1;
  }

  WP* _tmp = new_wp();
  /* Allocation exception */
  if (!_tmp) {
    printf("\nwatchpoint: allocation failed\n");
    return -1;
  }

  /* Initialize the node */
  strncpy(_tmp -> stored_expr , s , strlen(s));
  _tmp -> old_val = val;
  return _tmp -> NO;
}

void sdb_watchpoint_delete (int NO) {
  if (NO > NR_WP || NO < 0) {
    printf("\nValid watchpoint number should between 0 and %d\n" , NR_WP - 1);
    return;
  }
  free_wp(NO);
}

void sdb_watchpoint_display(void) {
  display_wp();
  /* More debug information required */

}