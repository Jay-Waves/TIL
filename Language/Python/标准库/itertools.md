
```python
from itertools import *
```

## Link Iterators together

```python
# link many iterable objects
chain([1,2,3], [4,5,6])

# repeatedly iterate given object
from item in cycle('AB'):
	print(item, end=' ')
>>> A B A B A B ....

# build more iterators from one
data = 'ABC'
it1, it2 = tee(data, 2)

# similar to `zip`, can also handle objects of different lengths
for item in zip_longest('A', '12', fillvalue='-'):
	# default fillvalue is None
	print(item)
>>> ('A','1'), ('-','2')
```

## Filter items from an iterator

```python
# stop iterating while the predicate returns Ture for the first time
takewhile(predicate, iterable)

# begin to iterate while the predicate returns False for the first time.
dropwhile(predicate, iterable)

# select part of iterable object, without copping, like `span` in c++
for item in islice('ABCDEF', 2, 5):
	pritn(item, end=' ')
>>> C D E

# return item if predicate(item) is True, opposite of `filter`
filterfalse(predicate, iterable)
```

## Combinations of items from iterators

```python
for item in product('AB', repeat=2):
	print(''.join(item), end=' ')
>>> AA AB BA BB

for item in permutations('AB', 2):
	print(''.join(item), end=' ')
>>> AB BA

combination(
```