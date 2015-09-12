#ifndef _NOQ_H_
#define _NOQ_H_

#include <stdio.h>
#include "hpl.h"

const char *Noq_parse(struct Hpl_Node *hpl, unsigned int *lineNum,
  FILE *stream);

#endif
