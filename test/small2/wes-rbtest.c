
#ifndef MANUALBOX
#define WILD
#define SAFE
#define TAGGED
#define INDEX
#define SIZED
#define SEQ
#define FSEQ
#define malloc_safe malloc
#define calloc_fseq calloc
#define free_safe   free
#define calloc_rbnode calloc
#else
#define WILD   __attribute__((wild))
#define SAFE   __attribute__((safe))
#define TAGGED __attribute__((tagged))
#define INDEX  __attribute__((index))
#define SIZED  __attribute__((sized))
#define SEQ    __attribute__((seq))
#define FSEQ   __attribute__((fseq))
#endif

/* A special purpose main */
//#include "main.h"
/****** Data sizes *******
* U8  must be an unsigned of 8 bits
* U16 must be an unsigned of 16 bits *
* U32 must be an unsigned of 32 bits *
* U64 must be an unsigned on 64 bits *
* UPOINT must be an unsigned of the size of a pointer
* ALIGN must be the number of bytes the data accesses must be aligned to - 1
*       i.e. it must be 3 for the data to be aligned on a 32-bit boundary. It
*       must be at least 1 (16-bit alignment). It must be <= UPOINT
* ALSHIFT is log2(ALIGN + 1)
* UALIGN is the integer whose size if ALIGN + 1.
*************************/
typedef unsigned long UL;
typedef unsigned char UC;
typedef int BOOL;
#define  TRUE  1
#define  FALSE 0

#define MASK(bitlen) ((1 << (bitlen)) - 1)

#ifdef alpha_UNIX	      /* Both GCC and DEC cc seem to have the same */
#define ALSHIFT  3
#define ALIGN    MASK(ALSHIFT)
#define UALIGN   U64

#define U8     unsigned char
#define S8     char
#define U16    unsigned short
#define S16    short
#define U32    unsigned int
#define S32    int
#define U64    unsigned long
#define S64    long
#define UPOINT U64
#define SPOINT S64
#define DIRLISTSEP ':'
#endif /* alpha_UNIX */

#ifdef x86_WIN32
#define ALSHIFT 2
#define ALIGN   MASK(ALSHIFT)
#define UALIGN  U32

#define U8     unsigned char
#define S8     char
#define U16    unsigned short
#define S16    short
#define U32    unsigned long
#define S32    long
#define UPOINT U32
#define SPOINT S32
 
#define DIRLISTSEP ';'

#ifdef _MSVC  /* MSVC on x86 */
#define U64    unsigned __int64
#define S64    __int64
#endif /* _MSVC */

#ifdef _GNUCC  /* GNU CC on x86 */
#define U64    unsigned long long
#define S64    long long
#endif /* _GNUCC */ 

#endif /* x86_WIN32 */

#ifdef x86_LINUX
#define ALSHIFT 2
#define ALIGN   MASK(ALSHIFT)
#define UALIGN  U32

#define U8     unsigned char
#define S8     char
#define U16    unsigned short
#define S16    short
#define U32    unsigned long
#define S32    long
#define UPOINT U32
#define SPOINT S32
#define U64    unsigned long long
#define S64    long long
#define DIRLISTSEP ':'
#endif /* x86_WIN32 */

#ifdef _DEBUG
#define _ASSERT(be) {if(!(be)){fprintf(stderr,"Assertion failed on line %d in file %s\n", __LINE__, __FILE__);exit(2);}}
#else
#define _ASSERT(be)
#endif

#define ERR_SET         
#define ERR_CHECK(v,stop,prg) 
#define EXIT(v, n)      exit(n)

#define ERRLINE    {fprintf(stderr, "Error at %s(%d):",__FILE__,__LINE__);\
                    ERR_SET;}
#define WARNLINE   {fprintf(stderr, "Warning at %s(%d):",__FILE__,__LINE__);}

#define ERROR0(v, txt)              {ERRLINE;\
                                     fprintf(stderr,txt);EXIT(v,1);}
#define ERROR1(v, txt, d1)          {ERRLINE;\
                                     fprintf(stderr,txt, d1);EXIT(v,1);}

typedef long clock_t;
 clock_t __cdecl clock(void);
 int    __cdecl rand(void);
