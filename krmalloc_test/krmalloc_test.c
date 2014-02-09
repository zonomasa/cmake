#include <stdio.h>
#include <stdlib.h>
extern void *krmalloc_init(void *start, size_t sz);
extern void *krmalloc(size_t sz);

int
main(int argc, char *argv[])
{
    void *pool;
    void *ptr;

    pool = malloc(sizeof(char) * 1024);

    krmalloc_init(pool,sizeof(char) * 1024);

    ptr = krmalloc(sizeof(int) * 10);
    if (ptr == NULL)
    {
        fprintf(stderr, "can not allocate memory\n");
        return -1;
    }

    krfree(ptr);

    return 0;
}
