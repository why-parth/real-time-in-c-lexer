# VScript

#### _version 1.2.0_

_**VScript**_ is the DSL (Domain Specific Language) of Verlet Lexer that works remarkably well with C.

<br>

<hr>

### Verlet Lexer : Use case and DSL

<hr>

#### Token Recognition
Verlet Lexer (framework) is a state machine that initializes token recognition in C. A state machine is any functioning unit whose behavior comes from a set of _"definable"_ rules, that are to be defined by the standard _(the word "standard" in this document is used to refer to all the clients that use this framework)_. The entire purpose of Verlet Lexer is to treat token recognition as a state dependent process and provide methods to define and manipulate states.

Verlet Lexer is nothing beyond a _token recognition utility_, and all of its interface is encapsulated in its DSL *__VScript__*.

<br>

#### Overview of VScript
Verlet Lexer comes with an initialized pipeline of how tokens are extracted from _any_ buffer, but that pipeline is highly rich in mechanism based workarounds. It would demand a complete understanding of all the internal mechanisms to be able to work with the pipeline.

Thus, Verlet Lexer comes with a layer of abstraction built on top of the pipeline that gets rid of all the complex mechanism based workarounds and provides us an _exception-less_ syntax directly in C. That exception-less syntax is _VScript_, VScript can be _(should be)_ looked at as a different language initialized in C, to be **_used side-by-side with C_**.

<br>

> Not necessary but worth knowing.

