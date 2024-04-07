> File at [paper/FastTrack: Efficient and Precise Dynamic Race Detection](../../../paper/FastTrack,%20Efficient%20and%20Precise%20Dynamic%20Race%20Detection.pdf)
> Cormac Flanagan and Stephen N. Freund. 2009. FastTrack: efficient and precise dynamic race detection. SIGPLAN Not. 44, 6 (June 2009), 121–133. https://doi.org/10.1145/1543135.1542490

- [Eraser](Eraser,%20A%20Dynamic%20Data%20Race%20Detector%20for%20Multithreaded%20Programs.md): low cost, low precision
- [Happens Before](Time,%20Clocks,%20and%20the%20Ordering%20of%20Events%20in%20a%20Distributed%20System.md): high cost, high precision
- FastTrack: low cost, high precision

FastTrack: Efficient and Precise Dynamic Race Detection
- sound & complete: 尽快地在每个变量上发现至少一个数据竞争.
- effcient
- Insight:
	1. Happens-Before 是一种偏序关系.
	2. 但对变量的访问几乎全是有序的.