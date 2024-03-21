## 6 Conditionals

The conditional statement in bash is similar to other programming languages. Conditions have many form like the most basic form is `if` expression `then` statement where statement is only executed if expression is true.

```bash
if [ expression ]; then
    will execute only if expression is true
else
    will execute if expression is false
fi
```

Sometime if conditions becoming confusing so you can write the same condition using the `case statements`.

```bash
case expression in
    pattern1 )
        statements ;;
    pattern2 )
        statements ;;
    ...
esac
```

Expression Examples:

```bash
statement1 && statement2  # both statements are true
statement1 || statement2  # at least one of the statements is true

str1=str2       # str1 matches str2
str1!=str2      # str1 does not match str2
str1<str2       # str1 is less than str2
str1>str2       # str1 is greater than str2
-n str1         # str1 is not null (has length greater than 0)
-z str1         # str1 is null (has length 0)

-a file         # file exists
-d file         # file exists and is a directory
-e file         # file exists; same -a
-f file         # file exists and is a regular file (i.e., not a directory or other special type of file)
-r file         # you have read permission
-s file         # file exists and is not empty
-w file         # you have write permission
-x file         # you have execute permission on file, or directory search permission if it is a directory
-N file         # file was modified since it was last read
-O file         # you own file
-G file         # file's group ID matches yours (or one of yours, if you are in multiple groups)

file1 -nt file2     # file1 is newer than file2
file1 -ot file2     # file1 is older than file2

-lt     # less than
-le     # less than or equal
-eq     # equal
-ge     # greater than or equal
-gt     # greater than
-ne     # not equal
```
