## Pip

### 网络问题

使用 `pip install` 时报错:

```
WARNING: There was an error checking the latest version of pip.
```

问题可能是:
1. pip 忘记更新, 每次安装前先执行: `python -m pip install --upgrade pip`
2. 没关 VPN, 连着 VPN 不会连接成功?? (换机场后又好了)

## 手动安装

当 pip 没有适合的包版本时, 可使用 wheel.exe 来下载 `.whl` 安装包.
1. 下载对应 `.whl` 文件
2. `pip install /pth/to/install`

### 参数

第三方安装包命名格式: `版本号-python版本号-适合的平台`
- python 版本号应和本机对应
- 平台和本机对应

查询版本和平台的方法:
```python
>>> import platform
>>> platform.architecture()
'''("64Bit","WindowsPE")'''
```