### File Name

```c
url_table.h  // the class declaration
url_table.cc // cpp class definition
url_table_inl.h   // inline functions that include lots of code
url_table_test.cc // test
```

### Type Name

```cpp
// classes and structs
class UrlTable {...
class UrlTableTester {...
struct UrlTableProperties {...

// typedefs
typedef ... PropertiesMap;

// enums
enum UrlTableErrors {...
```

### Variable Name

```cpp
string table_name;
MyStruct {
	string table_name;
}

MyClass {
	string table_name_;
}
```

### Constane Name

Uppercase trailing a 'k'

```cpp
const int kDaysInAWeek = 7;
```

### Function Name

```cpp
DeleteUrl();
AddTableEntry();
```

accessors and mutator (get and set functions): match the name of variable they are getting and setting.

```cpp
class MyClass {
public:
	...
	int num_entries() const { return num_entries_; }
	void set_num_entries(int num_entries) { num_entries_ = num_entries;}

private:
	int num_entries_;	
}
```

### Macro Name

avoid using macros. use all capitals and underscores if needed absolutely, like `MY_MACROS`