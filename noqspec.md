*No Quote* Specification 0.1.0
==============================

No Quote is a file format for storing plain text data similar to JSON and YAML, and inspired by Valve Software's [KeyValues] [] format.  The objective of No Quote is to create a hierarchical data format that is easy to read and easy to parse.

Description of the No Quote Format
==================================

No Quote (.noq) files consist of an ordered list of pairs containing a *name* and *value*.  Names can contain any character except for whitespace, and `[`, `]`, '#', and `:` characters,  which have special meanings.  Names do not need to be unique (thus a name does not uniquely identify a value) and names can be empty. Following the name is either a colon or opening square bracket, determining whether the value is a *datum* or another list (a *sublist*), respectively.  A datum contains all characters after the colon (including whitespace) until and exluding the next end-of-line sequence.  Note that *data* can contain the special `[`, `]`, `#`, and `:` characters.  A sublist is just another list like the root list, but it is enclosed by opening and closing square brackets.

#Hierarchical Pair Lists
#=======================
#
#No Quote files are concrete representations of an abstract data type called a #"Hierarchical Pair List" or "HPL."  An HPL consists of a list of pairs between #names and values. Names are strings, and values may either be fields (also #strings) or another, nested HPL.

Lexing and Parsing
==================

Tokenization
------------

The regular expressions describe how a No Quote file is broken into tokens.  The symbols `: # [ ]` are treated literally while other symbols have special meaning.  Basic RE symbols are used, such as the kleene star to match zero or more instances of a pattern, parentheses for grouping, and vertical bars for matching one of two patterns.  Patterns that are adjacent to eachother (spaces are ignored) create a concatenated pattern.  

Additional symbols are used to shorten sequences and reduce redundancy.  Words in curly braces are "expanded" into regular expressions; some are pre-defined, and can be interpreted as sets, while others are explicitly defined.

Character Class:    | Stands for:
--------------------|---------------------------
`{ws}`              | A whitespace character
`{nonws}`           | A character that is not whitespace
`{term}`            | A terminator character or sequence (\n, \r, and \r\n)
`{nonterm}`         | A character that is not a terminator

Explicitly defined macros: | Stands for:
---------------------------|--------------------------------------------------
`{wsc}`                    | A whitespace character or a comment
`{name}`                   | A valid name with leading and trailing whitespace

Tokens that carry values have a comma separated list of characters that identify what values they have.  A '.' is placed after the part of an expression read into one of the token's attributes; a character that follows the '.' indicates which attribute the sequence is placed.

Regular Expression                      | Token            | Abbreviation
----------------------------------------|------------------|-------------
`{name} : {nonterm}*.f ({term}\|<eof>)` | `FIELD_PAIR,n,f` | `f`
`{name} [`                              | `LIST_START,n`   | `l`
`{wsc}* ] {wsc}* <eof>`                 | `LIST_END`       | `e`

```
{wsc} = {ws}* # {nonterm}* {term}
{name} = {wsc}* {nonws}*.n {wsc}*
```

Grammar
-------

Variable | Expression | Description
---------|------------|---------------------------
`S`      | `::= fS`     | Start/top level
         | `::= lS'E`   |
         | `::= ε`      |
`S'`     | `::= fS'`    | Sub-level
         | `::= lS'E'`  |
`E`      | `::= eS`     | Exit to top level
`E'`     | `::= eS'`    | Exit to previous sub-level

[KeyValues]: https://developer.valvesoftware.com/wiki/KeyValues_class
