/* -*- Mode: C; tab-width: 8; c-basic-offset: 8; indent-tabs-mode: t; -*- */

/* XXX unify me with recorder definitions, to make this the SSOT for
 * syscall info */

#if !defined(SYSCALL_DEF)
# error Define SYSCALL_DEF to make this file do something useful
#endif

/**
 * These are the syscalls that rr knows how to replay.  The includer
 * should define SYSCALL_DEF to a macro that will do something each
 * syscall definition.  The format of SYSCALL_DEF is
 *
 *   SYSCALL_DEF(type, name, num_args)
 *
 * |type| is one of
 *   EMU: fully emulated syscall
 *   EXEC: fully executed syscall
 *   EXEC_RET_EMU: syscall is executed, but retval is emulated
 *   IRREGULAR: doesn't adhere to regular format (e.g. meta-syscalls
 *     like socketcall)
 *
 * |name| is the short syscall descriptor, e.g. "close".  |num_args|
 * is the number of recorded arguments.  Note: |num_args| is
 * meaningless for IRREGULAR syscalls.
 *
 * "Irregular" syscalls, those with special semantics or
 * conditionally-recorded arguments, are *not* defined here.
 * Includers must deal with them specially.
 */

/* Please keep the list of syscalls alphabetized by syscall name. */

/**
 *  int access(const char *pathname, int mode);
 *
 * access() checks whether the calling process can access the file
 * pathname.  If pathname is a symbolic link, it is dereferenced.
 */
SYSCALL_DEF(EXEC_RET_EMU, access, 0)

/**
 *  unsigned int alarm(unsigned int seconds)
 *
 * The alarm() system call schedules an alarm. The process will get a
 * SIGALRM after the requested amount of seconds.
 */
SYSCALL_DEF(EMU, alarm, 0)

/**
 *  int brk(void *addr)
 *
 * brk() and sbrk() change the location of the program break, which
 * defines the end of the process's data segment (i.e., theprogram
 * break is the first location after the end of the uninitialized data
 * segment).  Increasing the program break has the effect of
 * allocating memory to the process; decreasing the break deallocates
 * memory.

 * brk() sets the end of the data segment to the value specified by
 * addr, when that value is reasonable, the system has enough memory,
 * and the process does not exceed its maximum data size (see
 * setrlimit(2)).
 */
SYSCALL_DEF(EXEC, brk, 0)

/**
 *  int chdir(const char *path);
 *
 * chdir() changes the current working directory of the calling
 * process to the directory specified in path.
 */
SYSCALL_DEF(EMU, chdir, 0)

/**
 *  int chmod(const char *path, mode_t mode)
 *
 * The mode of the file given by path or referenced by fildes is
 * changed.
 */
SYSCALL_DEF(EMU, chmod, 0)

/**
 *  int clock_gettime(clockid_t clk_id, struct timespec *tp);
 *
 * The functions clock_gettime() and clock_settime() retrieve and set
 * the time of the specified clock clk_id.
 */
SYSCALL_DEF(EMU, clock_gettime, 1)

/**
 *  int clock_getres(clockid_t clk_id, struct timespec *res)
 *
 * The function clock_getres() finds the resolution (precision) of the
 * specified clock clk_id, and, if res is non-NULL, stores it in the
 * struct timespec pointed to by res.  The resolution of clocks
 * depends on the implementation and cannot be configured by a
 * particular process.  If the time value pointed to by the argument
 * tp of clock_settime() is not a multiple of res, then it is
 * truncated to a multiple of res.
 */
SYSCALL_DEF(EMU, clock_getres, 1)

/**
 *  int clone(int (*fn)(void *), void *child_stack, int flags, void *arg, (pid_t *ptid, struct user_desc *tls, pid_t *ctid));
 *
 * clone() creates a new process, in a manner similar to fork(2).  It
 * is actually a library function layered ontrace top of the
 * underlying clone() system call, hereinafter referred to tidas
 * sys_clone.  A description of sys_clone is given towards the end of
 * this page.
 *
 * NOTE: clone is actually implemented by sys_clone which has the
 * following signature:
 *
 *  long sys_clone(unsigned long clone_flags, unsigned long newsp, void __user *parent_tid, void __user *child_tid, struct pt_regs *regs)
 */
SYSCALL_DEF(IRREGULAR, clone, -1)

/**
 *  int close(int fd)
 *
 * close() closes a file descriptor, so that it no longer refers to
 * any file and may be reused.  Any record locks (see fcntl(2)) held
 * on the file it was associated with, and owned by the process, are
 * removed (regardless of the file descriptor that was used to obtain
 * the lock).
 */
SYSCALL_DEF(EMU, close, 0)

/**
 *  int creat(const char *pathname, mode_t mode);
 *
 * creat() is equivalent to open() with flags equal to
 * O_CREAT|O_WRONLY|O_TRUNC.
 */
SYSCALL_DEF(EMU, creat, 0)

/**
 *  int dup(int oldfd)
 *
 * dup() uses the lowest-numbered unused descriptor for the new
 * descriptor.
 */
SYSCALL_DEF(EMU, dup, 0)

/**
 *  int dup2(int oldfd, int newfd)
 *
 * dup2() makes newfd be the copy of oldfd, closing newfd first if
 *  necessary, but note the following..
 */
SYSCALL_DEF(EMU, dup2, 0)

/**
 *  int epoll_create(int size);
 *
 * epoll_create() creates an epoll "instance", requesting the kernel
 * to allocate an event backing store dimensioned for size
 * descriptors.  The size is not the maximum size of the backing store
 * but just a hint to the kernel about how to dimension internal
 * structures.  When no longer required, the file descriptor returned
 * by epoll_create() should be closed by using close(2).
 */
SYSCALL_DEF(EMU, epoll_create, 0)

/**
 *  int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
 *
 * This system call performs control operations on the epoll instance
 * referred to by the file descriptor epfd.  It requests that the
 * operation op be performed for the target file descriptor, fd.
 */
SYSCALL_DEF(EMU, epoll_ctl, 0)

