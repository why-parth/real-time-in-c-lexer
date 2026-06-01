![Title Image](.\ignore\verlet_no_bg.png)

##### Below shown is purely implemented in C23 via a self-coded framework called _Verlet Lexer_ _(Vlex)_. Not the best practices were followed, and many changes in the future are to come, but even this primitive lexer could colorize the tokens of a C code.
![Example Code](.\ignore\project_001_output.png)

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
    - __*UC*__ _(Universal Context)_ Maker **<span style="font-size:80%;color:red"> ------------------------------- ! Not Yet Added ! </span>**

>_Universal Context Maker_ is Verlet's approach as parsing tokens. It is a complex header with its own DSL that can be (is always) seen as a part of Verlet Lexer's DSL Vscript. UCM is a token parsing utility; Conventianally being the suceeding step to lexical analysis, token parsing in Verlet Lexer is a part of Vscript (that is a lexer's DSL).

# <span style="color:rgb(224, 102, 102)">Data Structures of Verlet Lexer</span>

## `avsme`
AVSME _( `avsme` )_ stands for ASCII-Variance-Subclass-Mainclass-Exists, it is a _memory_ layout that is defined to store the _identity_ of a `char`, _identity_ unifies all the essential information about a `char` that we can ever require in the later stages of lexical analysis.

`avsme` is defined in the SVH _(Standard Verlet Header)_ `svh_01_avsme.h`, AVSME Header. AVSME Header implements a 16 bit long layout using the `uint_least16_t`, the memory layout reserves (from the left):
- 8 bits for the ascii value of the char
- 1 bit for the variance of the char (for later stage processing)
- 2 bits for sub-class
- 4 bits for main-class
- 1 bit for marking validity

`ascii` `variance` `sub-class` `main-class` `exists`

### AVSME Interface
AVSME Interface refers to the collection of utilities defined in the AVSME Header.

`AVSME_GET(a)`, returns the value of the query (`ASCII`, `VARIANCE`, `SUBCLASS`, `MAINCLASS` or `EXISTS`).

`AVSME_SET(a)`, assigns the value of the query.

`AVSME_VARIANT(a)`, returns `AVSME_GET(a, VARINACE)`.

`AVSME_BOOLEAN(a)`, returns the boolean value (`AVSME_TRUE`, `AVSME_FALSE` or `AVSME_NONE`).


`AVSME_COMPARE(a, b)`, returns `1` if the value of the query is same for both `a` and `b`, `0` if not.

`AVSME_OVERLAP(a, b)`, returns `1` if the ASCII value of `a` and `b` is same or if the class of `a` _includes_ that of `b`, `0` otherwise.

> The phrase _"class of `a` includes that of `b`"_ will be made clear once char classes are understood. 


### Char Class
Characters in this framework can be grouped into classifications called _char classes_ such as `numeric` _(numbers)_, `operate` _(operators)_. Sometimes, classes are not enough to correctly group the characters, in those cases, _char sub-classes_ can also be defined.

Char classes can be _(must be)_ defined using the macro `charclass`, `charclass` is a keyword in Verlet Lexer framework, it is not allowed to name anything as `charclass`.

To not have any char class, you can define `charclass` as,

```C
#define charclass(c) AVSME_SET(1, ASCII, c)
```

Verlet Lexer comes with a standard implementation of char classes.

<table>
<tr>
	<td>Main Class</td>
	<td>Sub Class</td>
	<td>Instances</td>
</tr>
<tr>
    <td rowspan="3"><code>idvalid</code></td>
    <td><code>under</code></td>
    <td><code>_</code></td>
</tr>
<tr>
    <td><code>lower</code></td>
    <td><code>a</code>-<code>z</code></td>
</tr>
<tr>
    <td><code>upper</code></td>
    <td><code>A</code>-<code>Z</code></td>
</tr>
<tr>
    <td colspan="2"><code>numeric</code></td>
    <td><code>0</code>-<code>9</code></td>
</tr>
<tr>
    <td rowspan="2"><code>operate</code></td>
    <td><code>single</code></td>
    <td><code>~</code> <code>!</code></td>
</tr>
<tr>
    <td><code>double</code></td>
    <td><code>+</code> <code>-</code> <code>*</code> <code>/</code> <code>%</code> <code>^</code> <code>&amp;</code> <code>|</code></td>
</tr>
<tr>
    <td rowspan="2"><code>enclose</code></td>
    <td><code>close</code></td>
    <td><code>&gt;</code> <code>)</code> <code>]</code> <code>}</code></td>
</tr>
<tr>
    <td><code>open</code></td>
    <td><code>&lt;</code> <code>(</code> <code>[</code> <code>{</code></td>
</tr>
<tr>
    <td colspan="2"><code>quoting</code></td>
    <td><code>`</code> <code>"</code> <code>'</code></td>
</tr>
<tr>
    <td rowspan="3"><code>disjoin</code></td>
    <td><code>comma</code></td>
    <td><code>,</code> <code>;</code></td>
</tr>
<tr>
    <td><code>dot</code></td>
    <td><code>.</code> <code>:</code></td>
</tr>
<tr>
    <td><code>space</code></td>
    <td><code> </code></td>
</tr>
<tr>
    <td colspan="2"><code>special</code></td>
    <td><code>#</code> <code>$</code> <code>@</code> <code>?</code></td>
</tr>
<tr>
    <td colspan="2"><code>escape</code></td>
    <td><code>\</code></td>
</tr>
<tr>
    <td colspan="2"><code>assign</code></td>
    <td><code>=</code></td>
</tr>
</table>

This implementation not only has _classes_, but they also have _sub\_classes_. To use this implementation, we must define `charclass` as a macro as,

```#define charclass VERLET_charclass```

>If you use the above implementation alone, the escape sequences give an _invalid char_ error, and that error can be handles using the `VSCRIPT_INVALID_CHAR_CASE` macro. When you include the standard implementation via `verlet_std.h`, the escape sequences already come implemented using `VSCRIPT_INVALID_CHAR_CASE` so no need to do anything.

`VERLET_charclass_print` is a function that prints the Verlet's standard implementation of the `charclass`, it does work for other implementations but only if the guidelines are followed.

> Verlet's standard `charclass` implementation printed using `VERLET_charclass_print`
>
> ![Image of Verlet Standard Char-class implementation](.\ignore\charclass_image.png)
>
> You can see how each mainclass is represented with a number, and so is the sub-class. Those numbers are the internally assigend enumarations; Those classes that do not have any sub-classes have the _sub-class enumeration_ of `0`.
>
> Mainclass and Subclass can be represented using just _one two digit number_. Ones digit representing the subclass, Tens digit representing tha mainclass. For example `42` represents, `4` for mainclass that is char mainclass `enclose`, and `2` for subclass that is char subclass `close`.

Collectively, mainclass and subclass are called fullclass, that can be represented using just one two digit number. Fullclass for Verlet's standard charclass can be attained using the function `VELRET_fullclass`.

Char-class inclusion is a bool that is evaluated as, for `a` and `b`,
- Mainclass of `b` must always be as that of mainclass of `a`.
- If `a` has sub-class, subclass of `b` must always be as that of subclass of `a`.
- If `a` doesn't have sub-class, subclass of `b` can be anything.

> Char-class inclusion is also called char-class overlap. `AVSME_OVERLAP(a, b)` returns the overlap of `a` and `b`, sequence matters.






