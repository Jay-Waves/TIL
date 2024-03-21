## Functions

As in almost any programming language, you can use functions to group pieces of code in a more logical way or practice the divine art of recursion. Declaring a function is just a matter of writing function my_func { my_code }. Calling a function is just like calling another program, you just write its name.

```bash
function name() {
    shell commands
}
```

Example:

```bash
#!/bin/bash
function hello {
   echo world!
}
hello

function say {
    echo $1
}
say "hello world!"
```

When you run the above example the `hello` function will output "world!". The above two functions `hello` and `say` are identical. The main difference is function `say`. This function, prints the first argument it receives. Arguments, within functions, are treated in the same manner as arguments given to the script.