/**
 *  int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
 *
 * The epoll_wait() system call waits for events on the epoll instance
 * referred to by the file descriptor epfd.  The memory area pointed
 * to by events will contain the events that will be available for the
 * caller.  Up to maxevents are returned by epoll_wait().  The
 * maxevents argument must be greater than zero.
 *
 * XXX is this irregular?  CHECKED: (trace->recorded_regs.eax >= 0)
 */
SYSCALL_DEF(EMU, epoll_wait, 1)

/**
 *  int eventfd(unsigned int initval, int flags);
 *
 * eventfd() creates an "eventfd object" that can be used as an event
 * wait/notify mechanism by userspace applications, and by the kernel
 * to notify userspace applications of events.  The object contains an
 * unsigned 64-bit integer (uint64_t) counter that is maintained by
 * the kernel.  This counter is initialized with the value specified
 * in the argument initval.
 */
SYSCALL_DEF(EMU, eventfd2, 0)

/**
 *  int execve(const char *filename, char *const argv[], char *const envp[]);
 *
 * execve() executes the program pointed to by filename.
 */
SYSCALL_DEF(IRREGULAR, execve, -1)

/**
 *  void exit(int status)
 *
 * The exit() function causes normal process termination and the value
 * of status & 0377 is returned to the parent (see wait(2)).
 */
SYSCALL_DEF(IRREGULAR, exit, -1)

/**
 *  void exit_group(int status)
 *
 * This system call is equivalent to exit(2) except that it terminates
 * not only the calling thread, but all threads in the calling
 * process's thread group.
 */
SYSCALL_DEF(IRREGULAR, exit_group, -1)

/**
 *  int faccessat(int dirfd, const char *pathname, int mode, int flags)
 *
 * The faccessat() system call operates in exactly the same way as
 * access(2), except for the differences described in this manual
 * page....
 */
SYSCALL_DEF(EMU, faccessat, 0)

/**
 *  int posix_fadvise(int fd, off_t offset, off_t len, int advice);
 *
 * Programs can use posix_fadvise() to announce an intention to access
 * file data in a specific pattern in the future, thus allowing the
 * kernel to perform appropriate optimizations.
 */
SYSCALL_DEF(EMU, fadvise64, 0)
SYSCALL_DEF(EMU, fadvise64_64, 0)

/**
 * int fallocate(int fd, int mode, off_t offset, off_t len);
 *
 * fallocate() allows the caller to directly manipulate the allocated
 * disk space for the file referred to by fd for the byte range
 * starting at offset and continuing for len bytes
 */
SYSCALL_DEF(EMU, fallocate, 0)

/**
 *  int fchdir(int fd);
 *
 * fchdir() is identical to chdir(); the only difference is that the
 * directory is given as an open file descriptor.
 */
SYSCALL_DEF(EMU, fchdir, 0)

/**
 *  int fchmod(int fd, mode_t mode);
 *
 * fchmod() changes the permissions of the file referred to by the
 * open file descriptor fd
 */
SYSCALL_DEF(EMU, fchmod, 0)

/**
 *  int fcntl(int fd, int cmd, ... ( arg ));
 *
 * fcntl() performs one of the operations described below on the open
 * file descriptor fd.  The operation is determined by cmd. fcntl()
 * can take an optional third argument.  Whether or not this argument
 * is required is determined by cmd. The required argument type is
 * indicated in parentheses after each cmd name (in most cases, the
 * required type is long, and we identify the argument using the name
 * arg), or void is specified if the argument is not required.
 */
SYSCALL_DEF(IRREGULAR, fcntl64, -1)

/**
 *  int fstat(int fd, struct stat *buf)
 *
 * fstat() is identical to stat(), except that the file to be stat-ed
 * is specified by the file descriptor fd.
 */
SYSCALL_DEF(EMU, fstat64, 1)

/**
 *  int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags);
 *
 * The fstatat() system call operates in exactly the same way as
 * stat(2), except for the differences described in this manual
 * page....
 */
SYSCALL_DEF(EMU, fstatat64, 1)

/**
 *  int fstatfs(int fd, struct statfs *buf)
 *
 * The function statfs() returns information about a mounted file
 * system.  path is the pathname of any file within the
 * get_time(GET_TID(thread_id));mounted file system.  buf is a pointer
 * to a statfs structure defined approximately as follows:
 */
SYSCALL_DEF(EMU, fstatfs, 1)
SYSCALL_DEF(EMU, fstatfs64, 1)

/**
 *  int fsync(int fd)
 *
 * fsync() transfers ("flushes") all modified in-core data of (i.e.,
 * modified buffer cache pages for) the file referred to by the file
 * descriptor fd to the disk device (or other permanent storage
 * device) where that file resides.  The call blocks until the device
 * reports that the transfer has completed.  It also flushes metadata
 * information associated with the file (see stat(2))
 */
SYSCALL_DEF(EMU, fsync, 0)

/**
 *  int fdatasync(int fd)
 *
 * fdatasync() is similar to fsync(), but does not flush modified
 * metadata unless that metadata is needed in order to allow a
 * subsequent data retrieval to be correctly handled.  For example,
 * changes to st_atime or st_mtime (respectively, time of last access
 * and time of last modification; see stat(2)) do not require flushing
 * because they are not necessary for a subsequent data read to be
 * handled correctly.  On the other hand, a change to the file size
 * (st_size, as made by say ftruncate(2)), would require a metadata
 * flush
 */
SYSCALL_DEF(EMU, fdatasync, 0)

/**
 *  int ftruncate(int fd, off_t length)
 *
 * The truncate() and ftruncate() functions cause the regular file
 * named by path or referenced by fd to be truncated to a size of
 * precisely length bytes.
 */
SYSCALL_DEF(EMU, ftruncate64, 0)
SYSCALL_DEF(EMU, ftruncate, 0)
SYSCALL_DEF(EMU, truncate, 0)

/**
 *  int futex(int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3);
 *
 * The futex() system call provides a method for a program to wait for
 * a value at a given address to change, and a method to wake up
 * anyone waiting on a particular address (while the addresses for the
 * same memory in separate processes may not be equal, the kernel
 * maps them internally so the same memory mapped in different
 * locations will correspond for futex() calls).  This system call is
 * typically used to implement the contended case of a lock in shared
 * memory, as described in futex(7).
 */
