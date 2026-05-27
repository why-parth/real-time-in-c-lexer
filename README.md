
# Verlet Lexer
Verlet Lexer is a C framework that enables token recognition. It is commonly referred to as _Vlex_ and is a component of a unification called the _Verlet Framework_.

A lexer is any implementation that breaks down text data into fragmented data, into fragments called _tokens_. Verlet Lexer does the same but in a highly modifiable manner, being a C framework, it integrates with C to provide system level control along with token recognition which allows the making-of compilers, _transpilers_, interpreters, anything that relies on token recognition.

## Token Recognition in Verlet Lexer
Token reading in Vlex can be understood as reading a text _( an ASCII char buffer )_ char by char and for each char _( selected char )_, scanning the next char _( target char )_. Then we compare the selected char and the target char to see if it should either _join_ or _vary_.

For a better intuition, let us entertain a simple C instruction, `int a = 45;`, in this instruction, the tokens are `int`, `a`, `=`, `45`, `;`, let us see how Vlex performs tokenization on this instruction.

We read each char one by one, pseudo-code is being used to demonstrate the algorithm,
```
for c in "int a = 45;":
	print(c)
```
>```int a = 45;```

Then we add a condition to know if we shall treat the current been-read characters as one with the next character or not,
```
for c in "int a = 45;":
	if c is not of the same token:
		print('\n')
	print(c)
```
>`int`
>` `
>`a`
>` `
>`=`
>` `
>`45`
>`;`

>In C, we do not treat `' '` as a valid token, but the above loop is primitive and scans all the possible tokens out of a valid C code.

The flow of token recognition implemented some instructions that are used in the final form of Verlet Lexer. `c is not of the same token` is the same as asking of the current token varies on the next char or not.

In the most simplest form, a token recognition in Verlet Lexer can be represented as,
```C
char text[] = "int a = 45;";

char buffer[64];
int buffer_i = 0;
for (int i = 1; text[i]: i++) {

	if ( variation(text[i - 1], text[i]) ) {
		buffer[buffer_i] = 0;
		printf("'%s', ", buffer);
		buffer_i = 0;
	}
	
	buffer[buffer_i] = text[i];
	buffer_i++;
}
```
>`'int', ' ', 'a', ' ', '=', ' ', '45', ';', `

In the above code, the desired output will be only attained if and only if the function `int variation(char a, char b)` outputs the correct _variations_ per each input. variation is a boolean value that is assigned for each char pair. for example, if the pair (`a`, `b`) has variation equal to 1, then it means that if the selected char is a `a` then break the token if the target char is `b`. similarly, if the pair (`a`, `c`) has variation equal to 1, then it means that if the selected char is a `c` then continue appending the token.

Verlet Lexer assumes that throughout all the text, the rules for token recognition remains the same, thus, determination of whether a char belongs to the being-read token depends completely on what `variation` encodes.

`variation` does not exist in this framework, a data structure called VLUT is used to define the behavior of the lexer.

## `charclass`
In this framework, all the characters of the ASCII table must be grouped in various sets called _char classes_, and that grouping must follow the specification:

```uint_least16_t charclass(char c)```
>`uint_least16_t` is a memory layout defined in `stdint.h`, but the return memory layout of `charclass` could be any memory layout that has has at least 16 bits.

Definition of char classes requires the definition of the `charclass` macro. Vlex header do not function if `charclass	` isn't defined.

### Verlet Char Class
Vlex comes with a standard implementation of `charclass` called `VERLET_charclass` _( Verlet Char Class )_.
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

## Vscript
Verlet Script _( Vscript )_ is a DSL, a domain specific language, that handles,

  

* Behavior of the Lexer : This is done via a data structure called **VLUT**  _(Variation Look Up Table)_. VLUTs can be defined and are seen as a context on the basis of which the tokens are scanned. Multiple VLUTs are definable, one may switch the between different VLUTs according to needs.

  

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



### ```For```, ```Join``` and ```Vary``` Operations

Entire definition of a VLUT can be done using just these simple rules:
* All the VLUT definition must be be enclosed in the `Vlut` block.
* To select a set of characters, use the `For` keyword along with the desired char class.
* To apply an operation to a set of characters, use the `Join` or `Vary` keyword along with the desired char class.
	* `Join` means that the following char class will combine with the _selected_ char class to make the token.
	* `Vary` means that the following char class will NOT combine with the _selected_ char class terminating the token.

```
Vlut {
	For idvalid
		Join idvalid
		Join numeric;
}
```

Above written code is a completely valid _( initialized )_ C code, `idvalid` and `numeric` are char classes that we wish to apply the operations on. For example `For idvalid Join numeric;` means that all `numeric`s will be connected to the `idvalid`s.

### ```All``` Operation
* `All` is used to select all the characters.

```
Vlut {
	For All
		Join idvalid
		Vary All;
}
```
We read the text and for each pair ( selected char, target char ) we calculate the variation, it its 0, we keep appending the token but if its 1, then we stop the reading the examine the token.







