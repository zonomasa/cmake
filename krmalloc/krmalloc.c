#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef union Header {
    struct {
        union Header *nxt;
        unsigned      sz;
    }s;
    double _ALIGN;
} Hdr;

static Hdr *basep;
static Hdr *freep;

void
krmalloc_init(void *start, size_t sz) {
    printf("Call krmalloc_init\n");

    /* create header */
    basep = (Hdr *)(start - (ptrdiff_t)(((unsigned long)start) % sizeof(Hdr)));
    basep->s.nxt = basep + 1;
    basep->s.sz = 0;

    freep = basep->s.nxt;
    freep->s.nxt = basep;
    /* use number of units instead of byte */
    freep->s.sz = (sz - sizeof(Hdr)) / sizeof(Hdr);
}

void *
krmalloc(size_t sz) {
    /* get # of alined units */
    unsigned nunits = (sz + sizeof(Hdr) -1) / sizeof(Hdr) + 1;
    Hdr *prevp = freep;
    Hdr *p;

    printf("Call krmalloc\n");

    for (p = prevp->s.nxt; p->s.sz < nunits; prevp = p, p = p->s.nxt) {
        if (p == freep) return NULL;
    }

    if (p->s.sz == nunits) {
        prevp->s.nxt = p->s.nxt;
    }
    else {
        p->s.sz -= nunits;
        p += p->s.sz;
        p->s.sz = nunits;
    }
    freep = prevp;
    return (void *)(p + 1);
}

void
krfree(void *ptr) {
    Hdr *fp = (Hdr *)ptr -1;
    Hdr *p;

    printf("Call krfree\n");

    for (p = freep; !(p < fp && fp < p->s.nxt); p = p->s.nxt) {
        if (p >= p->s.nxt && (fp > p || fp < p->s.nxt)) break;
    }
    if (fp + fp->s.sz == p->s.nxt) {
        fp->s.sz += p->s.nxt->s.sz;
        fp->s.nxt = p->s.nxt->s.nxt;
    }
    else {
        fp->s.nxt = p->s.nxt;
    }
    if (p + p->s.sz == fp) {
        p->s.sz += fp->s.sz;
        p->s.nxt = fp->s.nxt;
    }
    else {
        p->s.nxt = fp;
    }
    freep = p;
}
