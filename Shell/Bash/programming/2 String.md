## 3 String Substitution

Check some of the syntax on how to manipulate strings

```bash
${variable#pattern}         # if the pattern matches the beginning of the variable's value, delete the shortest part that matches and return the rest
${variable##pattern}        # if the pattern matches the beginning of the variable's value, delete the longest part that matches and return the rest
${variable%pattern}         # if the pattern matches the end of the variable's value, delete the shortest part that matches and return the rest
${variable%%pattern}        # if the pattern matches the end of the variable's value, delete the longest part that matches and return the rest
${variable/pattern/string}  # the longest match to pattern in variable is replaced by string. Only the first match is replaced
${variable//pattern/string} # the longest match to pattern in variable is replaced by string. All matches are replaced
${#varname}     # returns the length of the value of the variable as a character string
```

## 4. Other String Tricks

Bash has multiple shorthand tricks for doing various things to strings.

```bash
${variable,,}    #this converts every letter in the variable to lowercase
${variable^^}    #this converts every letter in the variable to uppercase

${variable:2:8}  #this returns a substring of a string, starting at the character at the 2 index(strings start at index 0, so this is the 3rd character),
                 #the substring will be 8 characters long, so this would return a string made of the 3rd to the 11th characters.
```

Here are some handy pattern matching tricks

```bash
if [[ "$variable" == *subString* ]]  #this returns true if the provided substring is in the variable
if [[ "$variable" != *subString* ]]  #this returns true if the provided substring is not in the variable
if [[ "$variable" == subString* ]]   #this returns true if the variable starts with the given subString
if [[ "$variable" == *subString ]]   #this returns true if the variable ends with the given subString
```

The above can be shortened using a case statement and the IN keyword

```bash
case "$var" in
    begin*)
        #variable begins with "begin"
    ;;
    *subString*)
        #subString is in variable
    ;;

    *otherSubString*)
        #otherSubString is in variable
    ;;
esac
```
