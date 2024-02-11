SQL (Structured Query Language) 是[声明式语言](../../../Language/编程范式.md), 其功能:
- 数据定义语言 (Data Definition Lanuguage): 数据库结构定义
- 数据操纵语言 (Data Manipulation Language): 数据查询与修改
- 数据控制语言 (Data Control Language): 权限控制
- 事务控制语言 (Transaction Control Language): 

| SQL 功能 | 谓词                           |
| -------- | ------------------------------ |
| DDL      | creat, drop, alter             |
| DML      | select, insert, update, delete |
| DCL      | grant, revoke                               |

## 数据定义

| 对象                    | 创建操作      | 删除操作    | 修改操作    |
| ----------------------- | ------------- | ----------- | ----------- |
| 模式 <br> (无标准, 类似表命名空间) | create schema | drop schema |             |
| 表                      | create table  | drop table  | alter table |
| 视图                    | create view   | drop view   |             |
| 索引                    | create index  | drop index  | alter index            |

建表:

```sql
create table Student(
	Sno char(9) primary key,
	Sname char(20) unique,
	Ssec char(2),
	Cno char(4),
	foreign key (Cno) reference Class(Cno)
);
```

| 数据类型                     | 含  义                                                         |
| ---------------------------- | -------------------------------------------------------------- |
| CHAR(n) <br> CHARACTER(n)    | 长度为n的定长字符串 `'my char'`                                |
| VARCHAR(n)                   | 最大长度为n的变长字符串                                        |
| CLOB                         | 字符串大对象 (character large object)                          |
| BLOB                         | 二进制大对象(binary large object)                              |
| INT，INTEGER                 | 整数 (4字节)                                                   |
| SMALLINT                     | 短整数 (2字节)                                                 |
| BIGINT                       | 长整数 (8字节)                                                 |
| NUMERIC(p，d)                | 定点数, 由p位数字 (不包括符号, 小数点) 组成, 小数后面有d位数字 |
| DECIMAL(p, d) <br> DEC(p, d) | 同 NUMERIC                                                     |
| REAL                         | 取决于机器精度的单精度浮点数                                   |
| DOUBLE PRECISION             | 取决于机器精度的双精度浮点数                                   |
| FLOAT(n)                     | 可选精度的浮点数, 精度至少为n位数字                            |
| BOOLEAN                      | 逻辑布尔量                                                     |
| DATE                         | 日期, 包含年月日, 格式为 `YYYY-MM-DD`                          |
| TIME                         | 时间, 包含时分秒，格式为 `HH:MM:SS`                            |
| TIMESTAMP                    | 时间戳类型 `'1970-01-01 00:00:01' UTC`                         |
| INTERVAL                     | 时间间隔类型                                                   |

删除表: `drop table <t> [restrict | cascade]`
- restrict: 和 cascade 相反
- cascade: 若存在其他依赖该表对象, 相关依赖对象一起删除

修改表:
```sql
alter table <表名>
[add[column] <新列名> <数据类型> [约束]]
[add <表级约束>]
[drop[column] <列名> [cascade|restrict]]
[drop constraint <完整性约束名> [restrict|cascade]]
[alter column <列名> <数据类型>];
```

## 数据查询

```sql
select [all|distinct] <列表达式>[, <其他列表达式>]
from <表名> [, <其他表名>] [as] <别名>
[where <条件>]
[group by <列名> [having <条件>]]
[order by <列名> [asc|desc]];
```

常见查询条件:

| 条件     | 谓词              |
| -------- | ----------------- |
| 比较     | =, >, <, <>       |
| 字符匹配 | (not) like        |
| 空值     | is (not) null     |
| 逻辑     | and, or, not      |
| 确定范围 | (not) between and |
| 确定集合 | (not) in          |
| 存在     | (not) exists             | 

聚合函数: 注意 `where` 不能使用聚合函数, 需要使用 `having`. 除 `count()` 外所有聚合函数会忽略**含 null 行**, 分组时多个 null 会被分为一组.
- `count([distinct] <列名>)`
- `sum()`
- `avg()`
- `max()`
- `min()`

### 子语句

**嵌套查询:**

![|500](../../../attach/Pasted%20image%2020240105121556.png)

嵌套集合关系:

|     | =   | <>     | <    | <=    | >    | >=    |
| --- | --- | ------ | ---- | ----- | ---- | ----- |
| ANY | IN  | --     | <MAX | <=MAX | >MIN | >=MIN |
| ALL | --  | NOT IN | <MIN | <=MIN | >MAX | >=MAX      |

数据库没有全称量词 (for all), 需要用存在量词等价转换: $(\forall x) P\equiv \neg (\exists x(\neg P))$, 其中量词 $x\in Q$, P 为谓词. 举例而言, `选择了所有课程的学生`<->`不存在有课程没有被该学生选过`, 其中 $x=\text{课程}\in Q=\text{所有课程集合}$, $P=\text{学生选择了该门课}$.

**集合查询**:
- minus
- `union [all]` all 保留重复元组.
- intersect

### 查询优化

数据库 SQL 处理过程: 
![|350](../../../attach/Pasted%20image%2020240104222337.png)

查询优化分类
- 代数优化: 关系代数表达式优化
- 物理优化: 存取路径和底层操作算法优化

```
SQL 语句
  |
  | 编译
  v
关系代数
  |
  | 代数优化
  v
关系代数
  |
  | 物理优化
  v
执行程序
```

#### 执行开销

- 磁盘存取块数 (IO代价), **主要代价**.
- 处理机时间 (CPU代价).
- 查询的内存开销.
- (通信开销)

#### 启发式代数优化

1. **选择**运算尽可能先做
2. 投影和选择同时进行 (运算顺序接近), 对行列同时运算
3. 将投影同其前后的双目运算符结合. 
4. 选择操作和前面的笛卡尔积结合. 如 $R\substack{\bowtie\\A\theta B}S=\sigma_{t[A]\theta s[B]}(R\times S)$
5. 找出并合并公共子表达式

## 数据修改

```sql
update <table>
set <col> = <expr> [, ...]
[where <condition>];

insert
into <table> [<col1>, ...]
values (<cosnt1 [, ...]);

delete
from <table>
[where <condition>];
```