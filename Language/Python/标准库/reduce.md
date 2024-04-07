`from functools import reduce`
`reduce(funct, seq)`, 对seq序列中元素使用funct进行累积操作, 比如将seq序列中的元素累乘.

例子:
- 累乘: `reduce((lambda x, y: x*y), list)`
- 累加: `reduce((lambda x, y: x+y), list)`
