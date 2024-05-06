GNU MP 支持任意精度的C语言大整数运算库. 详见手册 https://gmplib.org/manual/

编译:
```bash
gcc test.c -lgmp
```

```c
#include<gmp.h>

{
	mpz_t a;
	mpz_init(a);

	gmp_scanf("%Zd", a);
	gmp_printf("a= %Zd\n", a);
}
```

支持数据类型:

| 基础类型 | 含义   |
| -------- | ------ |
| mpz_t    | 整数类   |
| mpq_t    | 有理数类 |
| mpf_t    |浮点数类       |

其他函数原型:

```c
typedef size_t mp_limb_t          // 高精度数中的一个机器字长
typedef unsigned long mp_bitcnt_t // 高精度数的位数

typedef struct {
	int _mp_alloc;    // 开辟的内存 limbs 数
	int _mp_size;     // |mp_size| 标识实际有效 limbs 数, 同时会记录符号.
	mp_limb_t *_mp_d; // 指向高精度数内存
} __mpz_struct
typedef __mpz_struct mpz_t[1];

mpz_init(mp); // set as 0
mpz_clear()
mpz_init_set_str(mp, str, scale) 

mpz_mul(mp1, mp2, tmp)

// conversions
mpz_get_ui()  // mpz_t -> signed long
mpz_get_si()  // mpz_t -> unsigned long
mpz_get_d()   // mpz_t -> double
mpz_get_str() // mpz_t -> char *
```