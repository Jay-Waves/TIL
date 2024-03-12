### Sampler

最常用:
- Euler A (经典)
- DPM2 A, 需搭配合理步数
- DPM++, 新型算法

### Sampling Steps

**迭代次数并不是越高越好**, 如 DPM A, Euler A 是**非线性**迭代, 迭代次数超阈值后质量反而下滑; DDIM, Euler 等**线性**迭代, 则需要更多迭代次数

- DPM2 A: 建议 40+
- EulerA / DDIM: 建议 30~40
- DPM Solver: 建议 20~30