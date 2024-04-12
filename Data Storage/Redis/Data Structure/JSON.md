Redis 支持无缝集成 JSON 格式, 通过 `JSONPath` 访问键, 并以树形数据结构存储 (即大部分操作是 `O(logn)` 的. Redis 对内部数据的操作皆有原子性.

配置方法见 [config](../config.md)

操作字符串:
```bash
# note that $ means root of json, which is a list
> json.set animal $ '"dog"'
OK
> json.get animal $
"[\"dog\"]"
> json.type animal $
1) "string"

> json.strlen animal $
1) "3"
> json.strappend animal $ '" (Canis familiaris)"'
1) "22"
> json.get animal $
"[\"dog (Canis familiaris)\"]"
```

操作整型:
```bash
> json.set num $ 0
OK
> json.numincrby num $ 1
"[1]"
> json.numincrby num $ -0.75
"[0.25]"
> json.nummultby num $ 4
"[1]"
```

访问复杂结构:
```bash
> json.set example $ '[ true, {"anwser":42}, null ]'
OK
> json.get example $
"[[true, {\"answer\":42}, null]]"
> json.get example $[1].answer
"[42]"
> json.del example $[-1]
(integer) 1
> json.get example $
"[[true, {\"answer\":42}]]"
```

操作列表:
```bash
> json.Set arr $ []
> json.ArrAppend arr $ 0
1) (integer) 1
> json.Get arr $
"[[0]]"

> json.ArrInsert arr $ 0 -2 -1
1) (integer) 3
>json.Get arr $
"[[-2, -1, 0]]"

> json.ArrTrim arr $ 1 1
1) (integer) 1
> json.Get arr $
"[[-1]]"

> json.ArrPop arr $
1) "-1"
>json.ArrPop arr $
1) (nil)
```

直接操作 json 对象:
```bash
> json.Set obj $ '{"name":"Leonard Cohen","lastSeen":1478476800}'
> json.ObjLen obj $
1) (integer) 2
> json.ObjKeys obj $
1) 1) "name"
   2) "lastSeen"
```