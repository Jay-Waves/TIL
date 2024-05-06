QT 核心库是: `QtWidgets`, `QtGui`, `QtCore`

```python
app = QApplication([])
```

QApplication 管理 Qt 的 event loop, 从 event queue 中选取 event 执行.

### `QMainWindow`

QMainWindow 提供了一个放组件的框架.

```python
# class QMainWindow
self.setWindowTitle(str)
self.setFixedSize(qsize(400, 300))
self.setCentralWidget(button)

# signals
self.windowTitleChanged.connect(...)

```

#### 1 QPushButton

```python
# class QPushButton("Press Me!")
is_checked = False

self.setCheckable(True)                # 让按钮有 check 功能
self.clicked.connect(toggled)          # clicked 动作发生时, 链接 toggled 信号槽
self.setChecked(is_checked)            # 设置 check 初值为 False

def toggled(checked):              # checked 就是按钮组件发出的信号
	is_checked = checked

self.isChecked()                       # 返回当前按钮状态
self.setText("New Promp")
self.setEnabled(False)                 # 按钮不可再点击
```

### Signals, Slots

signal 可以被同时发送给多个 slots (信号槽)

可以写一个函数接收信号:
```python
def toggled(checked):
	pritn(checked)

button.clicked.connect(toggled)
```

直接连接两个 widget:
```python
label=QLabel()
input_text=qLineEdit()
input_text.textChanged.connect(label.setText)
```

### Events

用户交互会产生 events. 在 Qt 中, 一个父组件可能有多级子组件, 子组件产生的 event 自下而上传递, 即如果下级组件无法处理该 event, 会将其传递给上层组件. 使用 `e.accept` 来主动将 event 标记为已处理; `e.ignore()` 标记为忽略, 此时 event 会继续向上传.

```python
class CustomButton(QPushButton)
	def mousePressEvent(self, e):
		e.accept()
```

#### 1 Mouse event

鼠标相关事件, 被对象 `Qt.QMouseEvent` 追踪, 并发布相关消息.
```python
setMouseTracking(True)

# 四个 MainWindow 中定义的鼠标方法, 可以在子类中覆盖它们来接收 事件句柄 e
mouseMoveEvent(self, e)
mousePressEvent(self, e)
mouseReleaseEvent(self, e)
mouseDoubleClickEvent(self, e)
```

事件句柄 e 有如下方法:
- `.button()` 引起该事件的那个按钮
- `.buttons()` 所有可点击的按钮的状态
- `.position` 鼠标相对 widget 的位置, 用 QPoint 表示.

用例: 判断 `e.button == Qt.MouseButton.LeftButton`. mouse button 相关标识符如下:

| Identifier                  | Value | Meanings                         |
| --------------------------- | ----- | -------------------------------- |
| Qt.MouseButton.NoButton     | 000   | 没有按钮被按下, 或者没有时间关联 |
| Qt.MouseButton.LeftButton   | 001   | 左侧按钮被鼠标按下               |
| Qt.MouseButton.RightButton  | 010   |                                  |
| Qt.MouseButton.MiddleButton | 100   |                                  |


#### 2 ContextMenu

`from QtWidgets import QMenu`

要定制 context menu, 应该覆盖父类 QMainWindow 的 `contextMenuEvent`.

```python
def contextMenuEvent(self, e):
	context = QMenu(self) 
	context.addAction(QAction("test 1", self))  # 传入 self, 在整个 qMianWindow 组件中使用该 context menu
	context.addAction(QAction("test 2", self)) 
	context.addAction(QAction("test 3", self)) 
	context.exec(e.globalPos())
```

