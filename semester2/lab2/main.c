#include <stdio.h>
#include "b_tree.h"

int main()
{
    b_tree t = {0};

    initTree(&t, 2);
    insert(&t, 1);
    insert(&t, 0);
    insert(&t, 3);
    insert(&t, 2);
    insert(&t, 4);

    printf("Hello World!\n");
    return 0;
}
