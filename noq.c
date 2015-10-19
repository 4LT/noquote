#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "noq.h"

static const uint8_t bom[3] = { 0xEF, 0xBB, 0xBF };

static const size_t INIT_NAME_SIZE = 16;
static const size_t INIT_DATUM_SIZE = 128;

struct RszString
{
    char *data;
    size_t length;
    size_t capacity;
};

struct RszString *newRszString(size_t cap)
{
    struct RszString *str = (struct RszString *)
      malloc(sizeof (struct RszString));
    str->data = (char *)malloc(cap);
    str->data[0] = '\0';
    str->length = 0;
    str->capacity = cap;
    return str;
}

void freeRszString(struct RszString *str)
{
    free(str->data);
    free(str);
}

void appendChar(struct RszString *str, char c)
{
    str->data[str->length] = c;
    str->length++;
    if (str->length >= str->capacity) {
        str->capacity = 2 * str->capacity;
        str->data = realloc(str->data, str->capacity);
    }
    str->data[str->length] = '\0';
}

void clearRszString(struct RszString *str)
{
    str->length = 0;
    str->data[0] = '\0';
}

bool stripChar(int testChar, FILE *stream)
{
    int streamChar = fgetc(stream);
    if (streamChar == testChar)
        return true;
    ungetc(streamChar, stream);
    return false;
}

bool handleLineTerm(int ch, unsigned int *line, FILE *stream)
{
    switch (ch) {

        case '\r':
            stripChar('\n', stream);
        case '\n':
            ++*line;
            return true;
        default:
            return false;
    }
}

void stripComment(unsigned int *line, FILE *stream)
{
    int ch;
    do {
        ch = getc(stream);
    } while (ch != EOF && !handleLineTerm(ch, line, stream));
}

const char *parseNoq(struct Hpl_Node *hpl, bool top, unsigned int *line,
  FILE *stream)
{
    enum { READ_LEADING_WS, READ_NAME, READ_TRAILING_WS, READ_DATUM, DONE }
      state = READ_LEADING_WS;
    struct RszString *name = newRszString(INIT_NAME_SIZE);
    struct RszString *datum = newRszString(INIT_DATUM_SIZE);
    const char *errorMsg;

    while (state != DONE) {
        int ch = fgetc(stream);

        if (iscntrl(ch) && !isspace(ch)) {
            errorMsg = "Illegal control character.";
            state = DONE;
        }

        else if (state == READ_DATUM) {
            if (handleLineTerm(ch, line, stream) || ch == EOF) {
                hpl = Hpl_appendDatum(hpl, name->data, datum->data);
                clearRszString(name);
                clearRszString(datum);
                state = READ_LEADING_WS;
            } else {
                appendChar(datum, (char)ch);
            }
        }

        else if (ch == EOF) {
            if (!top) {
                errorMsg = "End of file while not in top level.";
            }
            else if (state == READ_LEADING_WS) {
                errorMsg = NULL;
            } 
            else {
                errorMsg = "Missing datum.";
            }

            state = DONE;
        }

        else if (handleLineTerm(ch, line, stream)) {
            if (state != READ_LEADING_WS) {
                state = READ_TRAILING_WS;
            }
        }

        else if (isspace(ch)) {
            if (state != READ_LEADING_WS) {
                state = READ_TRAILING_WS;
            }
        }

        else if (ch == ':')
            state = READ_DATUM;

        else if (ch == '[') {
            struct Hpl_NodePair pair = Hpl_appendSubList(hpl, name->data);
            clearRszString(name);
            hpl = pair.nextNode;
            errorMsg = parseNoq(pair.subList, false, line, stream);

            if (errorMsg == NULL)
                state = READ_LEADING_WS;
            else
                state = DONE;
        }

        else if (ch == ']') {
            if (top)
                errorMsg = "Closing bracket has no matching bracket.";
            else if (state == READ_LEADING_WS)
                errorMsg = NULL;
            else
                errorMsg = "Missing datum.";
            state = DONE;
        }

        else if (ch == '#')
            stripComment(line, stream);
        
        else {
            if (state == READ_TRAILING_WS) {
                errorMsg = "Unexpected identifier after name.";
                state = DONE;
            } else {
                appendChar(name, (char)ch);
                state = READ_NAME;
            }
        }
    }

    freeRszString(name);
    freeRszString(datum);
    return errorMsg;
}

const char *Noq_parse(struct Hpl_Node *hpl, unsigned int *lineNum, FILE *stream)
{
    *lineNum = 1;

    if (!(stripChar(bom[0], stream) && stripChar(bom[1], stream) &&
      stripChar(bom[2], stream)))
    {
        rewind(stream);
    }

    return parseNoq(hpl, true, lineNum, stream);
}
