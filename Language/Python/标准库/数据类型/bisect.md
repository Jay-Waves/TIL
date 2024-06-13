## Bisection

访问列表需要 $O(n)$ 时间, bisection 库提供一种**折半查找算法**. 但须提前保证**列表**是**有序的**.

```python
x = list(range(10**6))
i = x.index(991234)

from bisection import bisect_left
i = bisect_left(x, 991234)
```

原型:
```python
# find position of `x` in `a[lo,hi]`, if `a` has many `x`, return the leftest
bisect_left(a, x, lo=0, hi=len(a))
bisect_right()

# insert
insort_left(a, x, lo=0, hi=len(a))
insort_right(...)
```