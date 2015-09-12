#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hpl.h"

struct Hpl_Node *Hpl_new()
{
    struct Hpl_Node *hpl = (struct Hpl_Node *) malloc(sizeof (struct Hpl_Node));
    hpl->type = HPL_EMPTY;
    hpl->next = NULL;
    hpl->name = NULL;
    hpl->value.field = NULL;
    return hpl;
}

struct Hpl_Node *Hpl_appendField(struct Hpl_Node *hpl, const char *name,
  const char *field)
{
    hpl->type = HPL_FIELD;
    hpl->name = (char *)malloc(strlen(name) + 1);
    strcpy(hpl->name, name);
    hpl->value.field = (char *)malloc(strlen(field) + 1);
    strcpy(hpl->value.field, field);
    hpl->next = Hpl_new();
    return hpl->next;
}

struct Hpl_NodePair Hpl_appendSubList(struct Hpl_Node *hpl, const char *name)
{
    hpl->type = HPL_SUB_LIST;
    hpl->name = (char *)malloc(strlen(name) + 1);
    strcpy(hpl->name, name);
    hpl->value.subList = Hpl_new();
    hpl->next = Hpl_new();
    struct Hpl_NodePair pair = { hpl->next, hpl->value.subList };
    return pair;
}

void Hpl_free(struct Hpl_Node *hpl)
{
    if (hpl->type != HPL_EMPTY)
        free(hpl->name);

    if (hpl->type == HPL_FIELD)
        free(hpl->value.field);
    else if (hpl->type == HPL_SUB_LIST)
        Hpl_free(hpl->value.subList);

    if (hpl->next != NULL)
        Hpl_free(hpl->next);

    free(hpl);
}

const char *Hpl_readField(const struct Hpl_Node *node)
{
    if (node->type != HPL_FIELD)
        return NULL;

    return node->value.field;
}

const struct Hpl_Node *Hpl_readSubList(const struct Hpl_Node *node)
{
    if (node->type != HPL_SUB_LIST)
        return NULL;

    return node->value.subList;
}

const struct Hpl_Node *Hpl_seekForward(const struct Hpl_Node *node,
  unsigned int distance)
{
    if (distance == 0)
        return node;
    if (node->type == HPL_EMPTY)
        return NULL;

    return Hpl_seekForward(node->next, distance - 1);
}

const struct Hpl_Node *Hpl_findNext(const struct Hpl_Node *node,
  const char *name)
{
    if (node->type == HPL_EMPTY)
        return NULL;
    if (strcmp(node->name, name) == 0)
        return node;

    return Hpl_findNext(node->next, name);
}
