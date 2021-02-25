#include <unistd.h>  // sbrk
#include <stdio.h>

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;
typedef unsigned int uint;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void xv6_free(void *ap) {
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header* morecore(uint nu) {
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  xv6_free((void*)(hp + 1));
  return freep;
}

void* xv6_malloc(uint nbytes) {
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}

void print_xv6_malloc() {
  Header *currp = NULL;
  currp = freep;
  for (int i = 0; i < 4; i++) {
    printf("currp: %p, currp+1: %p, currp->size: %d, currp->next: %p\n", currp, currp+1, currp->s.size, currp->s.ptr);
    currp = currp->s.ptr;
    if (currp == freep)
      break;
  }
  /*
  for (currp = freep; currp != NULL; currp = currp->s.ptr) {
    printf("currp: %p, currp->size: %d\n", currp, currp->s.size);
  }
  */
}

int main() {

    printf("sizeof(Header): %lu\n", sizeof(Header));
    int nbytes = 4;
    int nunits = ((nbytes + sizeof(Header) - 1) / sizeof(Header)) + 1;
    printf("nunits: %d\n", nunits);
    

    int *p1 = xv6_malloc(sizeof(int));
    *p1 = 1;
    printf("p1: %p, *p1: %d\n", p1, *p1);

    print_xv6_malloc();

    int *p2 = xv6_malloc(sizeof(int));
    *p2 = 2;
    printf("p2: %p, *p2: %d\n", p2, *p2);

    int *p3 = xv6_malloc(sizeof(int));
    *p3 = 3;
    printf("p3: %p, *p3: %d\n", p3, *p3);

    xv6_free(p1);

    int *p4 = xv6_malloc(sizeof(int));
    *p4 = 4;
    printf("p4: %p, *p4: %d\n", p4, *p4);

    long *lp1 = xv6_malloc(sizeof(long));
    *lp1 = 10;
    printf("lp1: %p, *lp1: %ld\n", lp1, *lp1);

    long *lp2 = xv6_malloc(sizeof(long));
    *lp2 = 20;
    printf("lp2: %p, *lp2: %ld\n", lp2, *lp2);

    long *lp3 = xv6_malloc(sizeof(long));
    *lp3 = 30;
    printf("lp3: %p, *lp3: %ld\n", lp3, *lp3);

    xv6_free(lp1);

    long *lp4 = xv6_malloc(sizeof(long));
    *lp4 = 40;
    printf("lp4: %p, *lp4: %ld\n", lp4, *lp4);

    printf("lp4: %p, lp4[-1]: %ld\n", lp4, lp4[-1]);

    int *p = xv6_malloc(sizeof(int));
    *p = 100;
    for (int i = 0; i < 45; i++)
        printf("p: %p, p[i]: %d\n", p+i, p[i]);


    print_xv6_malloc();
}
