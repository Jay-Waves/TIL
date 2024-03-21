## 7 Loops

`for` Syntax: 有很多种.

```bash
for x := 1 to 10 do
begin
  statements
end

for name [in list]
do
  statements that can use $name
done

for (( initialisation ; ending condition ; update ))
do
  statements...
done
```

`while` Syntax:

```bash
while condition; do
  statements
done
```

`until` Syntax:

```bash
until condition; do
  statements
done
```

### `seq`

生成一个数字序列, 通常用于生成特定循环区间:

```bash
seq First [Step] Last

# 生成 3\n4\n5
seq 3 5

# 生成 3\n5
seq 3 2 5
```