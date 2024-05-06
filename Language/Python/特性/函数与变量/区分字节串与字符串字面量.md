[More info about *Unicode* and *utf-8*](obsidian://open?vault=OOS&file=Hardware%26Convention%2FUnicode%20%E5%92%8C%20UTF-8%20%E4%B9%8B%E9%97%B4%E7%9A%84%E5%85%B3%E7%B3%BB)

python3 中处理 string 有两种字符类型:
- str: str 单指万国码 Unicode, 一个字符占用四个字节存储.
- bytes: bytes 指二进制编码, 即通过某种编码方式(默认是utf-8, 但不要理所当然认为)编码(压缩)的万国码. 其按字节存储, 每个字符对应的字节数不确定, 解压到str(Unicode)时需要指定编码方式 `str(bytes_var, "utf-8")`

两者转换常用的方法是 `str_var.encode(), bytes_var.decode()`, 输出结果如果形如`b'some_string'`, 那么它就是二进制形式, 不过被 python 输出时解码了.

### 前缀类型

-   1、在字符串前加上`r`或`R`表示该字符串是`非转义`的原始字符串。
-   2、在字符串前加上`u`或`U`表示该字符串是`unicode字符串`。