VScript is majorly a macro-based initialization that makes possible:
- Polymorphism : Macro based implementation of VScript allows C's compiler to replace/call functions depending on the datatype and number of inputs _(C11+' `_Generic` and `##` are extensively used)_.
- Automation : All the heavy lifting is done behind the scenes by the complex yet organized macros, and all we ever see are simple keywords.
- Optimization : It is true that using macros increase the final binary size as repetition of code takes place, but, the (Verlet Lexer's) CCast optimization reduces the final file-size by astronomical sizes while also making the over all computation faster.

<br>

VScript comes in two flavors, **_Hosted_** and **_Non-hosted_**. VScript exists to provide high level control over the pipeline, therefore, VScript simplifies the pipeline internally and that simplification is then wrapped in a usable syntax. The simplification is what is referred to as **_Non-hosted VScript_** and everything that extends that simplification is referred to as **_Hosted VScript_**.

> In this document, VScript and Hosted VScript are used interchangeably.

> In this document, we are only going to be discussing Hosted VScript, as that is easier to understand and demands near to zero knowledge of the pipeline. For understanding Non-hosted VScript, refer to _COLLECT.md_ .

<br>

<hr>

### Token Collection

<hr>


#### What is Token Collection ?
In Verlet Lexer's theory, the word _"collection"_ is used to denote the process of extraction of tokens.

<br>

#### How is Token Collection done ?
Let us entertain a practical example of token collection,
```C
char code[] = "int a = 45;";  // Char buffer to be collected from
```
Given the above `code`, can you figure out a way to correctly extract _valid C_ tokens? The problem is not extremely tough, one of the solutions is as straightforward as,
```C
char code[] = "int a = 45;";  // Char buffer to be collected from

uint8_t code_i = 0;           // Running Index of code's current char

do {
  if (different_token) putchar('\t');  // Split the token when needed
  putchar(code[code_i]);               // Append the previous token
} while (code[++code_i]);
```
In this solution, we print all the characters normally, but we print an extra `'\t'`, whenever the boolean `different_token` is `1`. As a result, we get,
> `int` ` ` `a` ` ` `=` ` ` `45` `;`

<br>

While the above solution works extremely well, it only works when the boolean `different_token` correctly splits `code`. Therefore, `different_token` must by dynamically assigned depending upon where we are in the `code`.

> `different_token` is an _arbitrary name_ of the condition that triggers the splitting of tokens. No such keyword exists in the actual implementation.

<br>

#### How does Verlet Lexer implement token collection ?
The idea is same, we iterate through all the characters but split the tokens only when a condition is met. In previous example we used an opaque boolean `different_token`, but in reality, Verlet Lexer uses a determinable parameter called `variation`. And a process called **_Variation Look Up_** to determine the `variation` at a given point.

VLU _(Variation Look Up)_ is a _definable state_. VLU can be understood as a _rule book_ that stores all the information of when to split a token.

Now the internal code simplifies to,
```C
char code[] = "int a = 45;";  // Char buffer to be collected from

uint8_t code_i = 0;           // Running Index of code's current char

do {
  Variation_Look_Up();           // Updates the variation boolean
  if (variation) putchar('\t');  // Split the token is variation is true
  putchar(code[code_i]);         // Append the previous token
} while (code[++code_i]);
```

<br>

#### What happens in the Variation Look Up process ?
While collecting tokens from a text, Verlet Lexer iterates through each of its `char`s. And for each **_adjacent_** `char` **_pair_**, it looks up its `variation`. The reason why this process is called a _"look up"_ is because for each adjacent `char` pair, the lexer searches a **_look up table_** to find its `variation`.

The look up table may look like,
(`'a'`, `'b'`) : `1`,
(`'a'`, `'c'`) : `0`,
(`'b'`, `'c'`) : `1`, ...

A `variation` look up table is nothing but a large dictionary storing the variations of all the possible `char` pairs.


<br>

To understand better, suppose we are reading the text `"int a = 45;"` to collect its tokens, then the we would start by iterate through each of the `char` one by one,
> `'i'` `'n'` `'t'` `' '` `'a'` `' '` `'='` `' '` `'4'` `'5'` `';'`

And for each adjacent `char` pair, we would look up for its `variation`. For (`'i'`, `'n'`), (`n'`, `'t'`), (`'t'`, `' '`), and so on.

<br>

Now if wish to treat all the characters as a single token, then it would mean that the `variation` for all adjacent `char` pair is `0`. In that case, the output tokens would be,
> `int a = 45;`

But we want there to be a split between `int` and ` ` _(space)_, we can tell the Verlet Lexer to do this by defining the `variation` of (`'t'`, `' '`) as `1`. Then,  
> `int` ` a = 45;`

Then, by defining the `variation` of (`' '`, `'a'`) as `1` we get,
> `int` ` ` `a = 45;`

If define the `variation` as `1` for (`'t'`, `' '`), (`' '`, `'a'`), (`'a'`, `' '`), (`' '`, `'='`), (`'='`, `' '`), (`' '`, `'4'`) and (`'5'`, `';'`). And `0` otherwise. Then we would get,
> `int` ` ` `a` ` ` `=` ` ` `45` `;`

<br>

By pinpointing the cases when we wish to split the tokens, we explicitly control the token collection. Each time a new `char` is read, `variation` for the new _(now)_ adjacent `char` pair is looked up and if its `1`, the token is split.

The only thing Verlet Lexer does is scan adjacent `char`s to check their `variation`, and splitting if its `1`. But, the lexer **_does NOT know_** where to look for the `variation`s. We have to explicitly direct the lexer to an appropriate location so that it can perform its look up for `variation`s. This makes Variation Look Up _(VLU)_ a state.

<br>

#### VLU as a state
Verlet Lexer is a state machine, and VLU is one _of its_ states. For the lexer to function, we must define and assign a VLU so that a look up can happen. VLU is the _"behavior"_ of the lexer, and yes, the behavior of lexer can dynamically be changes throughout the run-time of the program.


<br>

<hr>

### Keywords of Verlet Lexer Framework

<hr>

Now is a good time to pause the documentation and have a glimpse at all the keywords in Verlet Lexer.

<br>

#### What are keywords ?
Keywords are macros defined by the framework that can not be used as identifiers. As the macros will expand and will thus cause syntactic errors.

Verlet Lexer has a total of 32 keywords. However, it is used with _Verlet File I/O_ and _Verlet Utils_, so the total number of keywords increase, but the number of keywords _of_ Verlet Lexer is 32 only.

<br>

#### Keywords of Verlet Lexer

Keywords related to state definition
```C
charclass
VLU   For   Join   Vary   Char   All   Set   Call
Decl  Save  Make
Show  ShowVLU
```
Keywords related to global hash variable _(Non-hosted VScript)_
```C
hash_reset  hash_push
hash_value  hash_none
hash_of     hash_function
```
Keywords related to token comparison
```C
token_is  token_is_not
expect    keep_expecting    stop_expecting
```
Keywords related to token membership
```C
token_list
token_in  token_not_in
```
Keywords related to token management
```C
token_paste
token_print
token_show
```
Keywords related to Collection
```C
Using
Collect
```

<br>

<hr>

### `charclass`

<hr>

In Verlet Lexer, characters are grouped into classes called `charclass` _(char class)_. `charclass` of a `char` can be attained using `charclass(char)`, a function that can be changed at any point of run-time.

`charclass` is a state, like VLU, definable and manipulable. However this state MUST be defined. If VLU is not defined, the program will compile just fine, but if `charclass` is not defined, the program will not compile.

<br>

`charclass` definition must follow proper guidelines, and without the knowledge of the pipeline _(Non-hosted VScript)_, not much can be discussed about it.

You must NOT try to define `charclass` and expect it to work.

If you want no grouping amongst characters, the `charclass` of the passed `char` could be defined as its ASCII value. But you as a beginner might think that the implementation is as simple as,
```C
#define charclass(c) ( (uint8_t)c )
```
Well, its not.

> Although the above shown `charclass` definition looks simple and valid, it just so happens to be not frame-work friendly.
> Instead,
> ```C
> #define charclass(c) (AVSME_SET(AVSME_EXISTS, ASCII, c))
> ```
> But you will not have to ever define `charclass`

<br>

For a deeper understanding, read _COLLECT.md_ . You do not have to worry about the `charclass` definition, more than 90% of the times, the _Verlet Lexer's Standard Char Class_ will cover everything. 

<br>

#### `VERLET_charclass`

`VERLET_charclass` (Verlet's Standard Char Class implementation) is an implementation that comes with the Verlet Lexer. It is set active as,
```C
#define charclass VERLET_charclass
```
> It comes active by default, use `#undef charclass` to remove it and then define your own if you need to (NOT recommended unless guidelines are known).

<br>

`VERLET_charclass` is a function that implements the following classes,

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
    <td><code>oneval</code></td>
    <td><code>~</code> <code>!</code></td>
</tr>
<tr>
    <td><code>twoval</code></td>
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

The following distribution not only has classes, but it also has sub-classes. Sub-classes exist because classes were not enough to classify them fully.

<br>

#### `avsme` _(Non-hosted VScript)_

>This topic is not relevant Hosted VScript, but it is connected directly to `charclass` macro.
> This section can be completely ignored.

<BR>

The `charclass` macro by convention returns the datatype `avsme`. `avsme` does not have any restrictions to what it can be, but the default `avsme` is defined in `_01_non_statal.h` as,

```C
typedef uint_least16_t avsme;
```

In the same header, several macros that start with `AVSME_` are also defined and are collectively called AVSME Interface. AVSME Interface allows operations on `avsme` memory layouts, and since by default, `avsme` is a 16 bit memory layout, the hard coded implementations of AVSME Interface are define to operate in a non-definable behavior.

<br>

AVSME Interface, 
```
AVSME_NULL

AVSME_ASCII
AVSME_VARIANCE
AVSME_SUBCLASS
AVSME_MAINCLASS
AVSME_EXISTS

AVSME_ASCII_SHIFT
AVSME_VARIANCE_SHIFT
AVSME_SUBCLASS_SHIFT
AVSME_MAINCLASS_SHIFT
AVSME_EXISTS_SHIFT

AVSME_BIT(_name, _mask)
AVSME_GET(_name, _mask)
AVSME_SET(_name, _mask, _value)
AVSME_COMPARE(a, b, _mask)

AVSME_NONE
AVSME_TRUE
AVSME_FALSE

AVSME_BOOLEAN(a)
AVSME_OVERLAP(a, b)
AVSME_VARIANT(a)
AVSME_EXISTENT(a)
```

<br>

The later headers use the AVSME Interface, thus, defining a custom `charclass` requires a proper set of guidelines that make sure that the later header's do not fail upon custom `charclass` definitions. The guidelines are not tedious, but they are out of the scope of this document. Hence they will not be discussed.

<br>

<hr>

### Defining a VLU

<hr>

Defining VLUs is Verlet Lexer's heart. VLUs definition is extremely rich in functionality, VLUs alone make the Verlet Lexer a state of art. Usually, at the lexical analysis phase, you can not achieve much context, but in Verlet Lexer that is not the case.

<br>

#### `VLU` Block
The first step of defining a VLU is to open a VLU block (`VLU` Block), all VLU definitions happen in a VLU block. A VLU block is opened as,
```C
VLU {
  // All the VLU definition comes here
}
```
VLU block **_can_ NOT _be defined in the global scope_**.

<br>

#### `For`, `Join`, `Vary` and `All` Operators
After having opened a VLU block, the next thing is to define **_rules_**. The basic syntax is,
```C
VLU {

  For A
    Join B;

}
```
`For A` means that we are defining the rule _"for A,"_, and `Join B` after it means that _"if B comes in front of it, then join it."_. So the full rule becomes, **_"for A, if B comes in front of it, then join it."_**.

Here, 'A' and 'B' are just placeholders. In reality, we can either use `charclass`s to define for groups. Or we can use `char`s to define for individual `char`s.

<br>

```C
VLU {

  For idvalid      // // For all idavlids
    Join idvalid;  // Join all the idvalids that come after an idvalid

}
```
If you do not remember the `charclass`s, you can refer to them whenever you wish to, but the only thing the above rule does is that it tells the lexer to _join (concatenate)_ all the `idvalid`s that come in front of `idvalid`s.

<br>

So, the text `"this_is_a_full_token"` will be considered as a full token, as, all of the characters in this text are `idvalid`, hence, all of them are going to be joined.
> `this_is_a_full_token`

But what if you wanted there to be a split between alphabets and the underscore ? In such cases, we can use the `Vary` operator.

<br>

Just like how `Join` tells the lexer to join the two characters, `Vary` tells the lexer to vary the two characters i.e, break the token when the case occurs.
```C
VLU {

  For idvalid             // // For all idavlids
    Vary idvalid under    // Vary all the underscores that come after an idvalid
    Join idvalid lower    // Join all the lower alphabets that come after an idvalid
    Join idvalid upper;   // Vary all the upper alphabets that come after an idvalid

}
```
We can stack as many `Join`/`Vary` as we want to define a rule, all of the **_sub-rules_** will be followed in sequence. Now the same text would give,
> `this` `_is` `_a` `_full` `_token`
As you can see, whenever `'_'` comes in front of an `idvalid`, the token varies _(breaks)_.

<br>

But what if want the underscore to fully be disconnected ? In that case, we need to define a rule **_for underscore_**.
```C
VLU {

  For idvalid under    // // For all underscores
    Vary idvalid;      // Vary all the idvalids that comes after an underscore

  For idvalid             // // For all idavlids
    Vary idvalid under    // Vary all the underscores that come after an idvalid
    Join idvalid lower    // Join all the lower alphabets that come after an idvalid
    Join idvalid upper;   // Vary all the upper alphabets that come after an idvalid

}
```
The output will now be,
> `this` `_` `is` `_` `a` `_` `full` `_` `token`

Perfect, but the VLU blocks looks too hectic even for this small task. We can apply several changes to the VLU definition.

<br>

First, `All` operator can be used to select all the characters. This is how we would use it in our just defined VLU.
```C
VLU {

  For idvalid under    // // For all underscores
    Vary All;          // Vary everything that comes after an underscore

  For idvalid             // // For all idavlids
    Vary idvalid under    // Vary all the underscores that come after an idvalid
    Join All;             // Join everything that comes after an idvalid

}
```

Second, use `char` instead of `charclass` for selecting underscore.
```C
VLU {

  For '_' Char   // // For char '_'
    Vary All;    // Vary everything that comes after an underscore

  For idvalid        // // For all idvalids
    Vary '_' Char    // Vary if char '_' comes after an idvalid
    Join All;        // Join everything that comes after an idvalid

}
```
For using a `char` to select, the syntax is to type the `char` first and then the keyword `Char`.


<br>

#### Saving a VLU
VLU Block is used only to define a VLU. All the definitions you make in a VLU block are dynamic, and as soon as you exit the block, the definitions automatically free. Thus, we save the VLU in a static memory before we exit the block.

To save a VLU, we use the keyword `Make`,
```C
VLU {

  For '_' Char
    Vary All;

  For idvalid
    Vary '_' Char
    Join All;

  Make(token_reader, 10, 10);

}
```
`Make` takes in three arguments, first is the name of the variable you wish to store the VLU in. And the remaining two we will discuss in a bit.

<br>

The scope of the variable `token_reader` is as that of the VLU block. Therefore, VLU defined in the `main` function can not be accessed outside the `main` function. And VLU can not be defined in the global scope, therefore using `Make`, we can not create globally accessible VLUs.

To solve this problem, we can _declare_ the VLU in the global scope as,
```C
Decl(token_reader, 10, 10);
```
and then _save_ the VLU in the main function.
```C
int main (int argc, char * argv[]) {

  VLU {
  
    For '_' Char
      Vary All;
  
    For idvalid
      Vary '_' Char
      Join All;
    
    Save(token_reader);
  
  }
  
  /* code */

  return 0;

}

```
Now, the VLU can be accessed globally. Also at the time of saving, we do not need to pass three arguments. But what are those arguments ?

<br>

In reality, VLU is a buffer that is **_being treated as a 2-D array_**, therefore, you have to define the size of the buffer at the time of declaration.
- The second argument is the number of maximum rules the VLU can have.
- The third argument is the number of maximum sub-rule the VLU can have.


> It is C's limitation that we have to define the buffer size at the time of declaration, not automation can work here.

<br>

In our just defined VLU, the maximum number of rules is `2` and of sub-rules is also `2`, hence, the most memory efficient VLU saving would be of size,
```C
VLU {

  For '_' Char        // Rule 1:
    Vary All;           // Sub-rule 1

  For idvalid         // Rule 2:
    Vary '_' Char       // Sub-rule 1
    Join All;           // Sub-rule 2

  Make(token_reader, 2, 2);      // Saving the VLU

}
```

<br>

#### `Set` Operator
...

<br>

#### `Call` Operator
...

<br>

#### Printing the VLUs
...

<br>

<hr>

### Collecting Tokens

<hr>

...

<br>

#### `Using` and `Collect` Keyword
...


<br>

#### Verlet File I/O
...





Keywords related to File I/O _(Verlet File I/O)_
```C
Open    OpenPtr   Opened
Mode    In
Reader  ReadChar   ReadLine   Once
UpdateReaderPos  ReaderPosType  ReaderPos  ReaderGoTo  ResetReader
CloseReader
Writer  WriteChar  WriteLine  WriteFormat
UpdateWriterPos  WriterPosType  WriterPos  WriterGoTo  ResetWriter
CloseWriter
```
...


<br>

#### Using Verlet File I/O to collect tokens
...


<br>

#### Printing the VLUs
...

<br>


<hr>

### Operations on Collected Tokens

<hr>

...

<br>

#### Printing Tokens
...


<br>

#### Comparison of Tokens
...


<br>

#### Membership of Tokens
...


<br>

#### Hash of Tokens _(Non-hosted VScript)_
...

<br>


<hr>

### Finally

<hr>

...