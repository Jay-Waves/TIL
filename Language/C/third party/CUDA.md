```c
__global__ void kernel_func()
{
	printf("...");
}

int main()
{
	kernel_func<<<1, 1>>>(); 
	// CUDA Kernel: <<<grid_size, block_size>>>
	// grid_size: numbers of block
	// block_size: numbers of threads per block
}
```

![](../../../attach/Pasted%20image%2020240513155528.png)

GPU 包含更多的运算单元, CPU 负责处理逻辑而 GPU 负责协同其计算. CPU 及其内存称为 host, GPU 及其内存称为 device. 典型 CUDA 程序流程如:
1. 分配 host 内存, 初始化数据.
2. 分配 device 内存, 从 host 将数据拷贝到 device.
3. 调用 CUDA Kernel 函数, 在 device 上完成运算. 核函数用 `__global__` 声明.
4. 将 device 结果拷贝到 host.
5. 释放 device 和 host 上分配的内存.

- `__global__ void()` 在 host 调用, 在 device 执行. 是异步的.
- `__device__` 在 device 上调用执行, 不能和 `__global__` 同时用.
- `__host__` 在 host 上调用执行. 缺省值, 也不能和 `__global__` 同时用, 可以和 `__device__` 同时用.

## 内存托管

> cuda 6.0

https://developer.nvidia.com/blog/unified-memory-in-cuda-6/

## 例子: 矩阵

https://zhuanlan.zhihu.com/p/34587739

```c
struct Matrix {
	int height,
	int width,
	int *elements,
}

__device__
void GetElem(Matrix a, int height, int width);

__device__
void SetElem(Matrix a, int h, int w, int v);

__global__
void MulMatrix(Matrix *a, Matrix *b, Matrix *c)
{
	float calue = 0.0;
	int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	for (int i=0; i < a->width; ++i) {
		value += GetElem(a, row, i) * GetElem(b, i, col);
	}
	SetElem(c, row, col, value);
}

// 矩阵转置
void Mul
```

## 例子: 加法效率衡量

https://developer.nvidia.com/blog/maximizing-unified-memory-performance-cuda/


https://developer.nvidia.com/blog/even-easier-introduction-cuda/

## 安装 CUDA (on wsl2)

注意, 不要下载包含 driver 的工具集版本, wsl 公用 windows 的显卡驱动, 重新下一个 linux 版驱动会把原本的覆盖掉.

安装 WSL-Ubuntu 版本的 CUDA 工具集 https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=WSL-Ubuntu&target_version=2.0&target_type=deb_network

## 参考

![|300](../../../attach/Pasted%20image%2020240513160710.png)

https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html

https://docs.nvidia.com/cuda/wsl-user-guide/index.html