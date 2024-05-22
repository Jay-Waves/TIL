```c
__global__ void KernelFunc() {
	printf("...");
}

int main() {
	KernelFunc<<<1, 1>>>(); 
}
```

![|500](../../../attach/Pasted%20image%2020240513155528.png)

GPU 包含更多的运算单元, CPU 负责处理逻辑. 而 GPU 有更多运算核心, 上下文切换更加轻量, 常用于协同其计算. CPU 及其内存称为 host, GPU 及其内存称为 device. 典型 CUDA 程序流程如:
1. 分配 host 内存, `malloc(size_t)`, 初始化数据. 
2. 分配 device 内存 (即显存), `cudaError_t cudaMalloc(void** devPtr, size_t);`. 从 host 将数据拷贝到 device. `cudaMemcpy(void*dst, const void* src, size_t, cudaMemcpyKind kind=cudaMemcpyHostToDevice)`
3. 调用 CUDA Kernel 函数, 在 device 上完成运算. 核函数用 `__global__` 声明.
4. 将 device 结果拷贝到 host. `cudaMemcpy(...., kind=cudaMemcpyDeviceToHost)`
5. 释放 device 和 host 上分配的内存. `free()` + `cudaFree()`

![|500](../../../attach/Pasted%20image%2020240518124424.png)

CUDA 编程的核心是核函数 (`kernel_func<<<grid, block>>>`), 该函数会被放入 device 中并行化执行. 一个核在 device 中的映射称为**网格 (grid)**, 同一网格上的线程共享相同的全局内存空间; 每个网格又可分为多个**区块 (block)**, 每个区块中包含多个**线程 (thread)**. 

```c
dim3 grid(3, 2); // same as dim3 grid(3, 2, 1);
dim3 block(5, 3);
kernel_func<<<grid, block>>>(params...);
kernel_func<< <16, 1>> >(params...); // same as <<<dim3(16,1,1), 1>>>
```

定义上述的线程结构需要几个变量:

|           | 类型           | 描述                                   |
| --------- | -------------- | -------------------------------------- |
| blockIdx  | `dim3={x,y,z}` | 定义区块在网格中的坐标                 |
| blockDim  | `dim3`         | 定义区块的大小, 即容纳的线程的总体结构 |
| threadIdx | `dim3`         | 定义线程在区块中的坐标                                       |

对于二维行列结构, 其索引计算为:
```
row = blockIdx.y * blockDim.y + threadIdx.y;
col = blockIdx.x * blockDim.x + threadIdx.x;
```

## GPU硬件结构

一个区块中的线程, 放在同一个**流式多处理器 (SM, streaming multiprocessor)**, 现代 GPU 单区块支持的线程数一般为 1024. SM 包括 CUDA 核心, 共享内存和寄存器等结构. SM 采用 SIMT (single-instruciton, multiple-thread) 架构, 基本执行单元是线程束 (warps, 应和区块做区分, 区块大小应尽量为线程束线程数的倍数), 包含 32 个线程, 同时执行相同指令, 而每个线程有独立的PC, 寄存器和执行路径. 

由于线程束内的线程同时执行相同指令, 但可能遇到不同的程序分支(因为参数不同), 导致一些线程需要等待另一部分线程. 同时, 由于资源限制, 线程分配也有限. **所以 kernel 中定义的线程结构 `<<<grid, block>>>` 和物理层并不相同, 并发性能和逻辑也有差异**.

https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#simt-architecture

![|500](../../../attach/Pasted%20image%2020240518124354.png)

查询本机实际 GPU 硬件配置:

```c
    cudaDeviceProp devProp;
    CHECK(cudaGetDeviceProperties(&devProp, dev));
    devProp.name; // GPU device
    devProp.multiProcessorCount; // number of SMs
    devProp.sharedMemPerBlock;
    devProp.maxThreadsPerBlock;
    devProp.maxThreadsPerMultiProcessor; // 
```

### 函数类型限定词