#define CLOCKS_PER_SEC 1000

#define TIMESTART(clk) {clk=(double)clock();}
#define TIMESTOP(clk)  {clk=1000000.0 * ((double)clock()-(clk))/CLOCKS_PER_SEC;}

void * SAFE  malloc_safe(unsigned int);
void * SAFE  calloc_safe(unsigned int, unsigned int);
void * FSEQ  calloc_fseq(unsigned int, unsigned int);
void   __cdecl free_safe(void * SAFE);

int __cdecl dup(int);
int __cdecl dup2(int, int);
int __cdecl close(int);

struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;

int   __cdecl printf(const char * SAFE, ...);
int   __cdecl fprintf(FILE * SAFE, const char * SAFE, ...);
void  __cdecl exit(int);
int   __cdecl fflush(FILE * SAFE);
#define NULL (void*)0
extern FILE _iob[] SIZED;
#define stdout &_iob[1]
#define stderr &_iob[2]


extern  int   debugMM;      

#define SETDBGOUT   int _stdout; fflush(stdout);_stdout=dup(1);dup2(2,1);
#define RESTOREOUT  fflush(stdout); dup2(_stdout, 1); close(_stdout);
#ifdef _DEBUG
#define IFDEBUG(txt) {if(debug) {SETDBGOUT; txt; RESTOREOUT;}}
#else  /* _DEBUG */
#define IFDEBUG(txt) {;}
#endif  /* _DEBUG */


extern  char* __stackInit;
extern  int   __mmId;
#define STACK_CHECK(category) { char __probe;\
                      long _pDepth = __stackInit - & __probe;\
                      __MM_REPORT("stack", &__probe, _pDepth, category);}

#define STACK_INIT { char __probe;\
                     __stackInit = & __probe; __mmId = 0; }

#define MALLOC(mallocfun, res, err, sz, type, category) {\
       long _sz = (sz);\
       (res) = (type)mallocfun(_sz);\
       if(! (res)) {\
           ERROR0(err, "Cannot malloc\n"); \
       }\
       __MM_REPORT("malloc", (res), _sz, category);}

#define FREE(freefun, res, category) {\
       if(res) {\
        __MM_REPORT("free", (res), 0, category);\
        freefun(res); }}

#define CALLOC(callocfun, res, err, nrelem, sz, type, category) {\
       int _nrelem = (nrelem);\
       long _sz = (sz);\
       (res) = (type)callocfun(_nrelem, _sz);\
       if(! (res)) {\
           ERROR0(err, "Cannot calloc\n"); \
       }\
       __MM_REPORT("malloc", (res), _sz * _nrelem, category);}

#define REALLOC(res, err, sz, type, category) {\
       long _sz = (sz);\
       if((res)) { __MM_REPORT("free", (res), 0, category); }\
       (res) = (type)realloc((res), _sz);\
       if(! (res)) {\
           ERROR0(err, "Cannot realloc\n"); \
       }\
       __MM_REPORT("malloc", (res), _sz, category);}

#define STRDUP(res, err, what, category) {\
       char* _what = what;\
       long _sz = strlen(_what) + 1;\
       (res) = strdup(_what);\
       if(! (res)) {\
           ERROR0(err, "Cannot strdup\n"); \
       }\
       __MM_REPORT("malloc", (res), _sz, category);}
    
#if defined(_DEBUG) || defined(_DEBUGMM)
#define __MM_REPORT(what, where, size, category) {\
       if(debugMM) {\
        SETDBGOUT; \
        printf("*MM%d: %-6s 0x%08lx %08ld %-20s %s:%d\n", \
                __mmId ++, \
                (what), (where), (long)(size), (category),__FILE__,__LINE__);\
        RESTOREOUT; } \
        }
#else
#define __MM_REPORT(what, where, size, category) { }
#endif




//#include "redblack.h"
typedef struct rbNode {
  struct rbNode * SAFE left, * SAFE right, * SAFE parent;
  U32    key;
  U32    color;  // To make the data aligned
  char data[1] SIZED;
} RBNode;

extern void * SAFE calloc_rbnode(unsigned int nrelem, unsigned int size);

/*** KEYS are compared using unsigned comparisons ****/

