当无当前文件权限时, 通过保存当前修改到新文件, 来强制保存退出

1. `:w !sudo tee %`, 注意%指代当前文件
2. `q!`强制退出

***

定义 alias: `:ab email xxx@gmail.com`, 此后 vim 会自动替换.

***

在 Visual 模式下, 可以快速匹配域, 如: `"...", '...', (...), {...}, [...]`.
1. `va(` 选中域, 包括 `()`
2. `vi]` 选中域, 不包括 `[]`