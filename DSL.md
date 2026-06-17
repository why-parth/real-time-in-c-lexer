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
```
charclass
VLU   For   Join   Vary   Char   All   Set   Call
Decl  Save  Make
Show  ShowVLU
```
Keywords related to global hash variable _(Non-hosted VScript)_
```
hash_reset  hash_push
hash_value  hash_none
hash_of     hash_function
```
Keywords related to token comparison
```
token_is  token_is_not
expect    keep_expecting    stop_expecting
```
Keywords related to token membership
```
token_list
token_in  token_not_in
```
Keywords related to token management
```
token_paste
token_print
token_show
```
Keywords related to Collection
```
Using
Collect
```

<br>

<hr>

### `charclass`

<hr>

`charclass` is a ...

`charclass` is a state, like VLU, it is definable and manipulable. However this state MUST be defined. If VLU is not defined, the program will compile just fine, but if `charclass` is not defined, the program will not compile.

>>>>> `THIS  SECTION  IS  NOT  YET  WRITTEN`

<br>

<hr>

### Defining a VLU

<hr>

Defining VLUs is Verlet Lexer's heart. VLUs definition is extremely rich in functionality, VLUs alone make the Verlet Lexer a state of art. Usually, at the lexical analysis phase, you can not achieve much context, but in Verlet Lexer that is not the case.

<br>

#### VLU Block
The first step of defining a VLU is to open a VLU block, all VLU definitions happen in a VLU block. A VLU block is opened as,
```C
VLU {
  // All the VLU definition comes here
}
```

<br>

#### For Join Vary All
The first step of defining a VLU is to open a VLU block, all VLU definitions happen in a VLU block. A VLU block is opened as,
```C
VLU {
  // All the VLU definition comes here
}
```






<!---
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
---!>
