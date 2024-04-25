### 精度

`precision` 是 `cout` 的成员函数.
```cpp
streamsize prec = cout.precision(3); // 将精度设置为3, 返回值是之前的精度
cout.precision(prec); // 恢复原精度
```

`setprecision` 是控制器, 类似 `endl`
```cpp
streamsize prec = cout.precision(3);
cout << "my number is:" << setprecision(3) << 0.22 <<setprecision(prec) << endl;
```

注意 `streamsize` 类型是由 `<ios>` 定义的; `setprecision()` 由 `<iomanip>` 定义 

### 检测输入

`while (cin>>x) {/*...*/}`

`cin` 是一个 `istream` 类型. 根据读取是否成功, 会返回一个 bool 值. 

以下情况 `cin` 语句可能返回 false:
- 文件结尾
- 读入的数据和存放的变量类型不匹配
- 输入设备硬件问题

调用 `cin.clear()` 可以清除其状态:
```cpp
if (in) { 
	// 清除原先的内容 
	hw.clear(); 
	
	// 读作业成绩 
	double x; 
	while (in>>x) 
		hw．push_back(x);

	//正常读取, 也可能产生异常, 如文件末尾和不匹配类型等
	in.clear(); 
} 
return in;
```
