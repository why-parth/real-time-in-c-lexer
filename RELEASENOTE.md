# Verlet Lexer

#### version v1.1.0

In contrast to the last version, the addition of 8th SVH, ```svh_08_cimpl.h``` allows fast and secured token recognition.

<br>

### Addition
- Macro `formatted_string_of_collected` : Now you can print the recently collected string.

<br>

- Macro `Call` : VLUs can now command the collect function to call a function, this makes dynamic VLU state changes extreemly convinient.


<br>

- Macro `VS_OFF_CHARCLASS_ERR` : Allows you to toggle the error handling of the invalid charclass error.

<br>

- **Library files are now managed in a more oriented way.**

<br>

- Header `_07_ccast.h` : This header is (!) Not Completed (!), but, it still alows the code to be optimized to some extent.

<br>

### Fixes
- VLU Max Value segmentation bug : In the VLU block, when the number of values exeeded the maximum number of values, the dynamic pointer did not get reallocated correctly. Therefore, higher value count did not work.
VLU casting resolved, The casting did not work as the buffer allocation was not taking into account the extra 2 avsme that store key and number of values, this caused the VLU to crash if large VLUs were defined.

<br>

- `For`, `Join` and `Vary` now have EXIST mask on by default.

<br>

<hr>

<br>

### Note

#### This is a major upgrade from the last release.

<br>

##### Version 1.2.0 did not get released.
