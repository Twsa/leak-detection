#include <stdio.h>
#include <stdlib.h>
#include "insertlib.h"
int main()
{
        global_list_init();
        int i = 0;
        char *ptrs[5];
        for ( ; i < 5; i++) {
                ptrs[i] = (char *)malloc(sizeof(char));
        }
        for ( i = 0; i < 5; i++) {
                free(ptrs[i]);
        }
        mm_leak_detect();
        for ( i = 0; i < 5; i++) {
                if(wild_ptr_detect(ptrs[i]) == -1)
                        ptrs[i] = NULL;  
        }
        for ( i = 0; i < 5; i++) {
                wild_ptr_detect(ptrs[i]);
        }  
        return 0;
}