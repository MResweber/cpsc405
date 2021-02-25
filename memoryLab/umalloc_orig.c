#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.
// see https://www.cs.princeton.edu/courses/archive/fall07/cos217/lectures/14Memory-2x2.pdf for charts.

typedef long Align;

// union of stuct s and Align x ensures allocation on 8-byte boundary
union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header; // allows using Header instead of union header

static Header base;
static Header *freep; // points to circular linked list of free memory

void
free(void *ap) // ap points to user data, not Header
{
  Header *bp, *p;

  bp = (Header*)ap - 1; // bp points to Header of block with user data
  // find block in front of bp
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

// Called when more memory is needed for malloc's block
// memory used to be core memory
static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header)); // get block from OS
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1)); // add this big block to our free list
  return freep; 
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits; // # of Header's needed to hold user's bytes + header

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;

  if((prevp = freep) == 0){ // we have not allocated our big block
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr; // remove from free list
      else {
        p->s.size -= nunits;     // too big so adjust size
        p += p->s.size;          // p points to block to return
        p->s.size = nunits;      // block has nunits - amount requested
      }
      freep = prevp;             // update freep - circular linked list, can be any Header block
      return (void*)(p + 1);     // return pointer to user data
    }
    if(p == freep)               // Executed on first call to malloc and when morecore needed
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
