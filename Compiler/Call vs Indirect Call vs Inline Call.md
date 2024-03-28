一般情况下，当C或者C++编译器遇到一个非内联函数的定义时，它会为该函数的定义生成机器码，并把这些机器码存储在一个目标文件中。同时，它还创建了一个与这些机器码相关联的名称。在C中，这个名称通常就是函数本身的名称；而在C++中，该名称还要加上参数类型的编码，从而即使在出现函数重载的情况下，也能够获得唯一的名称（最后这个名称通常称为mangled name，有时也称为decorated name）。譬如，当编译器看到一个如下的调用：

```
f()
```

它将会生成函数f的机器码。对于大多数机器语言来说，调用指令本身需要例行程序f的起始位置。这时就出现了两种情况：该起始位置可能成为指令的一部分（在这种情况下，这种指令也被称为直接调用），也可能位于内存或机器寄存器的某处（间接调用）。事实上，大多数现代的计算机体系结构都提供了这两种程序调用指令；但是直接调用的执行效率比间接调用要高出不少（这里不讨论）。实际上，随着计算机体系结构的不断复杂化，直接调用和间接调用之间的效率差距也不断增大。因此，编译器通常都会尽可能地生成直接调用指令。

通常而言，编译器刚开始并不知道函数究竟位于什么地址（例如，函数可以位于其他翻译单元）。然而，如果编译器知道了函数的名称，那么它首先会生成一个不含地址的调用指令——或者称为一个地址仍未确定的调用指令。另外，编译器在目标文件中还会生成一个实体，借助这个实体，链接器在后面能够更新上面创建的调用指令，使它的地址指向给定名称的函数，从而成为一个地址确定的调用指令。链接器之所以能够完成这些功能，是因为它能够见到创建自所有翻译单元的所有目标文件，也就是说：链接器在看到函数定义的位置的同时，也看到了函数调用的位置，因此能够确定直接调用的具体位置。

遗憾的是，当函数名称并不确定的时候，就只能使用间接调用了。使用函数指针进行调用的例子通常就都属于这种情况：

```
<span data-darkreader-inline-color="">void</span> foo (<span data-darkreader-inline-color="">void</span> (*<span data-darkreader-inline-color="">pf)() )
{
    pf(); </span><span data-darkreader-inline-color="">//</span><span data-darkreader-inline-color=""> 通过函数指针pf进行间接调用</span>
}
```

在这个例子中，链接器通常都不能够知道参数pf究竟指向哪一个函数（也就是说，对于foo()的不同调用，pf所指向的函数就可能不同）。因此，编译器并不能根据pf来匹配任何名字：而是要到代码实际执行的时候，才能够知道具体的调用目标是什么函数。

对于现代的计算机而言，尽管执行直接调用指令的速度和执行其他一般的指令相差无几（例如，执行对两个整数进行求和的指令），但是函数调用仍然是一个比较严重的性能障碍。考虑如下代码：

```
<span data-darkreader-inline-color="">int</span> f1(<span data-darkreader-inline-color="">int</span> <span data-darkreader-inline-color="">const</span>&amp;<span data-darkreader-inline-color=""> r)
{
    </span><span data-darkreader-inline-color="">return</span> ++(<span data-darkreader-inline-color="">int</span>&amp;)r;        <span data-darkreader-inline-color="">//</span><span data-darkreader-inline-color=""> 不合理，但却是合法的</span>
<span data-darkreader-inline-color="">}

</span><span data-darkreader-inline-color="">int</span> f2(<span data-darkreader-inline-color="">int</span> <span data-darkreader-inline-color="">const</span>&amp;<span data-darkreader-inline-color=""> r)
{
    </span><span data-darkreader-inline-color="">return</span><span data-darkreader-inline-color=""> r;
}

</span><span data-darkreader-inline-color="">int</span><span data-darkreader-inline-color=""> f3()
{
    </span><span data-darkreader-inline-color="">return</span> <span data-darkreader-inline-color="">42</span><span data-darkreader-inline-color="">;
}

</span><span data-darkreader-inline-color="">int</span><span data-darkreader-inline-color=""> foo()
{
    </span><span data-darkreader-inline-color="">int</span> param = <span data-darkreader-inline-color="">0</span><span data-darkreader-inline-color="">;
    </span><span data-darkreader-inline-color="">int</span> answer = <span data-darkreader-inline-color="">0</span><span data-darkreader-inline-color="">;
    f2(param);
    f3();
    </span><span data-darkreader-inline-color="">return</span> answer +<span data-darkreader-inline-color=""> param;
}</span>
```


