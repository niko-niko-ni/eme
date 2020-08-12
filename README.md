# eme
A fast low-level programming languages with strong macro support and a dynamic compile-time environment. The goal is to create a sort of fusion of Lisp and C, where code can be changed freely at compile-time with macros, but becomes fast, low-level code so that the language runs as fast as C.

[![Run on Repl.it](https://repl.it/badge/github/niko-niko-ni/eme)](https://repl.it/github/niko-niko-ni/eme)

Examples of the programming language's use can be found in the `examples/` directory.

### eme: Where did the name come from and what does it mean?

The word "eme" means "language" or "tongue" in Sumerian, the oldest known written language.

### Why are macros so important?

Macros allow you to extend the language as you're writing your program. As your program grows, the language grows with it. For more information about this style of programming read [Paul Graham's essay "Programming Bottom-Up"](http://www.paulgraham.com/progbot.html). Many of programming language examples make use of macros, such as the list comprehension example. (found in `lcomp.eme`)
