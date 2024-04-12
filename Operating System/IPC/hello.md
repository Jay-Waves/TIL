
# [POSIX Shared Memory]{#SECTION002740000000000000000}

POSIX shared memory is actually a variation of mapped memory. The major
differences are to use `shm_open()` to open the shared memory object
(instead of calling `open()`) and use `shm_unlink()` to close and delete
the object (instead of calling `close()` which does not remove the
object). The options in `shm_open()` are substantially fewer than the
number of options provided in `open()`.

# [Mapped memory]{#SECTION002750000000000000000}

In a system with fixed memory (non-virtual), the address space of a
process occupies and is limited to a portion of the system\'s main
memory. For example, in Solaris 2.x virtual memory the actual address
space of a process occupies a file in the swap partition of disk storage
(the file is called the backing store). Pages of main memory buffer the
active (or recently active) portions of the process address space to
provide code for the CPU(s) to execute and data for the program to
process.

A page of address space is loaded when an address that is not currently
in memory is accessed by a CPU, causing a page fault. Since execution
cannot continue until the page fault is resolved by reading the
referenced address segment into memory, the process sleeps until the
page has been read. The most obvious difference between the two memory
systems for the application developer is that virtual memory lets
applications occupy much larger address spaces. Less obvious advantages
of virtual memory are much simpler and more efficient file I/O and very
efficient sharing of memory between processes.

## [Address Spaces and Mapping]{#SECTION002751000000000000000}

Since backing store files (the process address space) exist only in swap
storage, they are not included in the UNIX named file space. (This makes
backing store files inaccessible to other processes.) However, it is a
simple extension to allow the logical insertion of all, or part, of one,
or more, named files in the backing store and to treat the result as a
single address space. This is called mapping. With mapping, any part of
any readable or writable file can be logically included in a process\'s
address space. Like any other portion of the process\'s address space,
no page of the file is not actually loaded into memory until a page
fault forces this action. Pages of memory are written to the file only
if their contents have been modified. So, reading from and writing to
files is completely automatic and very efficient. More than one process
can map a single named file. This provides very efficient memory sharing
between processes. All or part of other files can also be shared between
processes.

Not all named file system objects can be mapped. Devices that cannot be
treated as storage, such as terminal and network device files, are
examples of objects that cannot be mapped. A process address space is
defined by all of the files (or portions of files) mapped into the
address space. Each mapping is sized and aligned to the page boundaries
of the system on which the process is executing. There is no memory
associated with processes themselves.

A process page maps to only one object at a time, although an object
address may be the subject of many process mappings. The notion of a
\"page\" is not a property of the mapped object. Mapping an object only
provides the potential for a process to read or write the object\'s
contents. Mapping makes the object\'s contents directly addressable by a
process. Applications can access the storage resources they use directly
rather than indirectly through read and write. Potential advantages
include efficiency (elimination of unnecessary data copying) and reduced
complexity (single-step updates rather than the read, modify buffer,
write cycle). The ability to access an object and have it retain its
identity over the course of the access is unique to this access method,
and facilitates the sharing of common code and data.

Because the file system name space includes any directory trees that are
connected from other systems via NFS, any networked file can also be
mapped into a process\'s address space.

## [Coherence]{#SECTION002752000000000000000}

Whether to share memory or to share data contained in the file, when
multiple process map a file simultaneously there may be problems with
simultaneous access to data elements. Such processes can cooperate
through any of the synchronization mechanisms provided in Solaris 2.x.
Because they are very light weight, the most efficient synchronization
mechanisms in Solaris 2.x are the threads library ones.

## [Creating and Using Mappings]{#SECTION002753000000000000000}

`mmap()` establishes a mapping of a named file system object (or part of
one) into a process address space. It is the basic memory management
function and it is very simple.

-   First `open()` the file, then
-   `mmap()` it with appropriate access and sharing options
-   Away you go.

`mmap` is prototypes as follows:

    #include <sys/types.h>
    #include <sys/mman.h>

    caddr_t mmap(caddr_t addr, size_t len, int prot, int flags,
              int fildes, off_t off);

The mapping established by `mmap()` replaces any previous mappings for
specified address range. The `flags` `MAP_SHARED` and `MAP_PRIVATE`
specify the mapping type, and one of them must be specified.
`MAP_SHARED` specifies that writes modify the mapped object. No further
operations on the object are needed to make the change. `MAP_PRIVATE`
specifies that an initial write to the mapped area creates a copy of the
page and all writes reference the copy. Only modified pages are copied.

A mapping type is retained across a `fork()`. The file descriptor used
in a mmap call need not be kept open after the mapping is established.
If it is closed, the mapping remains until the mapping is undone by
`munmap()` or be replacing in with a new mapping. If a mapped file is
shortened by a call to truncate, an access to the area of the file that
no longer exists causes a `SIGBUS` signal.

The following code fragment demonstrates a use of this to create a block
of scratch storage in a program, at an address that the system chooses.:

    int fd; 
    caddr_t result; 
    if ((fd = open("/dev/zero", O_RDWR)) == -1) 
       return ((caddr_t)-1); 

    result = mmap(0, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
    (void) close(fd);

## [Other Memory Control Functions]{#SECTION002754000000000000000}

`int mlock(caddr_t addr, size_t len)` causes the pages in the specified
address range to be locked in physical memory. References to locked
pages (in this or other processes) do not result in page faults that
require an I/O operation. This operation ties up physical resources and
can disrupt normal system operation, so, use of ` mlock()` is limited to
the superuser. The system lets only a configuration dependent limit of
pages be locked in memory. The call to mlock fails if this limit is
exceeded.

`int munlock(caddr_t addr, size_t len)` releases the locks on physical
pages. If multiple `mlock()` calls are made on an address range of a
single mapping, a single `munlock` call is release the locks. However,
if different mappings to the same pages are mlocked, the pages are not
unlocked until the locks on all the mappings are released. Locks are
also released when a mapping is removed, either through being replaced
with an mmap operation or removed with `munmap`. A lock is transferred
between pages on the *copy-on-write* event associated with a
`MAP_PRIVATE` mapping, thus locks on an address range that includes
` MAP_PRIVATE` mappings will be retained transparently along with the
copy-on-write redirection (see mmap above for a discussion of this
redirection)

`int mlockall(int flags)` and `int munlockall(void)` are similar to
`mlock()` and `munlock()`, but they operate on entire address spaces.
`mlockall()` sets locks on all pages in the address space and
`munlockall()` removes all locks on all pages in the address space,
whether established by mlock or mlockall.

`int msync(caddr_t addr, size_t len, int flags)` causes all modified
pages in the specified address range to be flushed to the objects mapped
by those addresses. It is similar to `fsync()` for files.

`long sysconf(int name)` returns the system dependent size of a memory
page. For portability, applications should not embed any constants
specifying the size of a page. Note that it is not unusual for page
sizes to vary even among implementations of the same instruction set.

`int mprotect(caddr_t addr, size_t len, int prot)` assigns the specified
protection to all pages in the specified address range. The protection
cannot exceed the permissions allowed on the underlying object.

`int brk(void *endds)` and `void *sbrk(int incr)` are called to add
storage to the data segment of a process. A process can manipulate this
area by calling `brk()` and `sbrk()`. `brk()` sets the system idea of
the lowest data segment location not used by the caller to addr (rounded
up to the next multiple of the system page size). `sbrk()` adds incr
bytes to the caller data space and returns a pointer to the start of the
new data area.


