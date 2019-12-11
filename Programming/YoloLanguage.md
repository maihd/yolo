# Yolo language - a C/C++ dialect, programming with C++11 compiler

1. Pass-by-value prefer to pass-by-reference/pass-by-pointer
2. Pointer type is mean to make a loan for the function execution (learn more from Rust Ownership)
3. No OOP, no classes, no methods, no static methods, no free functions (just functions), no inherits (extend by composition), no encapsulations (all thing in header is public, internal and private is the same)
4. Code and data is seperation. Data sometime come with an namespace that contain operations working on this data. Example: String and StringOps, MathTypes and MathOps.
5. Avoid change data
6. Pure function is prefered