No Quote Specification 0.1.0
==============================

No Quote is a file format for storing named data in a hierarchical structure, similar to JSON, YAML, and Valve Software's [KeyValues] [] format.  The objective of No Quote is to create a data format that is easy to read, manipulate, and parse.

Example
=======

```
# Comment
name :value
name :value2
root [
    foo :bar
    boo :baz

    paragraph :Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a
    :diam lectus. Sed sit amet ipsum mauris. Maecenas congue ligula ac quam
    :viverra nec consectetur ante hendrerit.
]

quote :"no escapes are necessary" # not a comment, part of the value
```

Description of the No Quote Format
==================================

No Quote (.noq) files are UTF-8 text files which consist of an ordered list of pairs composed of a *name* and *value*.  Names can contain any character except for whitespace, or `[`, `]`, `#`, and `:` characters, and names can even be empty.  Names do not need to be unique, so a name does not necessarily uniquely identify a value. Following the name is either a colon or opening square bracket, determining whether the value is a *datum* or another list (a *sublist*), respectively.  A datum contains all characters after the colon (including whitespace) until the next end-of-line sequence, which is not included as part of the dataum.  Note that data can contain the special `[`, `]`, `#`, and `:` characters.  A sublist is just another list like the root list, but it is enclosed by opening and closing square brackets. Finally, comments are started with a `#`, causing everything after the and including the character to be ignored until a line terminator is reaced.


Lexing and Parsing
==================

Possible lexing and parsing methods provided in an attempt to unambiguously determine whether or not a file meets the specification.

Tokenization
------------

These regular expressions describe how a No Quote file is broken into tokens.  The symbols `: # [ ]` are treated literally, while other symbols have special meaning. Basic regular expression rules apply (ex. kleene star), with the addition of character classes represented with curly braces.

Character Classes | Stands for:
------------------|---------------------------
*{ident}*         | A valid character as part of a name (non whitespace, [, ], :, or #)
*{term}*          | A terminator character or sequence (\n, \r, or \r\n)
*{nonterm}*       | A character that is not a terminator


Regular Expression                     | Token      | 
---------------------------------------|------------|
#*`{nonterm}*{term}`*                  | `no token` | 
:*`{nonterm}*{term}`*                  | datum      |
[                                      | [          |
]                                      | ]          |
*`{ident}*`*                           | name       |

Grammar 
-------

|Variable       |Production Rule                   |
|---------------|----------------------------------|
|_noq-file_     | ::= _pair-list_ \<eof\>          |
|_pair-list_    | ::= _pair_ _pair-list_           |
|               | ::= ε                            |
|_pair_         | ::= name datum                   |
|               | ::= name **[** _pair-list_ **]** |

+ Non-terminals (variables) are italicized.
+ Literal tokens are shown in bold.
+ Tokens with values are neither italicized or bold.
+ <eof> represents the end of the file.
+ The epsilon reprents an empty production rule.


[KeyValues]: https://developer.valvesoftware.com/wiki/KeyValues_class
