*No Quote* Specification 0.1.0
==============================

No Quote is a file format for storing plain text data similar to JSON and YAML,
and inspired by Valve Software's
[KeyValues] [] format.  The objective of No Quote is to create a hierarchical
data format that is easy to read, uses a minimal syntax, and which represents
textual data as literally as possible (or practical) by removing the need for
escape sequences and quote delimiters.

Hierarchical Pair Lists
=======================

No Quote files are concrete representations of an abstract data type called a
"Hierarchical Pair List" or "HPL."  An HPL consists of a list of pairs between
names and values. Names are strings, and values may either be fields (also
strings) or another, nested HPL.

Lexing and Parsing
==================

Tokenization
------------

The regular expressions describe how a No Quote file is broken into tokens.  The
symbols `: # [ ]` are treated literally while other symbols have special
meaning.  Basic RE symbols are used, such as the kleene star to match zero or
more instances of a pattern, parentheses for grouping, and vertical bars for
matching one of two patterns.  Patterns that are adjacent to eachother (spaces
are ignored) create a concatenated pattern.

Additional symbols are used to shorten sequences and reduce redundancy.  Words
in curly braces are "expanded" into regular expressions; some are pre-defined,
and can be interpreted as sets, while others are explicitly defined.

Pre-defined macros: | Stands for:
--------------------|---------------------------
{ws}                | A whitespace character
{nonws}             | A character that is not whitespace
{term}              | A terminator character or sequence (\n, \r, and \r\n)
{nonterm}           | A character that is not a terminator

Explicitly defined macros: | Stands for:
---------------------------|--------------------------------------------------
{wsc}                      | A whitespace character or a comment
{name}                     | A valid name with leading and trailing whitespace

Tokens that carry values have a comment separated list of characters that
identify what values they have.  A '.' is placed after the part of an expression
read into one of the token's attributes; a character that follows the '.'
indicates which attribute the sequence is placed.

Regular Expression                      | Token          | Abbreviation
----------------------------------------|----------------|-------------
{name} : {nonterm}*.f ({term}\|<eof>)   | FIELD_PAIR,n,f | f
{name} [                                | LIST_START,n   | l
{wsc}* ] {wsc}* <eof>                   | LIST_END       | e

{wsc} = {ws}* # {nonterm}* {term}
{name} = {wsc}* {nonws}*.n {wsc}*

Grammar
-------

Variable | Expression | Description
---------|------------|---------------------------
S        | ::= fS     | Start/top level
         | ::= lS'E   |
         | ::= ε      |
S'       | ::= fS'    | Sub-level
         | ::= lS'E'  |
E        | ::= eS     | Exit to top level
E'       | ::= eS'    | Exit to previous sub-level


[KeyValues]: https://developer.valvesoftware.com/wiki/KeyValues_class