SYSCALL_DEF(IRREGULAR, futex, -1)

/**
 *  char *getwd(char *buf);
 *
 * These functions return a null-terminated string containing an
 * absolute pathname that is the current working directory of the
 * calling process.  The pathname is returned as the function result
 * and via the argument buf, if present.
 */
SYSCALL_DEF(EMU, getcwd, 1)

/**
 *  int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
 *
 * The system call getdents() reads several linux_dirent structures
 * from the directory referred to by the open file descriptor fd into
 * the buffer pointed to by dirp.  The argument count specifies the
 * size of that buffer.
 */
SYSCALL_DEF(EMU, getdents, 1)
SYSCALL_DEF(EMU, getdents64, 1)

/**
 *  gid_t getegid(void);
 *
 * getegid() returns the effective group ID of the calling process.
 */
SYSCALL_DEF(EMU, getegid32, 0)

/**
 *  uid_t geteuid(void);
 *
 * geteuid() returns the effective user ID of the calling process.
 */
SYSCALL_DEF(EMU, geteuid32, 0)

/**
 *  int getgroups(int size, gid_t list[]);
 *
 * getgroups() returns the supplementary group IDs of the calling
 * process in list.  The argument size should be set to the maximum
 * number of items that can be stored in the buffer pointed to by
 * list. If the calling process is a member of more than size
 * supplementary groups, then an error results.  It is unspecified
 * whether the effective group ID of the calling process is included
 * in the returned list. (Thus, an application should also call
 * getegid(2) and add or remove the resulting value.)
 *
 * If size is zero, list is not modified, but the total number of
 * supplementary group IDs for the process is returned.  This allows
 * the caller to determine the size of a dynamically allocated list to
 * be used in a further call to getgroups().
 *
 * NOTE: This system call is executed although it should also be
 * possible to completely emulate the system call. However, the call
 * seems to change a register value (ecx) if the register is not zero.
 * This is very strange.
 */
SYSCALL_DEF(EXEC, getgroups32, 1)

/**
 *  pid_t getpgrp(void)
 *
 * The POSIX.1 getpgrp() always returns the PGID of the caller.
 */
SYSCALL_DEF(EMU, getpgrp, 0)

/**
 *  gid_t getgid(void);
 *
 * getgid() returns the real group ID of the calling process.
 */
SYSCALL_DEF(EMU, getgid32, 0)

/**
 *  pid_t getpid(void);
 *
 * getpid() returns the process ID of the calling process.  (This is
 * often used by routines that generate unique temporary
 * filenames.)
 */
SYSCALL_DEF(EMU, getpid, 0)

/**
 *  pid_t getppid(void);
 *
 * getppid() returns the process ID of the parent of the calling
 * process.
 */
SYSCALL_DEF(EMU, getppid, 0)

/**
 *  int getpriority(int which, int who);
 *
 * The scheduling priority of the process, process group, or user, as
 * indicated by which and who is obtained with the getpriority() call.
 */
SYSCALL_DEF(EMU, getpriority, 0)

/**
 *  int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
 *
 * getresuid() returns the real UID, the effective UID, and the saved
 * set-user-ID of the calling process, in the arguments ruid, euid,
 * and suid, respectively.  getresgid() performs the analogous task
 * for the process's group IDs.  @return: On success, zero is
 * returned.  On error, -1 is returned, and errno is set
 * appropriately.
 */
SYSCALL_DEF(EMU, getresgid32, 3)

/**
 *  int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid)
 *
 * getresuid() returns the real UID, the effective UID, and the saved
 * set- user-ID of the calling process, in the arguments ruid, euid,
 * and suid, respectively.  getresgid() performs the analogous task
 * for the process's group IDs.
 */
SYSCALL_DEF(EMU, getresuid32, 3)

/**
 *  int getrusage(int who, struct rusage *usage)
 *
 * getrusage() returns resource usage measures for who, which can be
 * one of the following..
 */
SYSCALL_DEF(EMU, getrusage, 1)

/**
 *  pid_t gettid(void);
 *
 * gettid() returns the caller's thread ID (TID).
 */
SYSCALL_DEF(EMU, gettid, 0)

/**
 *  int gettimeofday(struct timeval *tv, struct timezone *tz);
 *
 * The functions gettimeofday() and settimeofday() can get and set the
 * time as well as a timezone.  The tv argument is a struct timeval
 * (as specified in <sys/time.h>):
 */
SYSCALL_DEF(EMU, gettimeofday, 2)

/**
 *  uid_t getuid(void);
 *
 * getuid() returns the real user ID of the calling process
 */
SYSCALL_DEF(EMU, getuid32, 0)

/**
 *  int inotify_add_watch(int fd, const char *pathname, uint32_t mask)
 *
 * inotify_add_watch() adds a new watch, or modifies an existing
 * watch, for the file whose location is specified in pathname; the
 * caller must have read permission for this file.  The fd argument is
 * a file descrip tor referring to the inotify instance whose watch
 * list is to be modified.  The events to be monitored for pathname
 * are specified in the mask bit-mask argument.  See inotify(7) for a
 * description of the bits that can be set in mask.
 */
SYSCALL_DEF(EMU, inotify_add_watch, 0)

/**
 *  int inotify_init(void)
 *
 * inotify_init() initializes a new inotify instance and returns a
 * file descriptor associated with a new inotify event queue.
 */
SYSCALL_DEF(EMU, inotify_init, 0)
SYSCALL_DEF(EMU, inotify_init1, 0)

/**
 *  int inotify_rm_watch(int fd, uint32_t wd)
 *
 * inotify_rm_watch() removes the watch associated with the watch
 * descriptor wd from the inotify instance associated with the file
 * descriptor fd.
 */
SYSCALL_DEF(EMU, inotify_rm_watch, 0)

/**
 *  int ioctl(int d, int request, ...)
 *
 * The ioctl() function manipulates the underlying device parameters
 * of special files.  In particular, many operating characteristics of
 * character special files (e.g., terminals) may be controlled with
 * ioctl() requests.  The argument d must be an open file descriptor.
 *
 */
SYSCALL_DEF(IRREGULAR, ioctl, -1)

