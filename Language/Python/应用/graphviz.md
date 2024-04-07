### 绘制函数调用图
```python
# -*- coding: utf-8 -*-

from pycallgraph import Config
from pycallgraph import PyCallGraph
from pycallgraph.output import GraphvizOutput


def main():
    # do something...



if __name__ == "__main__":
    config = Config()
    # 关系图中包括(include)哪些函数名。
    # 如果是某一类的函数，例如类gobang，则可以直接写'gobang.*'，表示以gobang.开头的所有函数。（利用正则表达式）。
    # config.trace_filter = GlobbingFilter(include=[
    #     'draw_chessboard',
    #     'draw_chessman',
    #     'draw_chessboard_with_chessman',
    #     'choose_save',
    #     'choose_turn',
    #     'choose_mode',
    #     'choose_button',
    #     'save_chess',
    #     'load_chess',
    #     'play_chess',
    #     'pop_window',
    #     'tip',
    #     'get_score',
    #     'max_score',
    #     'win',
    #     'key_control'
    # ])
    # 该段作用是关系图中不包括(exclude)哪些函数。(正则表达式规则)
    # config.trace_filter = GlobbingFilter(exclude=[
    #     'pycallgraph.*',
    #     '*.secret_function',
    #     'FileFinder.*',
    #     'ModuleLockManager.*',
    #     'SourceFilLoader.*'
    # ])
    graphviz = GraphvizOutput()
    graphviz.output_file = 'graph.png' #可以使用/或\\相对路径
    with PyCallGraph(output=graphviz, config=config):
        main()
```

### 绘制流程图
`$ python3 -m pyflowchart example.py` 
输出flow_chart.js格式内容
这种格式可以直接被typora等编辑器解析, 也可以去[flowchart绘制](http://flowchart.js.org/)网站绘图

还可以指定函数和类绘图:
```bash
$ python3 -m pyflowchart example.py -f function_name
# or
$ python3 -m pyflowchart example.py -f ClassName.method_name
```

flowchart格式语言值得一学, 但pyflowchart提供了基本函数使用:
```python
from pyflowchart import *

st = StartNode('a_pyflow_test')
op = OperationNode('do something')
cond = ConditionNode('Yes or No?')
io = InputOutputNode(InputOutputNode.OUTPUT, 'something...')
sub = SubroutineNode('A Subroutine')
e = EndNode('a_pyflow_test')

st.connect(op)
op.connect(cond)
cond.connect_yes(io)
cond.connect_no(sub)
sub.connect(op, "right")  # sub->op line starts from the right of sub
io.connect(e)
 
fc = Flowchart(st)
print(fc.flowchart())
```