/* Creates a new RB node. The node has room for some data but nothing is put 
 * in there. The pointer to the data is returned. Start with NULL as an 
 * empty tree */
char * FSEQ insertRB(RBNode * * tree, U32 key, int datalen);


/* Finds a node. Returns a pointer to the data */
char * FSEQ findRB(RBNode * tree, U32 key);

/* Pass freeData=NULL if the data does not contain pointers that need to be 
 * deallocated */
int freeRB(RBNode * tree, int (* freeData)(U32 key, char * FSEQ data));

// A non-recursive scanner for RB trees
#define FORALLRBNODES(tree, donode) {\
 if(tree) {\
  DoLeftChildren:\
    while(tree->left) {\
      tree = tree->left;\
    }\
   DoNode:\
    /* No left child, or have done all the left descendants*/\
    donode;\
    if(tree->right) {\
      tree = tree->right;\
      goto DoLeftChildren;\
    }\
    /* No right child and we have done all the left descendants*/\
    while(tree->parent && tree->parent->right == tree)\
      tree = tree->parent;\
    if(tree->parent) {\
      tree = tree->parent;\
      goto DoNode;\
    }\
 }\
}

/* Some globals that PCC needs */
int error_level, anerror;
void myexit(int n) {
  exit(n);
}
#ifdef _MSVC
#define random rand
#else
/* weimer: not needed: extern int random(void); */
#endif
int __mmId;
int debugMM;
int debug;


#define DATASIZE 16   // This is the size of the data that is reserved in
                      // each node

int main() {
  /* Test hash tables */
  RBNode *t = NULL;
  int i;
  double clk;
  int count = 0;
  int sz;
  
  /* Add and delete random numbers from the hash table */
  TIMESTART(clk);
  for(i=0;i<500000;i++) {
    int k = random() & 0x7FFFL;
    insertRB(& t, k, DATASIZE);
  }
  for(i=0;i<500000;i++) {
    int k = random() & 0x7FFFL;
    void *data = NULL;
    if(findRB(t, k)) {
      count ++;
    }
  }
  sz = 0;
  FORALLRBNODES(t, { sz ++; });
  freeRB(t, NULL);
  TIMESTOP(clk);
  fprintf(stderr, "Hash has %d elements. Found %d times\n",
          sz, count);
  printf("Run hashtest in %8.3lfms\n", clk / 1000.0);
  fprintf(stderr, "Hello\n");
  exit (0);
  return 0;
}


// redblack.c

#define Red   0
#define Black 1


static RBNode *leftRotate(RBNode *r) {
  RBNode *t;
  _ASSERT(r->right);
  t        = r->right;
  r->right = t->left; if(t->left) {t->left->parent = r; }
  
  if(r->parent) {
    if(r->parent->left == r) {
      r->parent->left = t;
    } else {
      r->parent->right = t;
    }
  }
  t->parent = r->parent;
  t->left  = r; r->parent = t;
  return     t;  // like r = t  
}

static RBNode *rightRotate(RBNode *r) {
  RBNode *t;
  _ASSERT(r->left);
  t         = r->left;
  r->left   = t->right; if(t->right) { t->right->parent = r; }
  
  if(r->parent) {
    if(r->parent->left == r) {
      r->parent->left = t;
    } else {
      r->parent->right = t;
    }
  }
  t->parent = r->parent;
  t->right  = r; r->parent = t;
  return     t;
}

static RBNode * fixupRB(RBNode *x);
#ifdef _DEBUG
/* Check the tree and return the black height */
static int checkRBTreeRec(RBNode *tree, U32 minKey, U32 maxKey) {
  int bhl, bhr;
  if(! tree) return 1;
  _ASSERT((! tree->left || tree->left->parent == tree) &&
          (! tree->right || tree->right->parent == tree));
  _ASSERT(tree->key >= minKey && tree->key <= maxKey);
  _ASSERT(tree->color == Red || tree->color == Black);
  _ASSERT(tree->color == Black ||
          ((!tree->left || tree->left->color == Black) &&
           (!tree->right || tree->right->color == Black)));
  bhl = checkRBTreeRec(tree->left, minKey, tree->key);
  bhr = checkRBTreeRec(tree->right, tree->key + 1, maxKey);
  _ASSERT(bhl == bhr);
  return tree->color == Black ? bhl + 1 : bhl;
}