/**
 *  int ipc(unsigned int call, int first, int second, int third, void *ptr, long fifth);
 *
 * ipc() is a common kernel entry point for the System V IPC calls for
 * messages, semaphores, and shared memory.  call determines which IPC
 * function to invoke; the other arguments are passed through to the
 * appropriate call.
 */
SYSCALL_DEF(IRREGULAR, ipc, -1)

/**
 *  int kill(pid_t pid, int sig)
 *
 * The kill() system call can be used to send any signal to any
 * process group or process.
 */
SYSCALL_DEF(EMU, kill, 0)

/**
 *  ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size);
 *
 * getxattr() retrieves the value of the extended attribute identified
 * by name and associated with the given path in the file system.  The
 * length of the attribute value is returned.
 *
 * lgetxattr() is identical to getxattr(), except in the case of a
 * symbolic link, where the link itself is interrogated, not the file
 * that it refers to.
 *
 * fgetxattr() is identical to getxattr(), only the open file referred
 * to by fd (as returned by open(2)) is interrogated in place of path.
 */
SYSCALL_DEF(EMU, lgetxattr, 1)

/**
 *  int link(const char *oldpath, const char *newpath);
 *
 * link() creates a new link (also known as a hard link) to an
 * existing file.
 */
SYSCALL_DEF(EMU, link, 0)

/**
 *  int _llseek(unsigned int fd, unsigned long offset_high, unsigned long offset_low, loff_t *result, unsigned int whence);
 *
 * The _llseek() function repositions the offset of the open file
 * associated with the file descriptor fd to (off‐ set_high<<32) |
 * offset_low bytes relative to the beginning of the file, the current
 * position in the file, or the end of the file, depending on whether
 * whence is SEEK_SET, SEEK_CUR, or SEEK_END, respectively.  It
 * returns the resulting file position in the argument result.
 */
SYSCALL_DEF(EMU, _llseek, 1)

/**
 *  off_t lseek(int fd, off_t offset, int whence)
 *
 * The lseek() function repositions the offset of the open file
 * associated with the file descriptor fd to the argument offset
 * according to the directive whence as follows:
 */
SYSCALL_DEF(EMU, lseek, 0)

/**
 *  int lstat(const char *path, struct stat *buf);
 *
 * lstat() is identical to stat(), except that if path is a symbolic
 * link, then the link itself is stat-ed, not the file that it refers
 * to.
 */
SYSCALL_DEF(EMU, lstat64, 1)

/**
 *  int mkdir(const char *pathname, mode_t mode);
 *
 * mkdir() attempts to create a directory named pathname.
 */
SYSCALL_DEF(EMU, mkdir, 0)

/**
 *  int mkdirat(int dirfd, const char *pathname, mode_t mode);
 *
 * The mkdirat() system call operates in exactly the same way as
 * mkdir(2), except for the differences described in this manual
 * page....
 */
SYSCALL_DEF(EMU, mkdirat, 0)

/**
 *  int madvise(void *addr, size_t length, int advice);
 *
 * The madvise() system call advises the kernel about how to handle
 * paging input/output in the address range beginning at address addr
 * and with size length bytes.  It allows an application to tell the
 * kernel how it expects to use some mapped or shared memory areas, so
 * that the kernel can choose appropriate read-ahead and caching
 * techniques.  This call does not influence the semantics of the
 * application (except in the case of MADV_DONTNEED), but may
 * influence its performance.  The kernel is free to ignore the
 * advice.
 */
SYSCALL_DEF(EXEC, madvise, 0)

/**
 *  void *mmap2(void *addr, size_t length, int prot,int flags, int fd, off_t pgoffset);
 *
 * The mmap2() system call operates in exactly the same way as
 * mmap(2), except that the final argument specifies the offset into
 * the file in 4096-byte units (instead of bytes, as is done by
 * mmap(2)).  This enables applications that use a 32-bit off_t to map
 * large files (up to 2^44 bytes).
 */
SYSCALL_DEF(IRREGULAR, mmap2, -1)

/**
 *  int mprotect(const void *addr, size_t len, int prot)
 *
 * mprotect() changes protection for the calling process's memory
 * page(s) containing any part of the address range in the interval
 * [addr, addr+len-1].  addr must be aligned to a page boundary.
 *
 * If the calling process tries to access memory in a manner that
 * violates the protection, then the kernel generates a SIGSEGV signal
 * for the process.
 */
SYSCALL_DEF(EXEC, mprotect, 0)

/**
 *  void *mremap(void *old_address, size_t old_size, size_t new_size, int flags, ... ( void *new_address ));
 *
 * mremap() expands (or shrinks) an existing memory mapping,
 * potentially moving it at the same time (controlled by the flags
 * argument and the available virtual address space).
 */
SYSCALL_DEF(IRREGULAR, mremap, -1)

/**
 *  int munmap(void *addr, size_t length)
 *
 * The munmap() system call deletes the mappings for the specified
 * address range, and causes further references to addresses within
 * the range to generate invalid memory references.  The region is
 * also automatically unmapped when the process is terminated.  On the
 * other hand, closing the file descriptor does not unmap the region.
 */
SYSCALL_DEF(EXEC, munmap, 0)

/**
 *  int nanosleep(const struct timespec *req, struct timespec *rem)
 *
 * nanosleep() suspends the execution of the calling thread until
 * either at least the time specified in *req has elapsed, or the
 * delivery of a signal that triggers the invocation of a handler in
 * the calling thread or that ter- minates the process.
 *
 * CHECKED: trace->recorded_regs.ecx != NULL
 */
SYSCALL_DEF(IRREGULAR, nanosleep, -1)

/**
 *  int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
 *
 * select() and pselect() allow a program to monitor multiple file
 * descriptors, waiting until one or more of the file descriptors
 * become "ready" for some class of I/O operation (e.g., input
 * possible).  A file descriptor is considered ready if it is possible
 * to perform the corresponding I/O operation (e.g., read(2)) without
 * blocking.
 */
SYSCALL_DEF(EMU, _newselect, 4)

