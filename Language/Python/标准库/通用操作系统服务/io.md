---
date: 24-06-13
---

python3 中有两种字符串类型:
1. `str`, 用万国码存储的字符串[^1]
2. `bytes`, 可能是编码后的 `str` 字符串, 也可能是纯二进制的非文本数据.

两者可以互相转换, `str` 转化为 `bytes` 时需要解码, 如使用 UTF-8 标准解码: `str(mybytes, "utf-8")`. Python 3.15 已经将 UTF-8  设为默认编码格式, 而不是当前语言区域所使用的编码格式.  多数 Unix 平台默认使用 UTF-8 同一格式, 但早期 Windows 在不同语言区域的编码格式并不一样[^3], 可使用 `encoding="local"` 来获取当前语言区域的编码格式[^2].

[^1]: 万国码详见[字符编码](../../../../HardWare/字符编码.md)

[^2]: `>= Python 3.10`

[^3]: Windows95/98 使用 GB2312, Windows 2000 后使用 GBK, Windows XP 后支持 GB18030 (中国政府推荐的版本), Windows10 开始使用 Unicode/UTF-8 统一标准. 详见[字符编码](../../../../HardWare/字符编码.md)

### 字符串前缀

|                | 含义                         |
| -------------- | ---------------------------- |
| `r'my_string'` | 字符串是不应转义的原始字符串 |
| `u'my_string'` | 字符串是万国码字符串, 未编码, 类型为 `str` |
| `b'my_string'` | 字符串是二进制串, 类型为 `bytes`                             |

## 文件 I/O

文本 I/O 预期生成 `str`, 透明地执行数据编解码, 并选择性转换平台换行符. 文本 I/O 比二进制 I/O 慢很多, 因为需要频繁调用字符串编解码器, 来在万国码和字符编码间转换, 因而 `seek()` 也很慢.

```python
# 读取文本文件时, 默认返回类型是 `io.StringIO`, 并对文本中的字符进行解码
try:
	f = open("myfile.txt", "r", encoding="utf-8") 
except IOError:
	# IOError 后不应再调用 f.close()
	...
	return
except UnicodeDecodeError
	...
finally:
	f.close()
```

直接构造 `StringIO`:

```python
f = io.StringIO("text in memroy")
```

## 二进制 I/O

也成为缓冲 I/O, 预期 `bytes`, 不执行编解码操作. 总是读取或写入较大块的数据, 即使仅请求单个字节, 从而利用缓存避免频繁的系统调用. 

```python
# 读取文件时选择用二进制流打开
f = open("me.jpg", "rb")
```

直接构造 `BytesIO`:

```python
f = io.BytesIO(b"initial binary data: \x00\x01")
```

`IOBase` 实际是一个[上下文管理器](../contextlib.md):

```python
with open(...) as f:
	...
```

## 接口

| ABC            | Inherits From  | Methods                               | Mixin                                                                                                                                                                    |
| -------------- | -------------- | ------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| IOBase         |                | `fileno`, `seek`, `truncate`          | `close`, `closed`, `__enter__`, `__exit__`, `flush`, `isatty`, `__iter__`, `__next__`, `readable`, `readline`, `readlines`, `seekable`, `tell`, `writable`, `writelines` |
| RawIOBase      | IOBase         | `readinto`, `write`                   | IOBase's Methods, `read`, `readall`                                                                                                                                      |
| BufferedIOBase | IOBase         | `detach`, `read`, `read1`, `write`    | IOBase's Methods, `readinto`, `readinto1`                                                                                                                                |
| TextIOBase     | IOBase         | `detach`, `read`, `readline`, `write` | IOBase's Methods, `encoding errors`, `errors`, `newlines`                                                                                                                |
| BytesIO        | BufferedIOBase |                                       |                                                                                                                                                                          |
| StringIO       | TextIOBase               |                                       |                                                                                                                                                                          |

### `fileno()` 

返回流的底层文件描述符 (fid), 如果对象不存在文件描述符, 触发 `OSError`

### `flush()`

刷新流的写入缓冲区, 对只读和非阻塞流不起作用

### `isatty()`

如果流是交互式的 (连接到了终端 tty 设备), 返回 `True`

### `close()`

刷新并关闭当前流, 重复调用无效, 调用后对该流的任何操作都会触发 `ValueError`.

值方法 `closed` 在流已关闭时, 返回 `True`.


### `readable()`

如果流可读取, 返回 `True`, 否则返回 `False`. 不可读时, 调用 `read()` 将返回 `OSError`

### `readline(size=-1)`

从流中读取一行, 如果指定了 `size`, 则至多读取 `size` 个字节. 对于二进制文件, 总是假定行结束符为 `b'\n'`; 对于文本文件, 可用 `open(newline=...)` 来自定义行结束符.


### `readlines(hint=-1)`

从流中读取并返回包含多个文本行的列表, 用 `hint` 指定最大读取行数. 该方法应用场景有限, 因为 `IOBase` 是可迭代的对象, 可以直接用 `for line in file: ` 来遍历.

### `seekable()`

如果流支持随机化访问, 就返回 `True`; 否则返回 `False`, 此时  `seek(), tell(), truncate()` 都会引发 `OSError`

### `tell()`

返回当前的流位置.

### `seek()`

`seek(offset, whence=os.SEEK_SET)` 基于 `whence` 位置和偏移 `offset` 来设置新的流位置, 并返回新的绝对位置.

- `os.SEEK_SET, 0` 指流的开头, 此时 `offset` 应大于等于 0
- `os.SEEK_CUR, 1` 指当前流位置.
- `os.SEEK_END, 2` 指流的结尾, 此时 `offset` 一般为负值.