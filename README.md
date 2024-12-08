1. Introduction

This is a generic implementation of binary search trees.

Three binary search trees variants are contained within this project:
- "simple" (unbalanced) binary search tree
- red-black tree
- AVL tree

2. Disclaimer

This library has been mainly created for learning purposes. Please don't use this code in any mission-critical environments. I will not be held responsible for any damage it might cause whatsoever.

3. Requirements

Minimum required C++ standard is C++20.
In order to build and run the unit tests, the Qt framework should be installed (preferably Qt 6).

4. Supported key and value types

Both keys and values should satisfy some constraints in order to be supported with the binary search trees.

For keys:
- they should be copy constructible
- they should be totally ordered. In order to achieve this, following operators should be defined (if not already intrinsic to the type, i.e. for primitive types or standard library C++ classes for which the operator has already been implemented):
  - operator ==
  - either <=> or all four comparison operators: <, <=, >, >=
- they should have a std::ostream << operator. If this is not already defined for the type it should be defined as a friend function of the class: friend ostream& operator<<(ostream& out, const T& data) {} // T is the defined type

For values:
- they should be default constructible
- they should be copy constructible
- they should be copy assignable
- they should also have the ostream operator (see above)
