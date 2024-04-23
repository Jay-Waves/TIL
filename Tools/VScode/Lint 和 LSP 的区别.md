### Programming Language Server (PLS)

基于 Language Server Protocol (LSP) 的服务, 用于 IDE 与代码服务器通信, 提供"补全, 跳转定义, 悬浮提示, 代码分析建议"等功能. 后台存在 PLS 的守护进程.

如 `gopls`, `pypls`

### Lint (Linting Tools)

静态代码分析工具, 检查代码中"错误, 质量, 风格问题", 通常在编写或保存代码时运行一次.

如 `golint`, `flake8`, `mypy`, `eslint`

区别是:
1. PLS 功能更强, 提供和 IDE 的复杂交互. 而 Linter 仅关注静态代码检查.
2. Linter 更轻量, 不必集成整套环境.