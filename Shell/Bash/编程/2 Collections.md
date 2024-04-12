## 数组

Examples:

```bash
array[0]=val
array[1]=val
array[2]=val
array=([2]=val [0]=val [1]=val)
array=(val val val)
```

取址访问:

```bash
${array[i]}     # where i is the index
```

获取当前数组大小:

```bash
${#array[@]}
```

变量三元操作:
```bash
# 如果 str 不存在, 返回 expr; 否则返回 str.
${str-expr}
# 如果 str 不存在或为空 (""), 返回 expr; 否则返回 str.
# `:` 添加了对空 `""` 的检查.
${str:-expr}
# 如果 str 已经存在, 返回 str, 否则令 str=expr, 然后返回 str.
${str=expr}
# 如果 str 不存在(未初始化), 将 expr 输出到 stderr.
${str?expr}
# performs substring expansion. It returns the substring of $varname 
# starting at offset and up to length characters
${varname:offset:length}    
```

## 字符串

`""` 和 `''` 都用于定义字符串, 但 `''` 不进行任何变量展开(interpolation)和命令替换(command substitution), 纯字面量. 字符串皆支持直接跨行输入.

三元匹配操作:

```bash
# 从头匹配 pattern, 找到则删除
${variable#pattern}         
# 从头贪婪(尽可能长)匹配 pattern, 找到则删除
${variable##pattern}        
# 从末尾匹配 pattern, 找到则删除
${variable%pattern}         
# 从末尾贪婪匹配 pattern, 找到则删除
${variable%%pattern}        
# 用 string 替换贪婪匹配到的第一个 pattern
${variable/pattern/string}  
# 用 string 替换贪婪匹配到的所有 pattern
${variable//pattern/string} 
# 返回数组长度, 即字符串字符数量
${#varname}     
```

大小写转换:

```bash
# converts every letter in the variable to lowercase
${variable,,}  
# converts every letter in the variable to uppercase
${variable^^}    
```

索引:
```bash
# this returns a substring of a string, starting at the character at 
# the 2 index(strings start at index 0, so this is the 3rd character),
# the substring will be 8 characters long, so this would return a 
# string made of the 3rd to the 11th characters.
${variable:2:8}    
```

匹配字符串首尾:

```bash
#this returns true if the provided substring is *in* the variable
if [[ "$variable" == *subString* ]]  
#this returns true if the provided substring is not in the variable
if [[ "$variable" != *subString* ]]  
#this returns true if the variable starts with the given subString
if [[ "$variable" == subString* ]]   
#this returns true if the variable ends with the given subString
if [[ "$variable" == *subString ]]   
```

使用模式匹配简化匹配操作:

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
