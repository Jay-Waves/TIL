`std::initializer_list` 允许接收初始化列表作为函数参数, 只读. C++11


```cpp
#include<initializer_list>
using namespace std;

void print(initializer_list<int> vals) {
	for (auto p = vals.begin(); p != vals.end(), ++p){
		out << *p;
	}
	out << endl;
}

pring( {1, 2, 3, 4, 5} );


class MyArray{
public:
	std::vector<int> data_;
	MyArray(initializer_list<int> list): data(list){
		...
	}
}

MyArray arr = {1, 2, 3, 4, 5};
```

