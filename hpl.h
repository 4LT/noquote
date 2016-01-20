/*  Data structures and operations for HPLs (Hierarchical Pair Lists).
 *  An HPL is the abstract syntax tree for No Quote files.  At the top level is
 *  a list of name-value pairs, where a value is either a datum represented as a
 *  string, empty, or another list of pairs.
 */

#ifndef _HPL_H_
#define _HPL_H_

#include <stdbool.h>

/* Type of value found inside a node, as explained above. */
enum Hpl_ValType { HPL_DATUM, HPL_SUB_LIST, HPL_EMPTY };

/*  A Node, which may be regarded as a list as well.  That is, all nodes are
 *  lists, nodes with an HPL_EMPTY value type are empty lists, and nodes with
 *  an HPL_SUB_LIST value type have a list for its value.  Since a node can
 *  point to more than one node, nodes may also be regarded as a trees as well.
 *
 *  enum Hpl_ValType type - the type of the node's value
 *  char *name - name by which the node (and its value) can be referenced
 *  union Hpl_Val value - value associated with the node as either a datum or
 *      a branching list
 *  struct Hpl_Node *next - the next node in the list
 */
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

/*  A 2-tuple from appending a sub-list value to a list, containing the newly-
 *  created next element of the list and the list's sub-list value.
 */
struct Hpl_NodePair
{
    struct Hpl_Node *nextNode;
    struct Hpl_Node *subList;
};

/*  Functions for constructing hierarchical pair lists. */

/*  Creates a new, empty list. */
struct Hpl_Node *Hpl_new();
/*  Adds a new datum to the specified list (hpl). */
struct Hpl_Node *Hpl_appendDatum(struct Hpl_Node *hpl, const char *name,
  const char *datum);
/*  Creates a new list that is appended to the specified list (hpl). */
struct Hpl_NodePair Hpl_appendSubList(struct Hpl_Node *hpl, const char *name);
/*  Frees the memory allocated to the list, including all its elements and
 *  children recursively. */
void Hpl_free(struct Hpl_Node *hpl);

/*  Functions for reading hierarchical pair lists. All operations are non-
 *  destructive. */

/*  Gets the datum from a node, or returns NULL if the node is not of the datum
 *  type. */
const char *Hpl_readDatum(const struct Hpl_Node *node);
/*  Gets the sub-list from a node, or returns NULL if the node is not of the
 *  sub-list type. */
const struct Hpl_Node *Hpl_readSubList(const struct Hpl_Node *node);
/*  Gets the node that is "distance" steps ahead of the current node.  If the
 *  distance exceeds the number of remaining elements in the list, NULL is
 *  returned. */
const struct Hpl_Node *Hpl_seekForward(const struct Hpl_Node *node,
  unsigned int distance);
/*  Returns the first instance of a node matching the given name.  If there is
 *  no match, NULL is returned.  Sub-lists are not searched. */
const struct Hpl_Node *Hpl_findNext(const struct Hpl_Node *node,
  const char *name);

#endif
