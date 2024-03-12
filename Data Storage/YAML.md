类似 Json, 不过更适合人类读写. YAML: yaml ain't a markup language, 这语言挺傻逼的.

基础规则: 
- 大小写敏感
- 使用缩进表示层级关系
- 缩进时不允许使用 Tab 键, 只允许使用空格.
- 缩进的空格数目不重要, 只要相同层级的元素左侧对齐即可

注释使用 `#`

支持三种数据结构:
- 对象: 键值对
- 数组
- 标量

### 对象

`key: value`, 类似 Json 中: `{key: 'value'}`

允许行内对象: `key: {subkey1: value, subkey2: value}`, 类似 Json: `{key: {subkey1: value}}`

### 数组

```
- cat
- dog
- goldfish
```

类似 Json 中:

```json
['cat', 'dog', 'fish']
```

嵌套:

```yaml
-
 - cat
 - dog
 - fish
```

类似 Json 中:

```json
[['cat', 'dog', 'fish']]
```

### 标量

- 数值
- 布尔值: `true`, `false` 实际支持很多写法
- null: `~`
- 时间: 采用 ISO8601 格式. `2001-12-14t21:59:43.10-05:00`
- 字符串: 默认不用引号, 也可以用. 

转换数据类型: `e: !!str 123`, 类似 Json: `{e: '123'}`

字符串换行: 写成多行, 次行必有一缩进(自动转化为空格), 空行则转化为换行符. `|` 保留换行符.

```yaml
obj: hello
 world!

obj1: |
 hello
 world!
```

类似 Json:

```json
{obj: 'hello world!', obj2: 'hello\nworld!'}
```

### 数据引用

`&` 用来建立锚点, `<<` 表示合并到当前数据, `*` 用来引用锚点.