/**
 *  int open(const char *pathname, int flags)
 *  int open(const char *pathname, int flags, mode_t mode)
 *
 * Given a pathname for a file, open() returns a file descriptor, a
 * small, nonnegative integer for use in subsequent system calls
 * (read(2), write(2), lseek(2), fcntl(2), etc.).  The file descriptor
 * returned by a successful call will be the lowest-numbered file
 * descriptor not currently open for the process.
 */
SYSCALL_DEF(EMU, open, 0)

/**
 *  int openat(int dirfd, const char *pathname, int flags);
 *  int openat(int dirfd, const char *pathname, int flags, mode_t mode);
 *
 * The openat() system call operates in exactly the same way as
 * open(2), except for the differences described in this manual page.
 */
SYSCALL_DEF(EMU, openat, 0)

/**
 *  int perf_event_open(struct perf_event_attr *attr,
 *                      pid_t pid, int cpu, int group_fd,
 *                      unsigned long flags);
 *
 * Given a list of parameters, perf_event_open() returns a file
 * descriptor, for use in subsequent system calls (read(2), mmap(2),
 * prctl(2), fcntl(2), etc.).
 */
SYSCALL_DEF(EMU, perf_event_open, 0)

/**
 *  int pipe(int pipefd[2]);
 *
 * pipe() creates a pipe, a unidirectional data channel that can be
 * used for interprocess communication.  The array pipefd is used to
 * return two file descriptors referring to the ends of the pipe.
 * pipefd[0] refers to the read end of the pipe.  pipefd[1] refers to
 * the write end of the pipe.  Data writ‐ ten to the write end of the
 * pipe is buffered by the kernel until it is read from the read end
 * of the pipe.  For further details, see pipe(7).
 */
SYSCALL_DEF(EMU, pipe, 2)

/**
 *  int pipe2(int pipefd[2], int flags)
 *
 * If flags is 0, then pipe2() is the same as pipe().  The following
 * values can be bitwise ORed in flags to obtain different behavior...
 */
SYSCALL_DEF(EMU, pipe2, 2)

/**
 *  int poll(struct pollfd *fds, nfds_t nfds, int timeout)
 *  int ppoll(struct pollfd *fds, nfds_t nfds,
 *            const struct timespec *timeout_ts,
 *            const sigset_t *sigmask);
 *
 * poll() performs a similar task to select(2): it waits for one of a
 * set of file descriptors to become ready to perform I/O.
 *
 * The relationship between poll() and ppoll() is analogous to the
 * relationship between select(2) and pselect(2): like pselect(2),
 * ppoll() allows an application to safely wait until either a file
 * descriptor becomes ready or until a signal is caught.
 *
 * XXX is this irregular?  CHECKED: (trace->recorded_regs.eax > 0)
 */
SYSCALL_DEF(EMU, poll, 1)
SYSCALL_DEF(EMU, ppoll, 1)

/**
 *  int prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
 *
 * prctl() is called with a first argument describing what to do (with
 * values defined in <linux/prctl.h>), and further arguments with a
 * significance depending on the first one.
 *
 */
SYSCALL_DEF(EMU, prctl, 1)

/**
 *  ssize_t pread(int fd, void *buf, size_t count, off_t offset);
 *
 * pread, pwrite - read from or write to a file descriptor at a given
 * offset
 */
SYSCALL_DEF(EMU, pread64, 1)
SYSCALL_DEF(EMU, pwrite64, 0)

/**
 *  int prlimit(pid_t pid, int resource, const struct rlimit *new_limit, struct rlimit *old_limit);
 *
 * The Linux-specific prlimit() system call combines and extends the
 * functionality of setrlimit() and getrlimit().  It can be used to
 * both set and get the resource limits of an arbitrary process.
 */
SYSCALL_DEF(EXEC, prlimit64, 1)

/**
 *  int quotactl(int cmd, const char *special, int id, caddr_t addr);
 *
 * The quotactl() call manipulates disk quotas.  The cmd argument
 * indicates a command to be applied to the user or group ID
 * specified in id.  To initialize the cmd argument, use the
 * QCMD(subcmd, type) macro.  The type value is either USRQUOTA, for
 * user quotas, or GRPQUOTA, for group quotas.  The subcmd value is
 * described below.
 */
SYSCALL_DEF(IRREGULAR, quotactl, -1)

/**
 *  ssize_t read(int fd, void *buf, size_t count);
 *
 * read() attempts to read up to count bytes from file descriptor fd
 * into the buffer starting at buf.
 *
 * CHECKED: (trace->recorded_regs.eax > 0)
 */
SYSCALL_DEF(IRREGULAR, read, -1)

/**
 *  ssize_t readahead(int fd, off64_t offset, size_t count);
 *
 * readahead() populates the page cache with data from a file so that
 * subsequent reads from that file will not block on disk I/O.  The fd
 * argument is a file descriptor identifying the file which is to be
 * read.  The offset argu- ment specifies the starting point from
 * which data is to be read and count specifies the number of bytes to
 * be read.  I/O is performed in whole pages, so that offset is
 * effectively rounded down to a page boundary and bytes are read up
 * to the next page boundary greater than or equal to (offset+count).
 * readahead() does not read beyond the end of the file.  readahead()
 * blocks until the specified data has been read.  The current file
 * offset of the open file referred to by fd is left unchanged.
 */
SYSCALL_DEF(EMU, readahead, 0)

/**
 *  ssize_t readlink(const char *path, char *buf, size_t bufsiz);
 *
 * readlink() places the contents of the symbolic link path in the
 * buffer buf, which has size bufsiz. readlink() does not append a
 * null byte to buf.  It will truncate the contents (to a length of
 * bufsiz characters), in case the buffer is too small to hold all of
 * the contents.
 */
SYSCALL_DEF(EMU, readlink, 1)

/**
 *  int recvmmsg(int sockfd, struct mmsghdr *msgvec,
 *               unsigned int vlen, unsigned int flags,
 *               struct timespec *timeout);
 *
 * The recvmmsg() system call is an extension of recvmsg(2) that
 * allows the caller to receive multiple messages from a socket using
 * a single system call.  (This has performance benefits for some
 * applications.)  A further extension over recvmsg(2) is support for
 * a timeout on the receive operation.
 */
SYSCALL_DEF(IRREGULAR, recvmmsg, -1)

