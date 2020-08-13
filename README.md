# eme
A fast low-level programming languages with strong macro support and a dynamic compile-time environment. The goal is to create a sort of fusion of Lisp and C, where code can be changed freely at compile-time with macros, but becomes fast, low-level code so that the language runs as fast as C.

[![Run on Repl.it](https://repl.it/badge/github/niko-niko-ni/eme)](https://repl.it/github/niko-niko-ni/eme)

Examples of the programming language's use can be found in the `examples/` directory.

### eme: Where did the name come from and what does it mean?

The word "eme" means "language" or "tongue" in Sumerian, the oldest known written language.

### I've used macros before in C and C++ and they don't seem so special! What makes eme different?

In languages like C and C++ macros are very limited and have their own special syntax. In eme, macros are just code that is run at compile-time, meaning they have full access to the programming language and the compile-time environment. They can define new functions, datatypes, and macros, making them *far* more powerful than macros in most other languages.

### Why are macros so important?

Macros allow you to extend the language as you're writing your program. As your program grows, the language grows with it. For more information about this style of programming read [Paul Graham's essay "Programming Bottom-Up"](http://www.paulgraham.com/progbot.html). Some relevant quotes from that essay are provided below:

> [When] language and program fit one another well, you end up with code which is clear, small, and efficient.

> In typical code, once you abstract out the parts which are merely bookkeeping, what's left is much shorter; the higher you build up the language, the less distance you will have to travel from the top down to it.

> By making the language do more of the work, bottom up design yields programs which are smaller and more agile. A shorter program doesn't have to be divided into so many components, and fewer components means programs which are easier to read or modify. Fewer components also mean fewer connections between components, and thus less chance for errors there.

Many of the programming language examples found in `examples/` make use of macros, such as the list comprehension example found in `examples/lcomp.eme`.
