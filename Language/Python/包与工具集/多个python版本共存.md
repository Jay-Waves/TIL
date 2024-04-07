python多版本共存的核心冲突是, 所有版本的python/pip/pyhonmw都是同一个名字, 使用环境变量去搜索bin程序, 会相互覆盖掉.

### 解决办法:

1. 按python版本名为不同版本python创建文件夹
2. 将python.exe和pythonmw.exe按版本号重命名, 比如python38.exe; 以后再cmd中不再使用python, 而使用python+版本号来唤起程序, 如`python38 main.py`
3. 重新安装pip: `phthon38 -m pip install --upgrade pip --force-reinstall`
4. 然后进入\scripts文件夹, 将pip3改成pip38即可

截止23/2/22, 没有发现坑