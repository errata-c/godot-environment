A better method of initialization for the environments is needed, specifically one where a dictionary of initialization parameters can be passed over to configure the environment.

Create a `gdscript` class to represent action or observation spaces, provide a `_get` method to get members of the space easily.

Create a `gdscript` class to represent a reference to values within the action or observation space, provide a get and set method for accessing data within it. Provide methods to access subsets of the value tensor (`row`, `col`, `slice`), which should accept integers or arrays of integers to select the specific subset (the built-in `range` function returns an Array).



The Space class has been simplified.

Expand the Value class to include a range for the values. Give it the ability to represent any of a set of fundamental types. Give it a range of different constructors and assignment capabilities.

Test its new capabilities. Provide a mechanism for static binding of the value class and the space class, such that a concrete interface is given for the exact types, and the exact elements of the space.
