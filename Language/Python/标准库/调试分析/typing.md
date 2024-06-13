加强 Python 类型检查, 使 IDE 类型提示更加友好.

## 类型别名

python 的内置类型都是类对象, 可以赋值给变量, 就像起了一个 "别名".

```python
Vector = list[float]
```

旧版本使用 `TypeAlias` 来显示说明类型别名, 在 Py3.12 已被弃用.

```python
from typing import TypeAlias
```

Py3.12 引入了 `type` 关键字, 直接定义类型. 此后静态类型检查器会将他们视为同等类型.

```python
type Vector = list[float]
```

## 定义新类型

用 NewType 定义新类型, 此时类型检查器会视新类型为原类型的子类. 此时, 父类不能用于需要子类的位置, 否则静态类型检查会报错. 

```python
from typing import NewType
Int32 = NewType('Int32', int)
```

子类型可以执行父类的所有操作, 但返回的是父类型. 此处 `Int32` 是一个可调用对象, 它**立即返回传入的任何参数**, 所以没有开销, 仅用于类型检查.

```python
out: int = Int32(12) + Int32(123)

12 is Int32(12) # True
```

## 标注元组

`list` 类型只接收一个参数, 并假设容器中所有元素都是该类型, 实际上 Python 更灵活.

```python
x: list[int] = [] 
```

`Mapping` 接收两个参数,  一个键类型, 一个值类型.

```python
from collections.abc import Mapping
y: Mapping[str, int | str] = {} 
```

`tuple`  类型可接收任意长度参数, 该长度直接关系到数据长度. 若要模仿 `list` 行为, 即假设元组中元素类型相同并且不假定长度, 需要跟随参数 `...`

```python
z: tuple[int, str, int] = (1, '2', 3)
a: tuple[T, ...]
b: tuple  # 等价于 tuple[Any, ...]
c: tuple[()] = ()
```

## Any

`Any` 类型与所有类型相互兼容, 未指定类型都隐式地默认使用 `Any`. 当 `Any` 赋值给其他更精确类型时, 不执行类型检查. 

Python 中所有类型都是 `object` 的子类型, 但是 `object` 不是其他类型的子类型. 当值的类型为 `object` 时, 类型检查器几乎会拒绝所有对它的操作, 并且也不能将其赋值给更精确的类型变量 (`TypeError`). 相反, `Any` 是双向兼容的, 混用动态和静态代码时, 应将 `Any` 作为应急出口.

## 名义子类型与结构子类型

PEP484 将 Python 静态类型系统定义为 **名义子类型**, 当且仅当 A 为 B 的子类时, 才能在类 B 预期时使用类 A. 但该方法要求定义类时必须显式指明继承:

```python
from collections.abc import Sized, Iterable, Iterator

class Bucket(Sized, Iterable[int]):
	def __len__(self) -> int: ...
	def __iter__(self) -> Iterator[int]: ...
```

PEP544 允许在类定义时不显式说明基类, 即所谓的[鸭子类型](../../../类型系统.md), 也称为**结构子类型**.

```python
class Bucket:
	def __len__(self) -> int: ...
	def __iter__(self) -> Iterator[int]: ...
# Bucket 即是 Sized 类型, 因为定义了 __len__
# Bucket 也是 Iterator 类型, 因为定义了 __iter__
```

## 其他模块内类型

`Typing.Never`, `Typeing.NoReturn` 指一个函数绝对不返回

```python
def stop() -> Never:
	raise RuntimeError('no way')
```

`Typing.Self` 返回当前闭包中的类对象


`Typing.Union`

`Typing.Optional`

`Typing.Literal`

`Typing.ClassVar`

`Typing.Annotated`

`Typing.TypeGuard`

`Typing.Unpack`

`typing.cast(typ, val)`

`typing.assert_type(val, type, /)`

`typing.assert_never(arg, /)`

