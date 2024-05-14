---
url: https://ninja-build.org/
---

google 推出的轻量级构建系统, 应用于 chrome, android, llvm 等项目. 特点如下:
- 易于集成入 CMake 等上层构建系统, 像生成用于 Make 的 `MakeFile` 一样, 生成 `build.ninja` 构建文件作为 Ninja 的输入. ninja 官方不推荐手动写构建文件.
- 专注于构建速度 (注意不是编译器编译时间), 抛弃复杂决策.
- 语法简易, 像一个 assembler 而不是高层次语言, 功能比 Make 少.

对于我个人, 小型项目讨厌用 CMake, 又不喜欢 MakeFile 反人类的语法 (尤其是必须用 `\t` 作为缩进). Ninja 简洁易读的语法更吸引我.

## 语法

```ninja
cflags = -Wall -Werror
rule cc
	command = gcc $cflags -c $in -o $out
	description = '编译'

build foo.o: cc foo.c

# shadow vavirables like cflags **at this scope**
build new.o: cc new.c
	cflag = -Wall
```

### phony

`phony` 规则用于定义别名, 该语句不会被打印和记录.

```ninja
build foo: phony foo/with/long/path
```

也可用于创建永不重新构建的规则:

```ninja
rule touch
	command = touch $out
build file_always_exists: touch
build dummy_target: phony file_always_exists
```

### default

`default` 用于指定 Ninja 构建的输出文件. 默认会将所有没用作 `$in` 的文件都输出.

```ninja
build ...
...
default foo bar
```

## 参考

- [Ninja 官方仓库](https://github.com/ninja-build/ninja)
- [Ninja 官方网站](https://ninja-build.org/)
- [Performance of Open Source Software -- Ninja](https://aosabook.org/en/posa/ninja.html) 
- [Ninja 使用及介绍](https://juejin.cn/post/7121635640923389966)

