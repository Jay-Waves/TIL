> 摘录：[【Python】python文件打开方式详解——a、a+、r+、w+、rb、rt区别_兔子爱读书的博客-CSDN博客_python ru](https://blog.csdn.net/ztf312/article/details/47259805)

**第一步 排除文件打开方式错误：**

r**只读**，r+**读写，**不创建

w**新建只写**，w+**新建读写**，二者都会将文件内容清零

（以w方式打开，不能读出。w+可读写）

**w+与r+区别：**

r+：可读可写，若文件不存在，报错；w+: 可读可写，若文件不存在，创建

**r+与a+区别：**

```
fd = open("1.txt",'w+')fd.write('123')fd = open("1.txt",'r+')fd.write('456')fd = open("1.txt",'a+')fd.write('789')
```

结果：456789

说明r+进行了覆盖写。

**以a,a+的方式打开文件，附加方式打开**

（**a**：**附加写**方式打开，不可读；**a+**: **附加读写**方式打开）

以 'U' 标志打开文件, 所有的行分割符通过 Python 的输入方法(例#如 read\*() )，返回时都会被替换为换行符\\n. ('rU' 模式也支持 'rb' 选项) . 

r和U要求文件必须存在

**不可读的打开方式**：**w和a**

**若不存在会创建新文件的打开方式：a，a+，w，w+**

```
>>> fd=open(r'f:\mypython\test.py','w')>>> fd.read()Traceback (most recent call last):  File "<stdin>", line 1, in <module>IOError: File not open for reading>>> fd=open(r'f:\mypython\test.py','a')>>> fd.read()Traceback (most recent call last):  File "<stdin>", line 1, in <module>IOError: File not open for reading>>></span></span></span>
```

**r、rb、rt区别**

使用’r’一般情况下最常用的，但是在进行读取二进制文件时，可能会出现文档读取不全的现象；

使用’rb’按照二进制位进行读取的，不会将读取的字节转换成字符，二进制文件用二进制读取用’rb’ ；

rt模式下，python在读取文本时会自动把\\r\\n转换成\\n，文本文件用二进制读取用‘rt’；

**2.正确读写方式打开，出现乱码**

```
>>> fd=open(r'f:\mypython\test.py','a+')>>> fd.write('123')>>> fd.read()>>> fd.close()
```

close之前，手动打开文件，什么都没写入；close后，手动打开文件，乱码：123嚅?     

原因分析：指针问题。open()以a+模式开启了一个附加读写模式的文件，由于是a，所以指针在文件末尾。此时如果做read()，则Python发现指针位置就是EOF，读取到空字符串。

在写入123之后，指针的位置是4，仍然是文件尾，文件在内存中是123\[EOF\]。

但看起来read()的时候，Python仍然去试图在磁盘的文件上，将指针从文件头向后跳3，再去读取到EOF为止。

也就是说，你实际上是跳过了该文件真正的EOF，为硬盘底层的数据做了一个dump，一直dump到了一个从前存盘文件的\[EOF\]为止。所以最后得到了一些根本不期待的随机乱字符，而不是编码问题造成的乱码。

解决方案：读取**之前**将指针重置为文件头（如果读取之后重置再读，无效）

```
>>> fd=open(r'f:\mypython\test.py','a+')>>> fd.seek(0)>>> fd.read()'123'<span style="white-space:pre"></span>
```

**3.文件里有内容，却读出空字符**

```
>>> fd=open(r'f:\mypython\test.py','w+') >>> fd.write('456')>>> fd.flush()<span style="white-space:pre"></span>>>> fd.read()'' 
```

原因：同样是指针问题，写入后指针指向末尾\[EOF\]，因此读出空

解决方案一、调用close后重新打开，指针位于开头。(r,r+,a+,U都可以，注意不要用w，w+，a打开)

```
>>> fd.close()>>> fd=open(r'f:\mypython\test.py','a+')>>> fd.read()'456'>>> fd.close()>>> fd=open(r'f:\mypython\test.py','r+')>>> fd.read()'456'<pre name="code" class="python">>>> fd.close()>>> fd=open(r'f:\mypython\test.py','r')>>> fd.read()'456'>>> fd.close()>>> fd=open(r'f:\mypython\test.py','U')>>> fd.read()'456'
```

解决方案二、调用seek指向开头

```
>>> fd=open(r'f:\mypython\test.py','w+')>>> fd.write('456')>>> fd.seek(0)>>> fd.read()'456'
```

seek函数

seek(offset\[, whence\]) ，offset是相对于某个位置的偏移量。位置由whence决定，默认whence=0，从开头起；whence=1，从当前位置算起；whence=2相对于文件末尾移动，通常offset取负值。

**4\. 记得close()关闭**

当我们写文件时，操作系统往往不会立刻把数据写入磁盘，而是放到内存缓存起来，空闲的时候再慢慢写入。只有调用close()方法时，操作系统才保证把没有写入的数据全部写入磁盘。忘记调用close()的后果是数据可能只写了一部分到磁盘，剩下的丢失了。所以，还是用with语句来得保险：

with open('/Users/michael/test.txt', 'w') as f:  
    f.write('Hello, world!')

\----------

看评论：对于'r+'来说，如果先读取了内容，再写入的话就变成了追加的模式，如果直接写入内容，就是覆盖了