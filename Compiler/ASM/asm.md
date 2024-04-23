```
 <-    RAX: 64b                    ->         
                 <-  EAX:32b       ->
                          <-AX:16b ->
+----------------+--------+----+----+
|                |        | AH | AL |
+----------------+--------+----+----+
```

| Register | Functionality                                            |
| -------- | -------------------------------------------------------- |
| AX       | Accumulator, used for arithmetic, logic or return values |
| BX       | Base, hold indirect addresses                            |
| CX       | Count, used for bit shifting or string operation         |
| DX       | Data, division operation, I/O address                    |
| DI       | Destination Index, used for pointer                      |
| SI       | Source Index                                             |
| SP       | Stack Pointer, point to top of stack                     |
| BP       | Base Pointer, used for visiting stack                    |
| IP       | Instruction Pointer, same as `rip` in amd64                                     |
| FLAGS    |                                                          |

