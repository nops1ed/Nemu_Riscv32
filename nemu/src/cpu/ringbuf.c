#include <cpu/cpu.h>

typedef struct IRingBuffer_Node {
    char _logbuf[128];
    struct IRingBuffer_Node *next;
    bool hasval;
}IRingBuffer_Node;

static IRingBuffer_Node RBN_pool[CONFIG_RSIZE] = {};
/* Maybe not necessary */
/*
//static IRingBuffer_Node *head = NULL, *tail = NULL;
//static uin32_t Cur_Pos = 0;
*/
/* Indicate IRINGBUFFER initialized */
static bool flag = false;
static IRingBuffer_Node *Cur_Pos = NULL;

static void Init_RingBuffer(void) {
    for (int i = 0; i < CONFIG_RSIZE; i++) {
        RBN_pool[i].next = (i == CONFIG_RSIZE- 1 ? &RBN_pool[i + 1]: &RBN_pool[0]);
        RBN_pool[i].hasval = false;
    }
    flag = true;
    Cur_Pos = &RBN_pool[0];
}

/* Always wrap next node */
void Insert_RingBuffer(const char *logbuf) {
    if(!flag) Init_RingBuffer();
    size_t _size = sizeof(logbuf);
    memcpy(Cur_Pos->_logbuf, logbuf, _size);
    Cur_Pos->hasval = true;
    Cur_Pos = Cur_Pos->next;
}

void Display_RingBuffer(void) {
    if(!(Cur_Pos->hasval)) {
        printf("Run Program first\n");
        return;
    }
    IRingBuffer_Node *_tmp = Cur_Pos->next;
    while(!(_tmp->hasval)) _tmp = _tmp->next;
    while(_tmp != Cur_Pos) {
        printf("%s\n", _tmp->_logbuf);
        _tmp = _tmp->next;
    }
    printf("-->%s\n", _tmp->_logbuf);
} 