static int checkRBTree(RBNode *tree) {
  _ASSERT(tree->color == Black);
  checkRBTreeRec(tree, 0, (U32)(-1));
  return 1;
}

static int printRBIndent(U32 address) {
  if(address) {
    printf(" ");
    printRBIndent(address >> 1);
    printf("%d", address & 1);
  }
  return 1;
}

static int printRBTree(RBNode *tree, U32 address) {
  printRBIndent(address);
  if(tree) {
    printf(":%s - 0x%08lx\n",
           tree->color == Red ? "Red  " : "Black", tree->key);
    printRBTree(tree->left, address << 1);
    printRBTree(tree->right, (address << 1) + 1);
  } else {
    printf(":NIL\n");
  }
  return 1;
}
#endif

char * FSEQ insertRB(RBNode **tree, U32 key, int datalen) {
  RBNode *x, *t;
  CALLOC(calloc_rbnode,
         x, NULL, 1, sizeof(RBNode) + datalen, RBNode*, "RBNode");
  x->left = NULL;
  x->right = NULL;
  x->parent = NULL;
  x->color = Red;
  x->key   = key;

  // Now insert as if it were a simple binary search tree
  {
    RBNode **p = tree;
    RBNode *parent = NULL;
    while(*p) {               /* We have not reached a NIL */
      parent = *p;
      if(key <= (*p)->key) {
        p = & (*p)->left;
      } else {
        p = & (*p)->right;
      }
    }
    // Now *p = NIL
    *p = x; x->parent = parent;
  }
  t = fixupRB(x);
  if(t->parent == NULL) {
    *tree = t;
  }
  _ASSERT(*tree);
  (*tree)->color = Black;
  //  IFDEBUG(printf("Tree after insert of key=0x%08lx is\n", key);
  //        printRBTree(*tree, 1););
  IFDEBUG(checkRBTree(*tree));
  return & x->data[0];        /* Return the allocated node */
}


static RBNode * fixupRB(RBNode *x) {
  // Now fixup. We know that x is always RED. The root is always Black
  while(x->parent && x->parent->color == Red) {
    RBNode *par  = x->parent;
    RBNode *gpar = par->parent;
    RBNode *uncle;
    _ASSERT(x->color == Red);
    _ASSERT(gpar);  // the root is always black, so we must have a grand par
    _ASSERT(gpar->color == Black);
    if(par == gpar->left) {
      uncle = gpar->right;
      if(uncle && uncle->color == Red) {
      Case1:
        par->color   = Black;
        uncle->color = Black;
        gpar->color  = Red;
        x = gpar;
        continue;
      } else {
        _ASSERT(!uncle || uncle->color == Black);
        if(x == par->right) {
          uncle = x;
          leftRotate(par);
          x   = par;
          par = uncle;
        }
        _ASSERT(x == par->left);
        rightRotate(gpar);
        par->color = Black;
        gpar->color = Red;
        return par;
      }
    } else {
      uncle = gpar->left;
      _ASSERT(par == gpar->right);
      if(uncle && uncle->color == Red) {
        goto Case1;
      } else {
        _ASSERT(! uncle || uncle->color == Black);
        if(x == par->left) {
          uncle = x;
          rightRotate(par);
          x   = par;
          par = uncle;
        }
        _ASSERT(x == par->right);
        leftRotate(gpar);
        par->color = Black;
        gpar->color = Red;
        return par;
      }
    }
  }
  return x;
}

char* FSEQ findRB(RBNode *tree, U32 key) {
  while(tree) {
    if(tree->key == key)
      return & tree->data[0];
    if(key < tree->key)
      tree = tree->left;
    else
      tree = tree->right;
  }
  return NULL;
}

int freeRB(RBNode *tree, int (*freeData)(U32 key, char * FSEQ data)) {
  if(! tree) return 1;
  freeRB(tree->left, freeData);
  freeRB(tree->right, freeData);
  // Now free the node
  if(freeData) {
    (*freeData)(tree->key, & tree->data[0]);
  }
  free(tree);
  return 1;
}
