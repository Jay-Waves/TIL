kernel module package commands: `modprobe, insmod, depmod`

```shell
# on Debian
sudo apt install build-essential kmod

# on Arch
sudo pacman -S gcc kmod
```

dicover what modules are currently loaded within my kernel:

```shell
sudo lsmod
```

Modules are stored within file `/proc/modules`

## Preparatory Work

- turn off modversioning (`CONFIG_MODVERSIONS`)
- use X Window System
- disable UEFI SecureBoot, to enable to boot with third-party software.

install header files for kernel:

```shell
# on Debian
sudo apt update
sudo apt-cache search linux-header-`uname -r`
sudo apt install kmod linux-headers-5.4.0-80-generic # just an example

# on Arch
sudo pacman -S linux-headers
```

Header files are in `/lib/modules/<kernel_version>/build/include`. On WSL2, header files are in `<wsl-kernel-location>/include`, see [here](https://unix.stackexchange.com/questions/594470/wsl-2-does-not-have-lib-modules#:~:text=For%20those%20that%20need%20to%20load%20modules%20on,5%20Restart%20WSL.%20Your%20module%20should%20be%20loaded.). **For WSL2, default kernel image doesn't allow `insmod`, but you can [compiler your own WSL kernel](../Distributions/WSL/更新%20WSL%20Linux%20内核.md) for that.** It's suggested to compile our module in an environment which was identical to the one in which our precompiled kernel was built, for every linux distribution.

## Hello World

It is unclear what the outcomes are for those who deviate from this tradition, but is seemes prudent to adhere to it.

### Makefile

for wsl2:

```makefile
obj-m := hello.o
KERNEL := /path/to/WSL2-Linux-Kernel
PWD := /path/to/your/modules

all:
    make -C $(KERNEL) M=$(PWD) modules

clean:
    make -C $(KERNEL)$ M=$(PWD)$ clean

test:
    # tell make to ignore error in case the module isn’t loaded.
    -sudo rmmod hello.ko 
    # Clear the kernel log without echo
    sudo dmesg -C
    # Insert the module
    sudo insmod hello.ko
    # Display the kernel log
    dmesg
unload:
    sudo rm /dev/lkm_example
    sudo rmmod lkm_example
```

then `make` to compile the module. file `hello-1.ko` will be found , use  `modinfo hello-1.ko` to look up its information. 

- load the module: `sudo insmod hello-1.ko`
- list loaded module: `sudo lsmod` 
- then remove it: `sudo rmmod hello_1`, this name is listed in `lsmod`

Additional details about Kernel Build Mechanism, see:
> Documentation/kbuild/modules.rst
> Documentation/kbuild/makefiles.rst

### First Module

```c
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

int init_module(void)
{
    pr_info("Hello world 1.\n");

    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

void cleanup_module(void)
{
    pr_info("Goodbye world 1.\n");
}

MODULE_LICENSE("MIT");
MODULE_AUTHOR("YJW");
MODULE_DESCRIPTION("a sample driver?");
```

Kernel modules must have `init_module()` called when the module is `insmod`ed, `cleanup_module()` called just before it's removed, and `<linux/module.h>` header files included.

In later kenel versions, `init_module()` and `cleanup_module()` can be named anything by using macros: `module_init`, `module_exit`. Only requirement is that init and claneup functions must be defined before calling those macros.

```c
#include <linux/init.h> /* Needed for the macros */
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

static int __init hello_2_init(void)
{
    pr_info("Hello, world 2\n");
    return 0;
}

static void __exit hello_2_exit(void)
{
    pr_info("Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
```

### Passing Command Line Arguments

use `$ insmod mymodule.ko myvar=5`

```c
#include <linux/moduleparam.h>

int myint = 3;
module_param(myint, int, 0);
MODULE_PARM_DESC(myint, "An interger"); // description for this paramter

// array arguments
int myintarray[2];
// not interested in count
module_param_array(myintarray, int, NULL, 0); 

short myshortarray[4];
int count;
// put count int "count"
module_param_arrya(myshortarray, short, &count, 0); 

// module_param_string();
```

### Multipe Files

makefile:

```makefile
obj-m += startstop.o
startstop-objs := start.o stop.o
```