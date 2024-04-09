React 基础语法:

### JSX

JSX (JavaScript XML) 是一种类似 XML 的 JavaScript 扩展. 直接在 JS 中嵌入 HTML.

```
jsxCopy codeconst element = <h1>Hello, world!</h1>;

```

### 组件

React 应用由多个组件构成, 可以是函数或类:

**函数组件**:
```js
    jsxCopy codefunction Welcome(props) {
      return <h1>Hello, {props.name}</h1>;
    }
```

**类组件**:
```js
jsxCopy codeclass Welcome extends React.Component {
	render() {
		return <h1>Hello, {this.props.name}</h1>;
	}
}

```

### Props

Props (属性) 是从父组件传递给子组件的参数. Props 是只读的.

```js
jsxCopy codefunction Welcome(props) {
  return <h1>Hello, {props.name}</h1>;
}

```

### State

State 是组件的私有数据, 可变. 常用于存储组件的局部数据, 而 Props 用于传递数据.

```js
jsxCopy codeclass Clock extends React.Component {
  constructor(props) {
    super(props);
    this.state = {date: new Date()};
  }
  render() {
    return <div>{this.state.date.toLocaleTimeString()}</div>;
  }
}

```

### 事件处理

React 有自己的事件系统, 你可以在 JSX 中直接添加事件处理程序.

```js
jsxCopy code<button onClick={this.handleClick}>
  Click me
</button>
```