/**
 *  int rename(const char *oldpath, const char *newpath)
 *
 * rename() renames a file, moving it between directories if required.
 */
SYSCALL_DEF(EMU, rename, 0)

/**
 *  int rmdir(const char *pathname)
 *
 * rmdir() deletes a directory, which must be empty.
 */
SYSCALL_DEF(EMU, rmdir, 0)

/**
 *  int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
 *
 * sched_getaffinity() writes the affinity mask of the process whose
 * ID is pid into the cpu_set_t structure pointed to by mask.  The
 * cpusetsize argument specifies the size (in bytes) of mask.  If pid
 * is zero, then the mask of the calling process is returned.
 */
SYSCALL_DEF(EMU, sched_getaffinity, 1)

/**
 *  int sched_getparam(pid_t pid, struct sched_param *param)
 *
 * sched_getparam() retrieves the scheduling parameters for the
 * process i dentified by pid.  If pid is zero, then the parameters of
 * the calling process are retrieved.
 */
SYSCALL_DEF(EMU, sched_getparam, 1)

/**
 *  int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
 *
 * sched_setaffinity() sets the CPU affinity mask of the process whose
 * ID is pid to the value specified by mask.  If pid is zero, then the
 * calling process is used.  The argument cpusetsize is the length
 * (in bytes) of the data pointed to by mask.  Normally this argument
 * would be specified as sizeof(cpu_set_t).
 */
SYSCALL_DEF(EMU, sched_setaffinity, 0)

/**
 *  int sched_get_priority_max(int policy)
 *
 * sched_get_priority_max() returns the maximum priority value that
 * can be used with the scheduling algorithm identified by policy.
 */
SYSCALL_DEF(EMU, sched_get_priority_max, 0)

/**
 *  int sched_get_priority_min(int policy)
 *
 * sched_get_priority_min() returns the minimum priority value that
 * can be used with the scheduling algorithm identified by policy.
 */
SYSCALL_DEF(EMU, sched_get_priority_min, 0)

/**
 *  int sched_getscheduler(pid_t pid);
 *
 * sched_getscheduler() queries the scheduling policy currently
 * applied to the process identified by pid.  If pid equals zero, the
 * policy of the calling process will be retrieved.
 */
SYSCALL_DEF(EMU, sched_getscheduler, 0)

/**
 *  int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
 *
 * sched_setscheduler() sets both the scheduling policy and the
 * associated parameters for the process whose ID is specified in pid.
 * If pid equals zero, the scheduling policy and parameters of the
 * calling process will be set.  The interpretation of the argument
 * param depends on the selected policy.
 */
SYSCALL_DEF(EMU, sched_setscheduler, 0)

/**
 *  int sched_yield(void)
 *
 * sched_yield() causes the calling thread to relinquish the CPU.  The
 * thread is moved to the end of the queue for its static priority and
 * a new thread gets to run.
 */
SYSCALL_DEF(EMU, sched_yield, 0)

/**
 *  int sendmmsg(int sockfd, struct mmsghdr *msgvec, unsigned int vlen,
 *               unsigned int flags);
 *
 * The sendmmsg() system call is an extension of sendmsg(2) that
 * allows the caller to transmit multiple messages on a socket using a
 * single system call.  (This has performance benefits for some
 * applications.)
 */
SYSCALL_DEF(IRREGULAR, sendmmsg, -1)

/**
 *  int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
 *
 * The function setitimer() sets the specified timer to the value in
 * new_value.  If old_value is non-NULL, the old value of the timer is
 * stored there.
 */
SYSCALL_DEF(EMU, setitimer, 1)

/**
 *  int setpgid(pid_t pid, pid_t pgid);
 *
 * setpgid() sets the PGID of the process specified by pid to pgid.
 * If pid is zero, then the process ID of the calling process is used.
 * If pgid is zero, then the PGID of the process specified by pid is
 * made the same as its process ID.  If setpgid() is used to move a
 * process from one process group to another (as is done by some
 * shells when creating pipelines), both process groups must be part
 * of the same session (see setsid(2) and credentials(7)).  In this
 * case, the pgid specifies an existing process group to be joined and
 * the session ID of that group must match the session ID of the
 * joining process.
 */
SYSCALL_DEF(IRREGULAR, setpgid, -1)

/**
 *  int setpriority(int which, int who, int prio);
 *
 * The scheduling priority of the process, process group, or user, as
 * indicated by which and who is obtained with the getpriority() call
 * and set with the setpriority() call.
 */
SYSCALL_DEF(EMU, setpriority, 0)

/**
 *  int setregid(gid_t rgid, gid_t egid)
 *
 * setreuid() sets real and effective user IDs of the calling process
 */
SYSCALL_DEF(EMU, setregid32, 0)

/**
 *  int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
 *
 * setresgid() sets the real GID, effective GID, and saved
 * set-group-ID of the calling process.
 */
SYSCALL_DEF(EMU, setresgid, 0)

/**
 *  int setresgid32(gid_t rgid, gid_t egid, gid_t sgid);
 *
 * setresgid() sets the real GID, effective GID, and saved
 * set-group-ID of the calling process.
 */
SYSCALL_DEF(EMU, setresgid32, 0)

/**
 *  int setresuid(uid_t ruid, uid_t euid, uid_t suid);
 *
 * setresuid() sets the real user ID, the effective user ID, and the
 * saved set-user-ID of the calling process.
 */
SYSCALL_DEF(EMU, setresuid, 0)

/**
 *  int setresuid32(uid_t ruid, uid_t euid, uid_t suid);
 *
 * setresuid() sets the real user ID, the effective user ID, and the
 * saved set-user-ID of the calling process.
 */
SYSCALL_DEF(EMU, setresuid32, 0)

/**
 *  int setrlimit(int resource, const struct rlimit *rlim)
 *
 * getrlimit() and setrlimit() get and set resource limits
 * respectively.  Each resource has an associated soft and hard limit,
 * as defined by the rlimit structure (the rlim argument to both
 * getrlimit() and setrlimit()):
 *
 * NOTE: We should execute this system call, since this system call
 * sets a limit on a resource (e.g., the stack size) This bahavior
 * must be the same in the replay as in the recording phase.
 */
