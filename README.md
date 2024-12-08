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
- they should have a std::ostream << operator. If this is not already defined for the type it should be defined as a friend function of the class:
  friend ostream& operator<<(ostream& out, const T& data) {} // T is the defined type

For values:
- they should be default constructible
- they should be copy constructible
- they should be copy assignable
- they should also have the ostream operator (see above)

5. Null value

If a key is not contained within binary search tree, when querying the corresponding value (method getNodeValue()) a null value will be returned. This is an alternative to triggering an exception which would need handling within a try-catch block.

The value of the null value can be setup by user when constructing the tree. If the user prefers not to provide any specific value to be used as null value, then the default value of the key type will be used as null value (for example if the key is a std::string the null value is the empty string ""). For this reason it is mandatory to use key types that are default constructible (see previous section).

The null value cannot be changed once the tree got initialized. Setting a null value within tree (see method addOrUpdateNode()) has no effect: no new node will be created, respectively the value of an existing node will not get updated.
