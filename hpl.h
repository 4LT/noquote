#ifndef _HPL_H_
#define _HPL_H_

#include <stdbool.h>

enum Hpl_ValType { HPL_DATUM, HPL_SUB_LIST, HPL_EMPTY };

struct Hpl_Node
{
    enum Hpl_ValType type;
    char *name;
    union Hpl_Val
    {
        char *datum;
        struct Hpl_Node *subList;
    } value;
    struct Hpl_Node *next;
};

struct Hpl_NodePair
{
    struct Hpl_Node *nextNode;
    struct Hpl_Node *subList;
};

struct Hpl_Node *Hpl_new();
struct Hpl_Node *Hpl_appendDatum(struct Hpl_Node *hpl, const char *name,
  const char *datum);
struct Hpl_NodePair Hpl_appendSubList(struct Hpl_Node *hpl, const char *name);
void Hpl_free(struct Hpl_Node *hpl);

const char *Hpl_readDatum(const struct Hpl_Node *node);
const struct Hpl_Node *Hpl_readSubList(const struct Hpl_Node *node);
const struct Hpl_Node *Hpl_seekForward(const struct Hpl_Node *node,
  unsigned int distance);
const struct Hpl_Node *Hpl_findNext(const struct Hpl_Node *node,
  const char *name);

#endif
