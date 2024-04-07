rust 的包管理和编译工具链为 `cargo`

```sh
cargo --version
```

## 创建项目

创建一个新项目:

```sh
$ cargo new helloworld --vcs=git
$ ls helloworld
+ src/main.rs
+ Cargo.toml
+ .git
+ .gitignore
```

`Cargo.toml` 配置文件内容详见[文档](https://doc.rust-lang.org/cargo/reference/manifest.html):

```toml
[package]
name = "helloworld"
version = "0.1.0"
edition = "2021"

[dependencies]
```

`src/main.rs` 为主文件:

```rust
fn main() {
    println!("Hello, world!");
}
```

编译执行, 默认编译类型为 debug, `cargo` 会调用 `rustc` 编译器来编译程序. 编译后, `Cargo.lock` 文件用于存储当前依赖版本.

```sh
$ cargo build
$ ./target/debug/helloworld
Hello, World!
$ cargo run # also valid
Hello, World!
```

使用 `cargo run` 时, 编译器还会检查 `src` 源码是否有改动, 如果有改动会增量编译. 也可以用 `cargo check` 手动实现对源码的错误检查和增量编译, 但该命令不会链接可执行程序.