

##### Below shown is purely implemented in C23 via a self-coded framework called _Verlet Lexer_ _(Vlex)_. Not the best practices were followed, and many changes in the future are to come, but even this primitive lexer could colorize the tokens of a C code.
![Example Code](project_001_output.png)

# <span style="color:rgb(224, 102, 102)">Verlet Lexer</span>
Verlet Lexer is a C framework that enables token recognition. It is commonly referred to as _Vlex_ and is a component of a unification called the _Verlet Framework_.

A lexer is any implementation that breaks down text data into fragmented data, into fragments called _tokens_. Verlet Lexer does the same but in a highly modifiable manner, being a C framework, it integrates with C to provide system level control along with token recognition which allows the making-of compilers, _transpilers_, interpreters, anything that relies on token recognition.


### Components of Verlet Lexer
- Data Structures
    - `avsme`
    - `hash`
    - VLUT _(Variation Look-Up Table)_
- Collect Functions
    - `collect_immediate`
    - `collect_variation`
- Domain Specific Language _( **Vscript** )_
    - **VLUT** Maker
    - **EM** _(Expected Mode)_ Maker
    - **FIO** _(File Input/Output)_ Manager
    - __*UC*__ _(Universal Context)_ Maker **<span style="font-size:80%;color:red"> ! Not Yet Added ! </span>**

>_Universal Context Maker_ is Verlet's approach as parsing tokens. It is a complex header with its own DSL that can be (is always) seen as a part of Verlet Lexer's DSL Vscript. UCM is a token parsing utility; Conventianally being the suceeding step to lexical analysis, token parsing in Verlet Lexer is a part of Vscript (that is a lexer's DSL).
