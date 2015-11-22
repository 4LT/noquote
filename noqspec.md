*No Quote* Specification 0.1.0
==============================

No Quote is a file format for storing indexed data in a hierarchical structure, similar to JSON, YAML, and inspired by Valve Software's [KeyValues] [] format.  The objective of No Quote is to create a data format that is easy to read, manipulate, and parse.

Description of the No Quote Format
==================================

No Quote (.noq) files consist of an ordered list of pairs containing a *name* and *value*.  Names can contain any character except for whitespace, and `[`, `]`, `#`, and `:` characters,  which have special meanings.  Names do not need to be unique (thus a name does not uniquely identify a value), and names can be empty. Following the name is either a colon or opening square bracket, determining whether the value is a *datum* or another list (a *sublist*), respectively.  A datum contains all characters after the colon (including whitespace) until and exluding the next end-of-line sequence.  Note that *data* can contain the special `[`, `]`, `#`, and `:` characters.  A sublist is just another list like the root list, but it is enclosed by opening and closing square brackets.


Lexing and Parsing
==================

`NOTE: I may use different REs and Tokens (and thus a different grammar) in the future; I think there's a better way to do this.`

Tokenization
------------

These regular expressions describe how a No Quote file is broken into tokens.  The symbols `: # [ ]` are treated literally, while other symbols have special meaning.  Basic RE symbols are used: the kleene star matches zero or more instances of a pattern, parentheses group patterns, and vertical bars match one of two patterns on the left or right.  Patterns that are adjacent to eachother (spaces are ignored) create a concatenated pattern.  Items enclosed in curly braces are "macros" that expand into patterns.

Character Classes | Stands for:
------------------|---------------------------
*{ident}*         | A valid character as part of a name (non whitespace, [, ], :, or #)
*{term}*          | A terminator character or sequence (\n, \r, or \r\n)
*{nonterm}*       | A character that is not a terminator

Regular Expression                     | Token  | 
---------------------------------------|--------|
#*`{nonterm}*{term}`*                  | `none` | 
:*`{nonterm}*{term}`*                  | datum  |
[                                      | [      |
]                                      | ]      |
*`{ident}*`*                           | name   |

Tokens that carry values have a comma separated list of characters that identify what values they have.  A '.' is placed after the part of an expression that is read into one of the token's attributes; a character that follows the '.' indicates which attribute the sequence is placed.

Abbreviations stand for tokens in the grammar.

Grammar 
-------

               |
---------------|----------------------
_noq-file_     | ::= _pair-list_ <eof>
_pair-list_    | ::= _pair_ _pair-list_
               | ::= ε
_pair_         | ::= name datum
               | ::= name **[** _pair-list_ **]**

[KeyValues]: https://developer.valvesoftware.com/wiki/KeyValues_class