函数f1接收一个const int的引用实参，这个const关键字意味着函数不会修改该引用实参所引用的对象。然而，如果这个引用的对象是一个可修改的值，那么C++程序可以合法地去除这个const属性（约束），也就是说能够改变这个对象的值（你可能会认为这是很不合理的，但这的的确确是标准C++所允许的），函数f1的行为正是如此。由于存在这种（修改const所引用的值）的可能性，所以对于那些要对函数所生成代码进行优化的编译器（实际上大多数编译器都是这样），就必须假设：每个接收（指向对象的）引用或者指针的函数都可能修改所指向对象的值。另外我们还应该清楚一点：通常情况下，编译器只是看到函数的声明，而函数的定义（或者称为实现）通常位于另一个翻译单元。

因此，大多数编译器都会假设上面代码的f2()也会修改param的值（即使实际操作并没有修改param的值）。实际上，编译器同样也不能假设f3()并不会修改局部变量param的值，因为函数f1()和f2()都可能会把param的地址存储到一个全局可访问的指针中，于是，从编译器的角度看来，f3()是完全有可能通过这个全局可访问指针修改param的值的。所以，这种不确定的效果令大多数编译器都不知道应该如何对待各种对象，从而也就不能够把这种对象的过程值（或者称为中间值）存储在快速寄存器中，而只能存储于内存中。因此，涉及到机器代码移动的优化，也就受到了很大的限制（通常而言，函数调用会对代码移动形成一个障碍）。

另一方面，存在一些高级的C++编译系统，它们可以跟踪潜在别名的许多实例（潜在别名是指：f1()的作用域中的表达式r，就是foo()作用域中param所命名对象的一个别名），这种特性的代价是：编译速度、资源使用量和代码可靠性。

然而，通过使用内联，就可以大大帮助普通编译器进行优化。假设前面的f1(), f2()和f3()都被声明为内联函数，那么foo()的代码就可以被转化为大体于下面等价的代码：

```
<span data-darkreader-inline-color="">int</span><span data-darkreader-inline-color=""> foo_ex()
{
    </span><span data-darkreader-inline-color="">int</span> param = <span data-darkreader-inline-color="">0</span><span data-darkreader-inline-color="">;
    </span><span data-darkreader-inline-color="">int</span> answer = <span data-darkreader-inline-color="">0</span><span data-darkreader-inline-color="">;
    answer </span>= ++(<span data-darkreader-inline-color="">int</span>&amp;<span data-darkreader-inline-color="">)param;
    </span><span data-darkreader-inline-color="">return</span> answer+<span data-darkreader-inline-color="">param;
}</span>
```

而一个普通的优化器可以马上把上面的代码变成：

```
<span data-darkreader-inline-color="">int</span><span data-darkreader-inline-color=""> foo_ex() 
{  
     </span><span data-darkreader-inline-color="">return</span> <span data-darkreader-inline-color="">2</span><span data-darkreader-inline-color="">; 
}</span>
```

这就充分阐明了这里使用内联的优点：在一个调用系列中，不但能够避免执行这些（查找名称的）机器代码；而且能够让优化器看到函数对传递进来的变量进行了哪些操作。

使用基于模板的回调来生成机器码的话，那么这些机器码将主要涉及到直接调用和内联调用；而如果用传统的回调的话，那么将会导致间接调用。使用模板的回调将会大大节省程序的运行时间。