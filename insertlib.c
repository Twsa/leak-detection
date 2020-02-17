/*
* 插装库的设计
* 编译出插装库：gcc -shared -fpic -o insertlib.so insertlib.c
* 将插装库的代码编译进测试代码中：gcc test.c -I ./ ./insertlib.so -o test
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "list.h"

struct global_list{
        char *addr;
        int size;
        struct list_head list;
}head, *mylist, *pos, *next, *tmp;
char datetime[64] = {0};
static void get_local_time(void);
/*
* 全局链表的初始化，在使用全局链表需要进行初始化，该初始化指定一个链表头
*/
void global_list_init(void)
{
        INIT_LIST_HEAD(&head.list);
}
/*
* 全局链表的遍历，用来查看全局链表的成员的情况
*/
void global_list_for_each(void)
{
        list_for_each_entry(pos, &head.list, list)
                printf("addr: %x, size: %d\n", mylist->addr, mylist->size);
}
void *mss_malloc(size_t __size, int type, \
                const char *file, const char *func, int line) 
{
        char *ptr = (char *)malloc(__size);
        if (ptr == NULL) {
                printf("malloc failed\n");
                return NULL;
        }else {
                mylist = (struct global_list *)malloc(sizeof(struct global_list));
                mylist->addr = ptr;
                mylist->size = __size;
                list_add(&(mylist->list), &(head.list));
                get_local_time();
                printf("[mss_malloc]>>>file: %s, func: %s, line: %d, \
                        time: %s, addr:%x, size: %d, pid: %d\n", \
                        file, func, line, datetime, ptr, __size, getpid());
                global_list_for_each();
                printf("malloc ok\n");
                return ptr;
        }
}
void *mss_free(void *__ptr, int type, \
                const char *file, const char *func, int line)
{
        if (__ptr == NULL) {
                printf("free failed\n");
                return NULL;
        }else {
                list_for_each_entry_safe(pos, next, &head.list, list) {
                        tmp = list_entry(&pos->list, struct global_list, list); 
                        if(__ptr == tmp->addr) {
                                list_del(&pos->list);
                                free(tmp);
                                tmp = NULL;
                        }
                }
                get_local_time();
                printf("[mss_free]>>>file: %s, func: %s, line: %d, \
                        time: %s, addr:%x, size: %d, pid: %d\n", \
                        file, func, line, datetime, tmp->addr, tmp->size, getpid());
                global_list_for_each();
                free(__ptr);
                printf("free ok\n");
        }
}

static void get_local_time(void)
{
        time_t now;
        struct tm *tm_now;
        
        time(&now);
        tm_now = localtime(&now);
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", tm_now);
}
/*
* 内存泄露检测算法，
* 内存泄露。遍历记录内存申请信息的链表，如果链表
* 不为空，则链表中的每个节点都代表一个泄露的内
*/
void mm_leak_detect(void)
{
        if(list_empty(&head.list) == 0) {
                printf("Memory leaked!\n");
                global_list_for_each();
        }else {
                printf("Memory is ok\n");
        }
}
/*
* 野指针检测算法，
* 遍历记录保存内存申请信息的链表，取出链
* 表中的每个节点，然后在循环体内部判断待检测的指针的地址是否在
* 链表的节点的开始或内部，如果是，就不是野指针，如果是NULL不是野指针
* return:0表示不是野指针；-1表示是一个野指针
*/
int wild_ptr_detect(void *__ptr)
{
        if(__ptr == NULL) {
                 printf("%x is not  a wild pointer!\n",__ptr);
                 return 0;
        }
        list_for_each_entry(pos, &head.list, list) {
                tmp = list_entry(&pos->list, struct global_list, list); 
                if(tmp == __ptr) {
                        printf("%x is not  a wild pointer!\n",__ptr);
                        return 0;
                }
        }
        printf("%x is a wild pointer!\n",__ptr);
        return -1;
}