SYSCALL_DEF(EXEC, setrlimit, 1)

/**
 *  long set_robust_list(struct robust_list_head *head, size_t len)
 *
 * The robust futex implementation needs to maintain per-thread lists
 * of robust futexes which are unlocked when the thread exits. These
 * lists are managed in user space, the kernel is only notified about
 * the location of the head of the list.
 *
 * set_robust_list sets the head of the list of robust futexes owned
 * by the current thread to head.  len is the size of *head.
 */
SYSCALL_DEF(EXEC, set_robust_list, 0)

/**
 *  int set_thread_area(struct user_desc *u_info)
 *
 * set_thread_area()  sets  an  entry in the current thread's Thread Local
 * Storage (TLS) array.  The TLS array entry set by set_thread_area() cor‐
 * responds  to  the  value of u_info->entry_number passed in by the user.
 * If this value is in bounds, set_thread_area() copies the TLS descriptor
 * pointed to by u_info into the thread's TLS array.
 *
 * When  set_thread_area() is passed an entry_number of -1, it uses a free
 * TLS entry.  If set_thread_area() finds a free TLS entry, the  value  of
 * u_info->entry_number  is  set  upon  return  to  show  which  entry was
 * changed.
 */
SYSCALL_DEF(EXEC, set_thread_area, 1)

/**
 *  long set_tid_address(int *tidptr);
 *
 * The kernel keeps for each process two values called set_child_tid
 * and clear_child_tid that are NULL by default.
 *
 * If a process is started using clone(2) with the CLONE_CHILD_SETTID
 * flag, set_child_tid is set to child_tidptr, the fifth argument of
 * that system call.
 *
 * When set_child_tid is set, the very first thing the new process
 * does is writing its PID at this address.
 */
SYSCALL_DEF(EXEC_RET_EMU, set_tid_address, 1)

/**
 *  int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
 *
 * The sigaction() system call is used to change the action taken by a
 * process on receipt of a specific signal.  (See signal(7) for an
 * overview of signals.)
 *
 * signum specifies the signal and can be any valid signal except
 * SIGKILL and SIGSTOP.
 *
 * If act is non-NULL, the new action for signal signum is installed
 * from act.  If oldact is non-NULL, the previous action is saved in
 * oldact.
 */
SYSCALL_DEF(EMU, sigaction, 1)
SYSCALL_DEF(EMU, rt_sigaction, 1)

/**
 *  int sigaltstack(const stack_t *ss, stack_t *oss)
 *
 * sigaltstack() allows a process to define a new alternate signal
 * stack and/or retrieve the state of an existing alternate signal
 * stack.  An alternate signal stack is used during the execution of a
 * signal handler if the establishment of that handler (see
 * sigaction(2)) requested it.
 */
SYSCALL_DEF(EMU, sigaltstack, 1)

/**
 *  int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
 *
 * sigprocmask() is used to fetch and/or change the signal mask of the
 * calling thread.  The signal mask is the set of signals whose
 * delivery is currently blocked for the caller (see also signal(7)
 * for more details).
 */
SYSCALL_DEF(EMU, sigprocmask, 1)
SYSCALL_DEF(EMU, rt_sigprocmask, 1)

/**
 *  int sigreturn(unsigned long __unused)
 *
 * When the Linux kernel creates the stack frame for a signal handler,
 * a call to sigreturn() is inserted into the stack frame so that upon
 * return from the signal handler, sigreturn() will be called.
 */
SYSCALL_DEF(IRREGULAR, sigreturn, -1)
SYSCALL_DEF(IRREGULAR, rt_sigreturn, -1)

/**
 *  int socketcall(int call, unsigned long *args)
 *
 * socketcall() is a common kernel entry point for the socket system
 * calls.  call determines which socket function to invoke.  args
 * points to a block containing the actual arguments, which are passed
 * through to the appropriate call.
 */
SYSCALL_DEF(IRREGULAR, socketcall, -1)

/**
 *  ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
 *
 * splice() moves data between two file descriptors without copying
 * between kernel address space and user address space.  It transfers
 * up to len bytes of data from the file descriptor fd_in to the file
 * descriptor fd_out, where one of the descriptors must refer to a
 * pipe.
 *
 * NB: the documentation doesn't mention it explicitly, but the |off|
 * params are actually inout params, and are updated with the new file
 * offset on return.
 *
 * NOTE: Technically, the following implementation is unsound for
 * programs that splice with stdin/stdout/stderr and have output
 * redirected during replay.  But, *crickets*.
 */
SYSCALL_DEF(EMU, splice, 2)

/**
 * int stat(const char *path, struct stat *buf);
 *
 * stat() stats the file pointed to by path and fills in buf.
 */
SYSCALL_DEF(EMU, stat64, 1)

/**
 *  int statfs(const char *path, struct statfs *buf)
 *
 * The function statfs() returns information about a mounted file
 * system.  path is the pathname of any file within the mounted file
 * system.  buf is a pointer to a statfs structure defined
 * approximately as follows:
 */
SYSCALL_DEF(EMU, statfs, 1)

/**
 *  int statfs(const char *path, struct statfs *buf)
 *
 * The function statfs() returns information about a mounted file
 * system.  path is the pathname of any file within the mounted file
 * system.  buf is a pointer to a statfs structure defined
 * approximately as follows...
 *
 * FIXME: we use edx here, although according to man pages this system
 * call has only 2 paramaters. However, strace tells another story...
 */
SYSCALL_DEF(EMU, statfs64, 1)

/**
 *  int symlink(const char *oldpath, const char *newpath)
 *
 * symlink() creates a symbolic link named newpath which contains the
 * string oldpath.
 *
 * FIXME: Why was this disabled?
 */
SYSCALL_DEF(EMU, symlink, 0)

/**
 *  int sysinfo(struct sysinfo *info)
 *
 * sysinfo() provides a simple way of getting overall system
 * statistics.
 */
SYSCALL_DEF(EMU, sysinfo, 1)

