# Verlet Lexer

Verlet Lexer _( Vlex )_ is a C framework that enables token recognition.

A lexer is any implementation that breaks down text data into fragmented data called _tokens_. Verlet Lexer does the same but in an exteamly customizable fashion, being a C framework, it integrates with C to provide system level control along with definable token recogniton and

## Vscript

Verlet Script _( Vscript )_ is a DSL, a domain specific language, that handles,

* Behavior of the Lexer : This is done via a data structure called **VLUT** _(Variation Look Up Table)_. VLUTs can be defined and are seen as a context on the basis of which the tokens are scanned. Multiple VLUTs are definable, one may switch the between different VLUTs according to needs.

* Reading of the tokens : This is done via a type of function called **Collect Functions**. The process of reading tokens in Vscript is called _collecting_ the tokens. Tokens are collected from strings.

## VLUT
**V**ariation **L**ook **U**p **T**able is a data structure that defines how the tokens are formed. Thus defining VLUTs is the most sensitive part of this framework, if a VLUT is wrongly defined, tokens will be _formed_ wrongly and thus the entire point of lexer will vanish to null.

### ```Vlut``` Block
To define a VLUT, we must open a ```VLUT``` Block.
```
Vlut {
    /* All the VLUT definition happens here. */
}
```
What happens internally in this block is that a block memory is _dynamically allocated_ to us so that we can safely define VLUTs of any size.

### ```For```-```Join```-```Vary```
Entire definition of a VLUT can be done using just these three simple rules.
```
Vlut {
    For set1
        Join set2
        Vary set3;
}
```
Above written code is a completely valid _( initialized )_ C code, ```set1```, ```set2```, and ```set3``` represent the set of ```char```s that we wish to apply the rules to.