- `__global__ void()` 在 host 调用, 在 device 执行. 是异步的.
- `__device__` 在 device 上调用执行, 不能和 `__global__` 同时用.
- `__host__` 在 host 上调用执行. 缺省值, 也不能和 `__global__` 同时用, 可以和 `__device__` 同时用.

### CUDA 内存组织


### 内存托管

CUDA 6.0+ 以上, 提供了内存托管机制[^1], 简化了 GPU 编程的内存管理. 它允许 CPU 和 GPU 共享相同的内存地址空间, 开发者不必显式地管理主机和设备间的内存拷贝, 由 CUDA 运行时负责数据在主机和设备间的迁移. 

[^1]: https://developer.nvidia.com/blog/unified-memory-in-cuda-6/

```c
cudaMallocManaged(&a, size);
...
// no need for cudaMemcpy
cudaFree(a);
```

## GPU加速

CUDA 程序能够获得高性能的必要不充分条件:
- 数据传输比例少, 即减少主机核设备间频繁大量的数据传输.
- 核函数的算术强度高, 逻辑强度较低. SM 希望多线程执行相同程序指令, 仅有数据不同, 当遇到分支时, 未进入分支的线程将阻塞等待.
- 和函数中定义的线程数目较多, 即增大核函数的并行规模.

### 例子: 矩阵[^2]

[^2]: https://zhuanlan.zhihu.com/p/34587739, 前文部分介绍也来自这篇文章.

```c
#include <iostream>
#include <cstdlib>
#include <cuda_runtime.h>

struct Matrix {
	int height,
	int width,
	float *elements,

	static Matrix* create(int h, int w) {
		Matrix *m;
		cudaMallocManaged(&m, sizeof(Matrix);
		cudaMallocManaged(&m->elements, h * w * sizeof(float));
		m->height = h;
		m->wight = w;
		return m;
	}
	
	static void destroy(Matrix* m) {
		cudaFree(m->elements);
		cudaFree(m);
	}
}

__device__
void GetElem(Matrix *m, int row, int col) {
	return m->elements[row * m->width + col];
}

__device__
void SetElem(Matrix *m, int row, int col, float value) {
	m->elements[row * m->width + col] = value;
}

__global__
void MulMatrix(Matrix *a, Matrix *b, Matrix *c) {
	float calue = 0.0;
	int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	for (int i=0; i < a->width; ++i) {
		value += GetElem(a, row, i) * GetElem(b, i, col);
	}
	SetElem(c, row, col, value);
}
													
__global__
void TransposeMatrix(Matrix *in, Matrix *out) {
	int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	if (row < in->height && col < in->width ) {
		int val = GetElem(*in, row, col);
		SetElem(*out, col, row, val);
	}
}

int main() {
	int width = 1 << 10;
	int height = 1 << 10;
	Matrix *A, *B, *C;
	A = Matrix::create(height, width);
	B = Matrix::create(height, width);
	C = Matrix::create(height, width);
	for (int i=0; i < width * height; ++i) {
		A->elements[i] = 1.0;
		B->elements[i] = 2.0;
	}
	
	dim3 block_s(32, 32);
	dim3 grid_s(width + block_s.x - 1) / block_s.x, 
		(height + block_s.y - 1) / block_s.y);
	MulMatrix<<<grid_s, block_s>>>(A, B, C);
	
	cudaDeviceSynchronize();
	float maxError = 0.0;
	for (int i = 0; i < width * height; ++i)
		maxError = fmax(maxError, fabs(C->elements[i] - 2*width));
	std::cout << "max error" << maxError << std::endl;

	Matrix::destroy(A);
	Matrix::destroy(B);
	Matrix::destroy(C);
	return 0;
}
```

### 例子: 加法效率衡量

https://developer.nvidia.com/blog/maximizing-unified-memory-performance-cuda/


https://developer.nvidia.com/blog/even-easier-introduction-cuda/

## 安装 CUDA (on wsl2)

