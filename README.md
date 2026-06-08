# Verlet Lexer

#### version v1.1.0

In contrast to the last version, the addition of 8th SVH, ```svh_08_cimpl.h``` allows fast and secured token recognition.

<br>

### Addition
- Macro `token_not_in` : Similar to how the keyword `token_is` has its negative variant `token_is_not`, `token_in` now has its negative variant `token_not_in`.

<br>

- Header `svh_08_cimpl.h` : Implements the standard Verlet Framework to provide token recogintion utilities for C code.

    <br>

    - Token Lists for each token types :
        - Static[9] : `DataTypes`
        - Static[6] : `StorageClasses`
        - Static[4] : `TypeQualifiers`
        - Static[13] : `Contorls`
        - Static[4] : `Structures`
        - Static[1] : `Specifiers`
        - Static[6] : `CompileTimeOperators`
        - Static[2] : `Assertion`
        - Dynamic : `Function`
        - Dynamic : `Macros`
        - Static[15] : `Pre_Processor_Commands`
        - Static[4] : `Pre_Processor_Operator`

    <br>

    - Macro `Handle_Pre_Processing` : Allows you to handle pre-processing commands do.
    Currently, you will have to `#undef` the macro `Handle_Pre_Processing` and then implement handling, but in later version it will be having its own designated workflow.

    <br>

    - VLUT Helping Macros are added : Put these inside a VLUT definition and they will initialise the corresponding token recognition
        - Macro `Scan_Identifiers_3` : Scans all the identifiers.
        - Macro `Scan_Spaces_2` : Scans all the spaces and clubs all the adjacent spaces.
        - Macro `Detect_Number_System_4` : Scans the prefixes such as `'0b` in front of numbers.
        - Macro `Scan_Digits_3` : Scans the digits only.
        - Macro `Scan_Numbers_2_4` : Scans the prefixes such as `'0b` in front of numbers and the digits both.
        - Macro `Else_Vary_1` : Handles the `VLUT case not registered` error by varying all the default cases.
        - Macro `Else_Join_1` : Handles the `VLUT case not registered` error by joining all the default cases.



<br>

### Fixes
- `token_in` : Earlier it did not support overloading, now it does. Both static and dynamic token lists can be passes to it.
`VCo_enable_{name}_size` stores the size of the dynamic token list, `{name}` is the name of the token list.