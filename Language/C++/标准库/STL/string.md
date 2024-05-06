## String

**字符类型:**
- `char`: 普通字符.
- `wchar_t`: 宽字符, 用来存储编码更长的字符.

使用:
```cpp
#include <string>
#include <iostream>

std::string my_str='yjw';
std::cout << my_str << std::endl;

std::string my_str(n, my_char); //初始化为 n*my_char
```

c++ 使用缓冲区来优化输入, 只有缓冲区刷新时屏幕才会输出. 刷新条件有:
- 缓冲区已满
- 请求从标准输入流读入数据
- 明确要求刷新

**内置方法:**
- `size()` 字符数量
- `out<<my_str`, `in>>my_str`. 