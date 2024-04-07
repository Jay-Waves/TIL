[TOML, Tom's Obvious Minimal Language](https://toml.io/en/) 易读配置文件格式, 基于哈希表.

### 注释

```toml
# This is a TOML comment
```

### 字符串

```toml
str1 = "Name\tJos\u00E9\nLoc\tSF."
str2 = "You can \"quote\" me."
# multi-line strings
str1 = """
	first line
	another line \
	not another line"""
# literal strings: no escaping, what you see is what you get:
pth = 'C:\Users\nodejs\templates'
quoted = 'Tom "Dubs" Preston-Werner\c'
# multi-line litera strings:
re = '''I [dw]on't need \d{2} apples'''
```

### 标量

```toml
# integers
int1 = +99
int2 = 42
int3 = -17

# hexadecimal with prefix `0x`
hex1 = 0xDEADBEEF
hex2 = 0xdeadbeef
hex3 = 0xdead_beef

# octal with prefix `0o`
oct1 = 0o01234567
oct2 = 0o755

# binary with prefix `0b`
bin1 = 0b11010110

# fractional
float1 = +1.0
float2 = 3.1415
float3 = -0.01

# exponent
float4 = 5e+22
float5 = 1e06
float6 = -2E-2

# both
float7 = 6.626e-34

# separators
float8 = 224_617.445_991_228

# infinity
infinite1 = inf # positive infinity
infinite2 = +inf # positive infinity
infinite3 = -inf # negative infinity

# not a number
not1 = nan
not2 = -nan
```

### 时间格式

```toml
# offset datetime
odt1 = 1979-05-27T07:32:00Z
odt2 = 1979-05-27T00:32:00-07:00
odt3 = 1979-05-27T00:32:00.999999-07:00

# local datetime
ldt1 = 1979-05-27T07:32:00
ldt2 = 1979-05-27T00:32:00.999999

# local date
ld1 = 1979-05-27

# local time
lt1 = 07:32:00
lt2 = 00:32:00.999999
```

### 表 (字典)

常规表由头部 `headers` 定义

```toml
[table-1]
key1 = "mystring"
key2 = 1

[table-2]
key1 = "mystring"
key2 = 2

[fruit.apple]
smooth=True
# same as "{"fruit": {"apple":{"smooth":True}}}" in json
```

使用键值点序列的方式来定义子哈希表

```toml
# 就像json格式的: {"fruit": {"apple":1, "orange":2"}}
fruit.apple=1
fruit.orange=2

# 由于 fruit.apple 已被视为键而不是表, 下列语句是无效的
fruit.apple.sweet=True # invalid

# 同一表中无法重复定义
fruit.apple=4 # invalid

# 如果已经使用了 fruit.apple, 重复定义无效
[fruit.apple] # invalid
```

### 数组

```toml
integers = [1, 2, 0.1]
strings = [
	"all",
	'strings',
	"""are the sanme""",
	'''type''', # terminating comma is OK!
	]
```