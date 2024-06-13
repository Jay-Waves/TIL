SQLite 是极轻量的本地数据库. SQLite 没有服务进程需要配置和连接, 只需要运行 `sqlite.exe` 直接和本地数据库文件 `.db` 交互, 方便便携. 但由于没有同一服务进程管理数据库, SQLite 对并发性支持没那么好. SQLite 将每个 SQL 语句编译为可执行机器码, 而不是解析为一个程序对象, 所以 SQLite 更像是一个轻量虚拟机. 

|                | SQLite                                  | MySQL                                  |
| -------------- | --------------------------------------- | -------------------------------------- |
| 是否有服务进程 | 否                                      | 是, 配置麻烦                           |
| 存储文件       | 单 `.db` 文件. 任意存储位置, 直接跨平台 | 多文件存储, 固定配置位置, 跨平台须导出 |
| 软件体积       | <20MB                                   | >1GB                                   |
| 并发性         | 数据库文件级别, 读写锁                  | 表行级别                               |
| 类型支持       | 可变数据长度, 以及更宽松的类型          | 静态类型, 通常须设置长度, 有长度上限                                       |

### SQLite3 内置命令

sqlite3 命令行环境接收三种输入:
- SQL 语句, `;` 结尾. 
- 点命令,  `.` 开头.
- 注释, `#` 开头.

```bash
sqlite3

sql> .help
sql> .quit

sql> .dump
sql> .sha3sum
sql> .read file.sql
sql> .quit
```

#### 格式化输出

`.mode MODE` 设置输出格式, `MODE` 可以是下列之一:
- `csv`
- `column`
- `html`
- `insert` 即 sql insert 语句格式.
- `line` 按行输出每个值
- `list` 由设置的 separator 分隔符输出
- `tabs` 由 Tab 分隔输出

`.width 12` 为 `column` 模式设置列宽度.

#### 连接和管理数据库

SQLite 默认打开 main 主数据库, 以及 temp 临时 (内存中) 数据库. 

```bash
sql> .tables
sql> .schema
sql> .databases

sql> .open mydb.db
sql> .save mydb.db
```

### SQLite 数据类型

| 类型      | 解释                |
| --------- | ------------------- |
| `NULL`    | 空值                |
| `INTEGER` | 有符号整型          |
| `REAL`    | 浮点数, 8字节双精度 |
| `TEXT`    | 字符串              |
| `BLOB`    | 二进制对象, 原始方式存储                    |

> SQLite blessing:  
> May you do good and not evil  
> May you find forgiveness for yourself and forgive others  
> May you shared freely, never taking more than you give.