安装 WSL-Ubuntu 版本的 CUDA Toolkits[^3][^4], *注意, 不要下载包含 driver 的工具集版本, wsl 公用 windows 的显卡驱动, 重新下一个 linux 版驱动会把原本的覆盖掉.* 安装后需要手动设置环境[^5] :

确保 cuda-toolkit 已经正常安装, 添加环境变量:

```bash
# in .bashrc
export PATH=/usr/local/cuda/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

[^3]: https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=WSL-Ubuntu&target_version=2.0&target_type=deb_network

[^4]: https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#wsl

[^5]: https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#post-installation-actions 注意, `/usr/bin/nvidia-persistenced` 在 wsl 上无必要, 显卡驱动管理完全由 windows 宿主机负责.

检查 cuda 环境:

```bash
# 检查 devices 
> nvidia-smi

# 检查编译器 nvcc
> nvcc --version
```

Nvidia 提供了一些 CUDA 编程的例子, 可用来验证安装. 尝试编译 [Nvidia CUDA Examples](https://github.com/nvidia/cuda-samples) 后运行:

```bash
> ./1_Utilities/deviceQuery # 和 nvidia-smi 效果差不多
Detected 1 CUDA Capable device(s) 
Device 0: "NVIDIA GeForce RTX 3070 Laptop GPU" 
CUDA Driver Version / Runtime Version 12.3 / 12.4 
CUDA Capability Major/Minor version number: 8.6
```

这里需要区分一下概念: 

|              | capability version                                            | runtime (CUDArt) version                           | driver version                            |
| ------------ | --------------------------------------------------------- | -------------------------------------------------- | --------------------------------------------- |
| 名称         | 设备能力版本                                              | runtime API 版本                                   | 驱动程序版本                                  |
| (我的)版本号 | 8.6                                                       | 12.4                                               | 12.3                                          |
| 含义         | GPU 硬件版本, 每代 Nvidia GPU 都有特定设备能力版本. | 软件接口层, 每个 toolkit 版本都会更新 runtime api. | 驱动程序版本, 负责和操作系统交互操作 GPU 设备 |

GPU Driver 可能与 Toolkit 版本并不匹配[^6], 可能导致编译出的 CUDA 程序无法正常运行:

| CUDA Toolkit | linux x86_64 driver | windows driver |
| ------------ | ------------------- | -------------- |
| CUDA 12.x    | `>=525.60.13`       | `>=527.41`     |
| CUDA 11.x    | `>=450.80.02`       | `>=452.39`               |

[^6]: https://docs.nvidia.com/deploy/cuda-compatibility/index.html


## CUDA 编译与调试

CUDA 的源码文件称为 `.cu`, 用于和正常 C/C++ 文件区分. CUDA 的编译器封装则称为 `nvcc`, 其底层仍依赖于主机编译器 (`gcc, clang`), 用法也类似.

```bash
# 这里使用 -arch 指定设备能力版本.
nvcc -arch=sm_86 demo.cu -o demo
```

***

### 更多工具

`nvidia-smi` 检查当前显卡设备.

CUDA 提供了专用的性能测试工具 `nvprof`, 适用于 CUDA 8.0 以下版本. 更新版本中, 应使用 Nsight 功能组件:

```bash
# nsight systems, 运行分析 CUDA 应用.
nsys profile --stats=true ./a.out

# nsight computer, CUDA 内核分析
nv-nsight-cu-cli ....
```

## CUDA 相关库

CUDA 标准库:

|          | 简介                             |
| -------- | -------------------------------- |
| Thrust   | 类 C++ STL                       |
| cuBLAS   | basic linear algebra subroutines |
| cuFFT    | fast Fourier transforms          |
| cuSPARSE | sparse matrix                    |
| cuRAND   | random number generator          |
| cuSolver | dense matrix                     |
| cuDNN         | deep neural network                         |


## 参考

![|300](../../../attach/Pasted%20image%2020240513160710.png)

https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html

https://docs.nvidia.com/cuda/wsl-user-guide/index.html