/**
 *  int tgkill(int tgid, int tid, int sig)
 *
 * tgkill() sends the signal sig to the thread with the thread ID tid
 * in the thread group tgid.  (By contrast, kill(2) can only be used
 * to send a signal to a process (i.e., thread group) as a whole, and
 * the signal will be delivered to an arbitrary thread within that
 * process.)
 */
SYSCALL_DEF(EMU, tgkill, 0)

/**
 *  time_t time(time_t *t);
 *
 * time() returns the time since the Epoch (00:00:00 UTC, January 1,
 * 1970), measured in seconds. If t is non-NULL, the return value is
 * also stored in the memory pointed to by t.
 */
SYSCALL_DEF(EMU, time, 1)

/**
 *  clock_t times(struct tms *buf)
 *
 * times() stores the current process times in the struct tms that buf
 *  points to.  The struct tms is as defined in <sys/times.h>:
 */
SYSCALL_DEF(EMU, times, 1)

/**
 *  int getrlimit(int resource, struct rlimit *rlim)
 *
 * getrlimit() and setrlimit() get and set resource limits
 * respectively.  Each resource has an associated soft and hard limit,
 * as defined by the rlimit structure (the rlim argument to both
 * getrlimit() and setrlimit()):
 */
SYSCALL_DEF(EMU, ugetrlimit, 1)

/**
 *  mode_t umask(mode_t mask);
 *
 * umask() sets the calling process's file mode creation mask (umask)
 * to mask & 0777 (i.e., only the file permission bits of mask are
 * used), and returns the previous value of the mask.
 */
SYSCALL_DEF(EMU, umask, 0)

/**
 *  int uname(struct utsname *buf)
 *
 * uname() returns system information in the structure pointed to by
 * buf. The utsname struct is defined in <sys/utsname.h>:
 */
SYSCALL_DEF(EMU, uname, 1)

/**
 *  int unlink(const char *path);
 *
 * The unlink() function shall remove a link to a file. If path names
 * a symbolic link, unlink() shall remove the symbolic link named by
 * path and shall not affect any file or directory named by the
 * contents of the symbolic link. Otherwise, unlink() shall remove the
 * link named by the pathname pointed to by path and shall decrement
 * the link count of the file referenced by the link.
 */
SYSCALL_DEF(EMU, unlink, 0)

/**
 *  int unlinkat(int dirfd, const char *pathname, int flags)
 *
 * The unlinkat() system call operates in exactly the same way as
 * either unlink(2) or rmdir(2) (depending on whether or not flags
 * includes the AT_REMOVEDIR flag) except for the differences
 * described in this manual page.
 */
SYSCALL_DEF(EMU, unlinkat, 0)

/**
 *  int utime(const char *filename, const struct utimbuf *times)
 *
 * The utime() system call changes the access and modification times
 * of the inode specified by filename to the actime and modtime fields
 * of times respectively.
 *
 * If times is NULL, then the access and modification times of the
 * file are set to the current time.
 *
 * Changing timestamps is permitted when: either the process has
 * appropriate privileges, or the effective user ID equals the user ID
 * of the file, or times is NULL and the process has write permission
 * for the file.
 *
 * FIXME: is mod_time set by the kernel?
 */
SYSCALL_DEF(EMU, utime, 0)

/**
 *  int utimes(const char *filename, const struct timeval times[2])
 *
 * The utime() system call changes the access and modification times
 * of the inode specified by filename to the actime and modtime fields
 * of times respectively.
 *
 */
SYSCALL_DEF(EMU, utimes, 1)

/**
 *  int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);
 *
 * utimensat() and futimens() update the timestamps of a file with
 * nanosecond precision.  This contrasts with the historical utime(2)
 * and utimes(2), which permit only second and microsecond precision,
 * respectively, when setting file timestamps.
 */
SYSCALL_DEF(EMU, utimensat, 0)

/**
 *  pid_t vfork(void);
 *
 * vfork(), just like fork(2), creates a child process of the calling
 * process.  For details and return value and errors, see fork(2).
 */
SYSCALL_DEF(IRREGULAR, vfork, -1)

/**
 *  pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
 *
 * The wait3() and wait4() system calls are similar to waitpid(2), but
 * additionally return resource usage information about the child in
 * the structure pointed to by rusage.
 */
SYSCALL_DEF(EXEC_RET_EMU, wait4, 2)

/**
 *  pid_t waitpid(pid_t pid, int *status, int options);
 *
 * The waitpid() system call suspends execution of the calling process
 * until a child specified by pid argument has changed state.  By
 * default, waitpid() waits only for terminated children, but this
 * behavior is modifiable via the options argument, as described
 * below....
 */
SYSCALL_DEF(EMU, waitpid, 1)

/**
 *  ssize_t write(int fd, const void *buf, size_t count);
 *
 * write() writes up to count bytes to the file referenced by the file
 * descriptor fd from the buffer starting at buf. POSIX requires that
 * a read() which can be proved to occur after a write() has returned
 * returns the new data. Note that not all file systems are POSIX
 * conforming.
 *
 * Note: write isn't irregular per se; we hook it to redirect output
 * to stdout/stderr during replay.
 */
SYSCALL_DEF(IRREGULAR, write, -1)

/**
 *  ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
 *
 * The writev() function writes iovcnt buffers of data described by
 * iov to the file associated with the file descriptor fd ("gather
 * output").
 */
SYSCALL_DEF(EMU, writev, 0)

/**
 *  void* rrcall_init_syscall_buffer(void* untraced_syscall_ip,
 *                                   struct sockaddr_un* addr,
 *                                   struct msghdr* msg, int* fdptr,
 *                                   struct socketcall_args* args_vec);
 *
 * Do what's necessary to map the shared syscall buffer region in the
 * caller's address space and return the mapped region.
 * |untraced_syscall_ip| lets rr know where our untraced syscalls will
 * originate from.  |addr| is the address of the control socket the
 * child expects to connect to.  |msg| is a pre-prepared IPC that can
 * be used to share fds; |fdptr| is a pointer to the control-message
 * data buffer where the fd number being shared will be stored.
 * |args_vec| provides the tracer with preallocated space to make
 * socketcall syscalls.
 *
 * This is a "magic" syscall implemented by rr.
 */
SYSCALL_DEF(IRREGULAR, rrcall_init_syscall_buffer, -1)
