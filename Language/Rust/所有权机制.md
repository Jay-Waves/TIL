Rust 通过**所有权机制**完成编译时对内存管理的检查. 内存管理的核心任务是管理堆上数据, 减少重复堆数据, 并及时清理避免内存泄露.

Rust Ownership Rules:
1. Each value in Rust has an owner.
2. There can only be one owner at a time.
3. When the owner goes out of scope, the value will be dropped.

Rust 中变量离开作用域后会失效, 类似 C++ RAII, 称为 `Drop`.
```rust 
{ // s is invalid
	let s = "hello"; // s is valid
	
} // s is dropped and invalid now
```

Rust 处理**堆上数据**时, 并不进行"引用拷贝", 而是丢弃. 该行为称为 `Move`.  ==从设计上, rust 希望不对数据进行值拷贝, 也因此所有自动拷贝行为的性能开销都是可控的.==
```rust
let s1 = String::from("hello");
let s2 = s1; // from now s1 is invalid

let s3 = s2.clone(); // both s2, s3 are valid

// return or call of function also transfer ownership
takes_owner_ship(s3); // from now s3 is invalid
```

Rust 处理**纯栈上数据**时 (不能嵌套任何堆数据), 会直接进行值拷贝. 称为 `Copy`
```rust
let x = 5;
let y = x; // both x,y are valid
```

## Reference

创建一个引用的过程叫 `borrowing`, 引用并没有所有权. 默认, 引用不可改变原对象.
```rust
let s1 = String::from("hello");
let len = cal_length(&s1);
```

**存在一个可变引用时, 关于该对象不能存在其他引用**, 在编译期即避免了数据竞争.

```rust
let mut s = String::from("hello");
// 错误代码:
let r1 = &s; 
let r2 = &mut s; 
println!("{} and {}", r1, r2); // error!

// 正确代码:
let r1 = &s;
println!("{}", r1); // the end of r1's lifetime
let r2 = &mut s;
println!("{}", r2); // no problem
```

Rust 编译器确保引用的值总是有效的, 即不会出现空指针错误. 当对数据的引用没有结束生命周期, 数据本身也应没有结束生命周期.
```rust
fn dangle() -> &String { // dangle returns a reference to a String

    let s = String::from("hello"); // s is a new String

    &s // we return a reference to the String, s
} // Here, s goes out of scope, and is dropped. Its memory goes away.
  // Danger!
```

## Slice 

切片类型是一种引用, 没有所有权.

