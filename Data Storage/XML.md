XML是可扩展标记语言 eXtensible Markup Language 的缩写

1. 纯文本, 默认 utf-8
2. 可嵌套

格式: 
- 第一行固定为 `<?xml version="1.0" encoding="UTF-8" ?>`
- 第二行是 DTD (document type definition), 类似 `<!DOCTYPE note SYSTEM "book.dtd">`
- 一个 XML 有且只有一个根元素.

|需转义字符|表示|
|---|---|
|< | `&lt;` |
| > | `&gt;`|
|&|`&amp;`|
|"|`&quot;`|
|'|`&apos;`|