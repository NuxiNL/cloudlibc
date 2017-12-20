// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <uv.h> - event loops
//
// Features missing:
// - UV_CONNECT, UV_UDP_IPV6ONLY, UV_UDP_REUSEADDR, UV_UDP_SEND,
//   uv_connect_cb, uv_connect_t, uv_connection_cb, uv_membership,
//   uv_udp_send_cb, uv_udp_send_t, uv_udp_t::send_queue_count,
//   uv_udp_t::send_queue_size, uv_listen(), uv_pipe_bind(),
//   uv_pipe_chmod(), uv_pipe_connect(), uv_pipe_pending_instances(),
//   uv_tcp_bind(), uv_tcp_connect(), uv_tcp_simultaneous_accepts(),
//   uv_udp_bind(), uv_udp_send(), uv_udp_set_broadcast(),
//   uv_udp_set_membership(), uv_udp_set_multicast_interface(),
//   uv_udp_set_multicast_loop(), uv_udp_set_multicast_ttl(),
//   uv_udp_set_ttl() and uv_udp_try_send():
//   Requires global network namespace.
// - UV_FS_ACCESS, UV_FS_CHMOD, UV_FS_CHOWN, UV_FS_COPYFILE, UV_FS_LINK,
//   UV_FS_LSTAT, UV_FS_MKDIR, UV_FS_MKDTEMP, UV_FS_O_*, UV_FS_READLINK,
//   UV_FS_REALPATH, UV_FS_RENAME, UV_FS_RMDIR, UV_FS_SCANDIR,
//   UV_FS_STAT, UV_FS_SYMLINK, UV_FS_UNLINK, UV_FS_UTIME, UV_FS_O_*,
//   uv_dirent_t, uv_fs_t::path, uv_fs_t::ptr, uv_chdir():, uv_cwd(),
//   uv_exepath(), uv_fs_access(), uv_fs_chmod(), uv_fs_copyfile(),
//   uv_fs_link(), uv_fs_lstat(), uv_fs_mkdir(), uv_fs_mkdtemp(),
//   uv_fs_open(), uv_fs_readlink(), uv_fs_realpath(), uv_fs_rename(),
//   uv_fs_rmdir(), uv_fs_scandir(), uv_fs_scandir_next(), uv_fs_stat(),
//   uv_fs_symlink(), uv_fs_unlink(), uv_fs_utime(), uv_os_homedir() and
//   uv_os_tmpdir():
//   Global filesystem namespace is not available.
// - UV_FS_EVENT, UV_FS_POLL, uv_fs_event* and uv_fs_poll*:
//   File system activity monitoring is not available.
// - UV_FS_FCHMOD, UV_FS_FCHOWN, uv_fs_fchmod(), uv_fs_fchown(),
//   uv_os_free_passwd() and uv_os_get_passwd():
//   UNIX credential management is not available.
// - UV_LOOP_BLOCK_SIGNAL, UV_SIGNAL, uv_signal_cb, uv_signal_t,
//   uv_loop_configure(), uv_signal_init() uv_signal_start(),
//   uv_signal_start_oneshot() and uv_signal_stop():
//   Signals cannot be handled in this environment. Consider sending
//   RPCs over sockets instead.
// - UV_PRIORITIZED:
//   Sending and receiving out-of-band data is unsupported.
// - uv_calloc_func, uv_free_func, uv_malloc_func, uv_realloc_func,
//   uv_default_loop() and uv_replace_allocator():
//   Introduces global state, which this implementation attempts to
//   avoid to improve thread safety.
// - uv_lib_t, uv_dlclose(), uv_dlerror(), uv_dlopen() and uv_dlsym():
//   This environment does not support loading libraries dynamically.
// - uv_pid_t, uv_process_t::pid, uv_kill(), uv_os_getpid() and
//   uv_os_getppid():
//   Global process namespace cannot be accessed.
// - uv_process_flags, uv_process_options_t, uv_stdio_container_t,
//   uv_stdio_flags and uv_spawn():
//   Launching unsandboxed processes is unsupported, as this would
//   circumvent this environment's security framework. Use <program.h>'s
//   program_spawn() instead.
// - uv_tty_mode_t, uv_tty_set_mode() and uv_tty_get_winsize():
//   Terminal attributes cannot be configured.
// - uv_backend_fd():
//   Event loops provided by this implementation are not backed by a
//   file descriptor.
// - uv_cpu_info(), uv_free_cpu_info(), uv_free_interface_addresses(),
//   uv_get_total_memory(), uv_interface_addresses(), uv_loadavg(),
//   uv_os_gethostname(), uv_resident_set_memory() and uv_uptime():
//   System-wide resource statistics are not available.
// - uv_get_process_title(), uv_set_process_title() and uv_setup_args():
//   Process title cannot be modified in this environment.
// - uv_loop_fork():
//   This environment does not support forking, which is why this
//   function is not necessary.
// - uv_pipe_getpeername(), uv_pipe_getsockname(), uv_tcp_getpeername(),
//   uv_tcp_getsockname(), uv_udp_getsockname():
//   Socket address metadata is not available in this environment.
// - uv_os_setenv() and uv_os_unsetenv():
//   Environment variables are not available.
// - uv_recv_buffer_size(), uv_send_buffer_size(), uv_tcp_keepalive() and
//   uv_tcp_nodelay():
//   Socket attributes cannot be adjusted.

#ifndef _UV_H_
#define _UV_H_

#include <_/limits.h>
#include <_/types.h>

#ifndef argdata_t
typedef struct argdata_t argdata_t;
#define argdata_t argdata_t
#endif
#ifndef _FILE_DECLARED
typedef struct _FILE FILE;
#define _FILE_DECLARED
#endif
#ifndef _INT64_T_DECLARED
typedef __int64_t int64_t;
#define _INT64_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif
#ifndef _UINT64_T_DECLARED
typedef __uint64_t uint64_t;
#define _UINT64_T_DECLARED
#endif

struct addrinfo;
struct sockaddr;
struct sockaddr_in;
struct sockaddr_in6;

typedef int uv_file;
typedef int uv_os_fd_t;
typedef int uv_os_sock_t;

typedef struct {
  char *base;
  size_t len;
} uv_buf_t;

// clang-format off

#define _UV_ENUM_ENTRY(upper, lower) UV_##upper,
#define _UV_STRUCT_TYPE(upper, lower) \
  typedef struct uv_##lower##_s uv_##lower##_t;

#define UV_HANDLE_TYPE_MAP(func) \
  func(ASYNC, async)             \
  func(CHECK, check)             \
  func(HANDLE, handle)           \
  func(IDLE, idle)               \
  func(NAMED_PIPE, pipe)         \
  func(POLL, poll)               \
  func(PREPARE, prepare)         \
  func(PROCESS, process)         \
  func(STREAM, stream)           \
  func(TCP, tcp)                 \
  func(TIMER, timer)             \
  func(TTY, tty)                 \
  func(UDP, udp)

typedef enum {
  UV_UNKNOWN_HANDLE = 0,
  UV_HANDLE_TYPE_MAP(_UV_ENUM_ENTRY)
  UV_FILE,
  UV_HANDLE_TYPE_MAX,
} uv_handle_type;

UV_HANDLE_TYPE_MAP(_UV_STRUCT_TYPE)

#define UV_REQ_TYPE_MAP(func)    \
  func(FS, fs)                   \
  func(GETADDRINFO, getaddrinfo) \
  func(GETNAMEINFO, getnameinfo) \
  func(REQ, req)                 \
  func(SHUTDOWN, shutdown)       \
  func(WORK, work)               \
  func(WRITE, write)

typedef enum {
  UV_UNKNOWN_REQ = 0,
  UV_REQ_TYPE_MAP(_UV_ENUM_ENTRY)
  UV_REQ_TYPE_MAX,
} uv_req_type;

UV_REQ_TYPE_MAP(_UV_STRUCT_TYPE)

#undef _UV_ENUM_ENTRY
#undef _UV_TYPE_ENTRY

//
// Error handling.
//

#define UV_ERRNO_MAP(func)                                         \
  func(E2BIG, "argument list too long")                            \
  func(EACCES, "permission denied")                                \
  func(EADDRINUSE, "address already in use")                       \
  func(EADDRNOTAVAIL, "address not available")                     \
  func(EAFNOSUPPORT, "address family not supported")               \
  func(EAGAIN, "resource temporarily unavailable")                 \
  func(EAI_ADDRFAMILY, "address family not supported")             \
  func(EAI_AGAIN, "temporary failure")                             \
  func(EAI_BADFLAGS, "bad ai_flags value")                         \
  func(EAI_BADHINTS, "invalid value for hints")                    \
  func(EAI_CANCELED, "request canceled")                           \
  func(EAI_FAIL, "permanent failure")                              \
  func(EAI_FAMILY, "ai_family not supported")                      \
  func(EAI_MEMORY, "out of memory")                                \
  func(EAI_NODATA, "no address")                                   \
  func(EAI_NONAME, "unknown node or service")                      \
  func(EAI_OVERFLOW, "argument buffer overflow")                   \
  func(EAI_PROTOCOL, "resolved protocol is unknown")               \
  func(EAI_SERVICE, "service not available for socket type")       \
  func(EAI_SOCKTYPE, "socket type not supported")                  \
  func(EALREADY, "connection already in progress")                 \
  func(EBADF, "bad file descriptor")                               \
  func(EBUSY, "resource busy or locked")                           \
  func(ECANCELED, "operation canceled")                            \
  func(ECHARSET, "invalid Unicode character")                      \
  func(ECONNABORTED, "software caused connection abort")           \
  func(ECONNREFUSED, "connection refused")                         \
  func(ECONNRESET, "connection reset by peer")                     \
  func(EDESTADDRREQ, "destination address required")               \
  func(EEXIST, "file already exists")                              \
  func(EFAULT, "bad address in system call argument")              \
  func(EFBIG, "file too large")                                    \
  func(EHOSTDOWN, "host is down")                                  \
  func(EHOSTUNREACH, "host is unreachable")                        \
  func(EINTR, "interrupted system call")                           \
  func(EINVAL, "invalid argument")                                 \
  func(EIO, "i/o error")                                           \
  func(EISCONN, "socket is already connected")                     \
  func(EISDIR, "illegal operation on a directory")                 \
  func(ELOOP, "too many symbolic links encountered")               \
  func(EMFILE, "too many open files")                              \
  func(EMLINK, "too many links")                                   \
  func(EMSGSIZE, "message too long")                               \
  func(ENAMETOOLONG, "name too long")                              \
  func(ENETDOWN, "network is down")                                \
  func(ENETUNREACH, "network is unreachable")                      \
  func(ENFILE, "file table overflow")                              \
  func(ENOBUFS, "no buffer space available")                       \
  func(ENODEV, "no such device")                                   \
  func(ENOENT, "no such file or directory")                        \
  func(ENOMEM, "not enough memory")                                \
  func(ENONET, "machine is not on the network")                    \
  func(ENOPROTOOPT, "protocol not available")                      \
  func(ENOSPC, "no space left on device")                          \
  func(ENOSYS, "function not implemented")                         \
  func(ENOTCONN, "socket is not connected")                        \
  func(ENOTDIR, "not a directory")                                 \
  func(ENOTEMPTY, "directory not empty")                           \
  func(ENOTSOCK, "socket operation on non-socket")                 \
  func(ENOTSUP, "operation not supported on socket")               \
  func(ENOTTY, "inappropriate ioctl for device") \
  func(ENXIO, "no such device or address")                         \
  func(EOF, "end of file")                                         \
  func(EPERM, "operation not permitted")                           \
  func(EPIPE, "broken pipe")                                       \
  func(EPROTO, "protocol error")                                   \
  func(EPROTONOSUPPORT, "protocol not supported")                  \
  func(EPROTOTYPE, "protocol wrong type for socket")               \
  func(ERANGE, "result too large")                                 \
  func(EREMOTEIO, "remote I/O error")                              \
  func(EROFS, "read-only file system")                             \
  func(ESHUTDOWN, "cannot send after transport endpoint shutdown") \
  func(ESPIPE, "invalid seek")                                     \
  func(ESRCH, "no such process")                                   \
  func(ETIMEDOUT, "connection timed out")                          \
  func(ETXTBSY, "text file is busy")                               \
  func(EXDEV, "cross-device link not permitted")                   \
  func(UNKNOWN, "unknown error")

// clang-format on

typedef enum {
  // C/POSIX error codes. Map these to [-1000, 0).
  UV_E2BIG = -1,
  UV_EACCES = -2,
  UV_EADDRINUSE = -3,
  UV_EADDRNOTAVAIL = -4,
  UV_EAFNOSUPPORT = -5,
  UV_EAGAIN = -6,
  UV_EALREADY = -7,
  UV_EBADF = -8,
  UV_EBUSY = -10,
  UV_ECANCELED = -11,
  UV_ECONNABORTED = -13,
  UV_ECONNREFUSED = -14,
  UV_ECONNRESET = -15,
  UV_EDESTADDRREQ = -17,
  UV_EEXIST = -20,
  UV_EFAULT = -21,
  UV_EFBIG = -22,
  UV_EHOSTUNREACH = -23,
  UV_EINTR = -27,
  UV_EINVAL = -28,
  UV_EIO = -29,
  UV_EISCONN = -30,
  UV_EISDIR = -31,
  UV_ELOOP = -32,
  UV_EMFILE = -33,
  UV_EMLINK = -34,
  UV_EMSGSIZE = -35,
  UV_ENAMETOOLONG = -37,
  UV_ENETDOWN = -38,
  UV_ENETUNREACH = -40,
  UV_ENFILE = -41,
  UV_ENOBUFS = -42,
  UV_ENODEV = -43,
  UV_ENOENT = -44,
  UV_ENOMEM = -48,
  UV_ENOPROTOOPT = -50,
  UV_ENOSPC = -51,
  UV_ENOSYS = -52,
  UV_ENOTCONN = -53,
  UV_ENOTDIR = -54,
  UV_ENOTEMPTY = -55,
  UV_ENOTSOCK = -57,
  UV_ENOTSUP = -58,
  UV_ENOTTY = -59,
  UV_ENXIO = -60,
  UV_EPERM = -63,
  UV_EPIPE = -64,
  UV_EPROTO = -65,
  UV_EPROTONOSUPPORT = -66,
  UV_EPROTOTYPE = -67,
  UV_ERANGE = -68,
  UV_EROFS = -69,
  UV_ESPIPE = -70,
  UV_ESRCH = -71,
  UV_ETIMEDOUT = -73,
  UV_ETXTBSY = -74,
  UV_EXDEV = -75,

  // getaddrinfo() error codes. Map these to [-2000,-1000).
  UV_EAI_AGAIN = -1001,
  UV_EAI_BADFLAGS = -1002,
  UV_EAI_FAIL = -1003,
  UV_EAI_FAMILY = -1004,
  UV_EAI_MEMORY = -1005,
  UV_EAI_NONAME = -1006,
  UV_EAI_OVERFLOW = -1010,
  UV_EAI_SERVICE = -1007,
  UV_EAI_SOCKTYPE = -1008,

  // libuv specific error codes. Map these to [-3000,-2000).
  UV_EAI_ADDRFAMILY = -2001,
  UV_EAI_BADHINTS = -2002,
  UV_EAI_CANCELED = -2003,
  UV_EAI_NODATA = -2004,
  UV_EAI_PROTOCOL = -2005,
  UV_ECHARSET = -2006,
  UV_EHOSTDOWN = -2007,
  UV_ENONET = -2008,
  UV_EOF = -2009,
  UV_EREMOTEIO = -2010,
  UV_ESHUTDOWN = -2011,
  UV_UNKNOWN = -2012,
} uv_errno_t;

__BEGIN_DECLS
const char *uv_err_name(int);
const char *uv_strerror(int);
int uv_translate_sys_error(int);
__END_DECLS

//
// Common data structures.
//

#define _UV_SLIST_DECLARE_STRUCTURES(name, type) \
  struct name##_head {                           \
    type *__first;                               \
  };                                             \
                                                 \
  struct name##_entry {                          \
    type *__next;                                \
  }

struct __uv_queue_link {
  struct __uv_queue_link *__next;
  struct __uv_queue_link *__prev;
};

#define _UV_TAILQ_DECLARE_STRUCTURES(name) \
  struct name##_head {                     \
    struct __uv_queue_link __link;         \
  };                                       \
                                           \
  struct name##_entry {                    \
    struct __uv_queue_link __link;         \
  }

#define _UV_HEAP_DECLARE_STRUCTURES(name, type) \
  struct name##_head {                          \
    type **__elements;                          \
    size_t __length;                            \
    size_t __capacity;                          \
    uint64_t __next_sequence;                   \
  };                                            \
                                                \
  struct name##_entry {                         \
    size_t __index;                             \
    uint64_t __sequence;                        \
  }

#define _UV_CIRCLEBUF_DECLARE_STRUCTURES(name, type) \
  struct name##_head {                               \
    type *__elements;                                \
    size_t __offset;                                 \
    size_t __length;                                 \
    size_t __capacity;                               \
  };

_UV_CIRCLEBUF_DECLARE_STRUCTURES(__uv_pending_fds, int);
_UV_HEAP_DECLARE_STRUCTURES(__uv_active_timers, uv_timer_t);
_UV_SLIST_DECLARE_STRUCTURES(__uv_closing_handles, uv_handle_t);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_active_asyncs);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_active_checks);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_active_idles);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_active_prepares);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_active_processes);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_handles);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_reading_polls);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_reading_streams);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_shutdowns);  // TODO(ed): Use STAILQ?
_UV_TAILQ_DECLARE_STRUCTURES(__uv_writes);     // TODO(ed): Use STAILQ?
_UV_TAILQ_DECLARE_STRUCTURES(__uv_writing_polls);
_UV_TAILQ_DECLARE_STRUCTURES(__uv_writing_streams);

//
// Version-checking macros and functions.
//

#define UV_VERSION_MAJOR 1
#define UV_VERSION_MINOR 18
#define UV_VERSION_PATCH 0
#define UV_VERSION_IS_RELEASE 0
#define UV_VERSION_SUFFIX "cloudlibc"

#define UV_VERSION_HEX \
  (UV_VERSION_MAJOR << 16 | UV_VERSION_MINOR << 8 | UV_VERSION_PATCH)

__BEGIN_DECLS
unsigned int uv_version(void);
const char *uv_version_string(void);
__END_DECLS

//
// uv_loop_t - Event loop.
//

typedef struct uv_loop_s {
  void *data;

  uint64_t __now;
  _Bool __stop;

  struct __uv_handles_head __handles;

  struct __uv_active_asyncs_head __active_asyncs;
  struct __uv_active_checks_head __active_checks;
  struct __uv_active_idles_head __active_idles;
  struct __uv_active_prepares_head __active_prepares;
  struct __uv_active_processes_head __active_processes;
  struct __uv_active_timers_head __active_timers;
  struct __uv_closing_handles_head __closing_handles;
  struct __uv_reading_polls_head __reading_polls;
  struct __uv_reading_streams_head __reading_streams;
  struct __uv_writing_polls_head __writing_polls;
  struct __uv_writing_streams_head __writing_streams;

  size_t __active_ref_handles_reqs;

  void *__events_buffer;
  size_t __events_capacity;
  void *__subscriptions_buffer;
  size_t __subscriptions_capacity;
} uv_loop_t;

typedef enum {
  UV_RUN_DEFAULT = 0,
  UV_RUN_ONCE,
  UV_RUN_NOWAIT,
} uv_run_mode;

typedef void (*uv_walk_cb)(uv_handle_t *, void *);

__BEGIN_DECLS
int uv_backend_timeout(const uv_loop_t *);
int uv_loop_alive(const uv_loop_t *);
int uv_loop_close(uv_loop_t *);
int uv_loop_init(uv_loop_t *);
size_t uv_loop_size(void);
uint64_t uv_now(const uv_loop_t *);
int uv_run(uv_loop_t *, uv_run_mode);
void uv_stop(uv_loop_t *);
void uv_update_time(uv_loop_t *);
void uv_walk(uv_loop_t *, uv_walk_cb, void *);
__END_DECLS

//
// uv_handle_t - Base handle.

typedef void (*uv_alloc_cb)(uv_handle_t *, size_t, uv_buf_t *);
typedef void (*uv_close_cb)(uv_handle_t *);

struct uv_handle_s {
#define _UV_HANDLE_FIELDS                                       \
  _Alignas(__max_align_t) uv_loop_t *loop;                      \
  uv_handle_type type;                                          \
  void *data;                                                   \
                                                                \
  unsigned int __state;                                         \
  struct __uv_handles_entry __uv_handles_entry;                 \
  struct __uv_closing_handles_entry __uv_closing_handles_entry; \
  uv_close_cb __close_cb;

  _UV_HANDLE_FIELDS
};

__BEGIN_DECLS
void uv_close(uv_handle_t *, uv_close_cb);
int uv_fileno(const uv_handle_t *, uv_os_fd_t *);
size_t uv_handle_size(uv_handle_type);
int uv_has_ref(const uv_handle_t *);
int uv_is_active(const uv_handle_t *);
int uv_is_closing(const uv_handle_t *);
void uv_ref(uv_handle_t *);
void uv_unref(uv_handle_t *);
__END_DECLS

//
// uv_req_t - Base request.
//

struct uv_req_s {
#define _UV_REQ_FIELDS                \
  _Alignas(__max_align_t) void *data; \
  uv_req_type type;

  _UV_REQ_FIELDS
};

__BEGIN_DECLS
int uv_cancel(uv_req_t *);
size_t uv_req_size(uv_req_type);
__END_DECLS

//
// uv_timer_t - Timer handle.
//

typedef void (*uv_timer_cb)(uv_timer_t *);

struct uv_timer_s {
  _UV_HANDLE_FIELDS

  uv_timer_cb __cb;
  uint64_t __timeout;
  uint64_t __repeat;
  struct __uv_active_timers_entry __uv_active_timers_entry;
};

__BEGIN_DECLS
int uv_timer_again(uv_timer_t *);
uint64_t uv_timer_get_repeat(const uv_timer_t *);
int uv_timer_init(uv_loop_t *, uv_timer_t *);
void uv_timer_set_repeat(uv_timer_t *, uint64_t);
int uv_timer_start(uv_timer_t *, uv_timer_cb, uint64_t, uint64_t);
int uv_timer_stop(uv_timer_t *);
__END_DECLS

//
// uv_prepare_t - Prepare handle.
//

typedef void (*uv_prepare_cb)(uv_prepare_t *);

struct uv_prepare_s {
  _UV_HANDLE_FIELDS

  uv_prepare_cb __cb;
  struct __uv_active_prepares_entry __uv_active_prepares_entry;
};

__BEGIN_DECLS
int uv_prepare_init(uv_loop_t *, uv_prepare_t *);
int uv_prepare_start(uv_prepare_t *, uv_prepare_cb);
int uv_prepare_stop(uv_prepare_t *);
__END_DECLS

//
// uv_check_t - Check handle.
//

typedef void (*uv_check_cb)(uv_check_t *);

struct uv_check_s {
  _UV_HANDLE_FIELDS

  uv_check_cb __cb;
  struct __uv_active_checks_entry __uv_active_checks_entry;
};

__BEGIN_DECLS
int uv_check_init(uv_loop_t *, uv_check_t *);
int uv_check_start(uv_check_t *, uv_check_cb);
int uv_check_stop(uv_check_t *);
__END_DECLS

//
// uv_idle_t - Idle handle.
//

typedef void (*uv_idle_cb)(uv_idle_t *);

struct uv_idle_s {
  _UV_HANDLE_FIELDS

  uv_idle_cb __cb;
  struct __uv_active_idles_entry __uv_active_idles_entry;
};

__BEGIN_DECLS
int uv_idle_init(uv_loop_t *, uv_idle_t *);
int uv_idle_start(uv_idle_t *, uv_idle_cb);
int uv_idle_stop(uv_idle_t *);
__END_DECLS

//
// uv_async_t - Async handle.
//

typedef void (*uv_async_cb)(uv_async_t *);

struct uv_async_s {
  _UV_HANDLE_FIELDS

  __uint32_t __readfd;
  __uint32_t __writefd;
  uv_async_cb __cb;
  struct __uv_active_asyncs_entry __uv_active_asyncs_entry;
};

__BEGIN_DECLS
int uv_async_init(uv_loop_t *, uv_async_t *, uv_async_cb);
int uv_async_send(uv_async_t *);
__END_DECLS

//
// uv_poll_t - Poll handle.
//

typedef void (*uv_poll_cb)(uv_poll_t *, int, int);

struct uv_poll_s {
  _UV_HANDLE_FIELDS

  int __fd;
  uv_poll_cb __cb;
  int __events;
  int __status;
  int __revents;
  struct __uv_reading_polls_entry __uv_reading_polls_entry;
  struct __uv_writing_polls_entry __uv_writing_polls_entry;
};

enum uv_poll_event {
  UV_READABLE = 0x1,
  UV_WRITABLE = 0x2,
  UV_DISCONNECT = 0x4,
};

__BEGIN_DECLS
int uv_poll_init(uv_loop_t *, uv_poll_t *, int);
int uv_poll_init_socket(uv_loop_t *, uv_poll_t *, uv_os_sock_t);
int uv_poll_start(uv_poll_t *, int, uv_poll_cb);
int uv_poll_stop(uv_poll_t *);
__END_DECLS

//
// uv_process_t - Process handle.
//

typedef void (*uv_exit_cb)(uv_process_t *, int64_t, int);

struct uv_process_s {
  _UV_HANDLE_FIELDS

  uv_exit_cb __cb;
  int __fd;
  struct __uv_active_processes_entry __uv_active_processes_entry;
};

__BEGIN_DECLS
void uv_disable_stdio_inheritance(void);
int uv_process_kill(uv_process_t *, int);
__END_DECLS

//
// uv_stream_t - Stream handle.
//

typedef void (*uv_read_cb)(uv_stream_t *, ssize_t, const uv_buf_t *);
typedef void (*uv_shutdown_cb)(uv_shutdown_t *, int);
typedef void (*uv_write_cb)(uv_write_t *, int);

struct uv_stream_s {
#define _UV_STREAM_FIELDS                                       \
  _UV_HANDLE_FIELDS                                             \
                                                                \
  size_t write_queue_size;                                      \
                                                                \
  int __fd;                                                     \
  uv_alloc_cb __alloc_cb;                                       \
  uv_read_cb __read_cb;                                         \
  struct __uv_shutdowns_head __shutdown_queue;                  \
  struct __uv_writes_head __write_queue;                        \
  struct __uv_pending_fds_head __pending_fds;                   \
  struct __uv_reading_streams_entry __uv_reading_streams_entry; \
  struct __uv_writing_streams_entry __uv_writing_streams_entry;
  _UV_STREAM_FIELDS
};

struct uv_shutdown_s {
  _UV_REQ_FIELDS

  uv_stream_t *handle;

  uv_shutdown_cb __cb;
  struct __uv_shutdowns_entry __uv_shutdowns_entry;
};

struct uv_write_s {
  _UV_REQ_FIELDS

  uv_stream_t *handle;
  uv_stream_t *send_handle;

  uv_buf_t *__bufs;
  unsigned int __nbufs_done;
  unsigned int __nbufs_total;
  uv_write_cb __cb;
  struct __uv_writes_entry __uv_writes_entry;
};

__BEGIN_DECLS
int uv_accept(uv_stream_t *, uv_stream_t *);
int uv_is_readable(const uv_stream_t *);
int uv_is_writable(const uv_stream_t *);
int uv_read_start(uv_stream_t *, uv_alloc_cb, uv_read_cb);
int uv_read_stop(uv_stream_t *);
int uv_shutdown(uv_shutdown_t *, uv_stream_t *, uv_shutdown_cb);
int uv_stream_set_blocking(uv_stream_t *, int);
int uv_try_write(uv_stream_t *, const uv_buf_t *, unsigned int);
int uv_write(uv_write_t *, uv_stream_t *, const uv_buf_t *, unsigned int,
             uv_write_cb);
int uv_write2(uv_write_t *, uv_stream_t *, const uv_buf_t *, unsigned int,
              uv_stream_t *, uv_write_cb);
__END_DECLS

//
// uv_tcp_t - TCP handle.
//

struct uv_tcp_s {
  _UV_STREAM_FIELDS
};

__BEGIN_DECLS
int uv_tcp_init(uv_loop_t *, uv_tcp_t *);
int uv_tcp_init_ex(uv_loop_t *, uv_tcp_t *, unsigned int);
int uv_tcp_open(uv_tcp_t *, uv_os_sock_t);
__END_DECLS

//
// uv_pipe_t - Pipe handle.
//

struct uv_pipe_s {
  _UV_STREAM_FIELDS

  int ipc;
};

__BEGIN_DECLS
int uv_pipe_init(uv_loop_t *, uv_pipe_t *, int);
int uv_pipe_open(uv_pipe_t *, uv_file);
int uv_pipe_pending_count(uv_pipe_t *);
uv_handle_type uv_pipe_pending_type(uv_pipe_t *);
__END_DECLS

//
// uv_tty_t - TTY handle.
//

struct uv_tty_s {
  _UV_STREAM_FIELDS
};

__BEGIN_DECLS
int uv_tty_init(uv_loop_t *, uv_tty_t *, uv_file, int);
int uv_tty_reset_mode(void);
__END_DECLS

//
// uv_udp_t - UDP handle.
//

typedef void (*uv_udp_recv_cb)(uv_udp_t *, ssize_t, const uv_buf_t *,
                               const struct sockaddr *, unsigned int);

struct uv_udp_s {
  _UV_HANDLE_FIELDS
};

enum uv_udp_flags {
  UV_UDP_PARTIAL = 0x1,
};

__BEGIN_DECLS
int uv_udp_init(uv_loop_t *, uv_udp_t *);
int uv_udp_init_ex(uv_loop_t *, uv_udp_t *, unsigned int);
int uv_udp_open(uv_udp_t *, uv_os_sock_t);
int uv_udp_recv_start(uv_udp_t *, uv_alloc_cb, uv_udp_recv_cb);
int uv_udp_recv_stop(uv_udp_t *);
__END_DECLS

//
// File system operations.
//

typedef struct {
  long tv_sec;
  long tv_nsec;
} uv_timespec_t;

typedef struct {
  uint64_t st_dev;
  uint64_t st_mode;
  uint64_t st_nlink;
  uint64_t st_ino;
  uint64_t st_size;
  uv_timespec_t st_atim;
  uv_timespec_t st_mtim;
  uv_timespec_t st_ctim;
} uv_stat_t;

typedef enum {
  UV_FS_UNKNOWN = -1,
  UV_FS_CUSTOM,
  UV_FS_CLOSE,
  UV_FS_FDATASYNC,
  UV_FS_FSTAT,
  UV_FS_FSYNC,
  UV_FS_FTRUNCATE,
  UV_FS_FUTIME,
  UV_FS_READ,
  UV_FS_SENDFILE,
  UV_FS_WRITE,
} uv_fs_type;

struct uv_fs_s {
#define _UV_WORK_FIELDS \
  _UV_REQ_FIELDS        \
  uv_loop_t *loop;

  _UV_WORK_FIELDS

  uv_fs_type fs_type;
  ssize_t result;
  uv_stat_t statbuf;

  union {
    struct {
      uv_file __file;
    } __close;
    struct {
      uv_file __file;
    } __fdatasync;
    struct {
      uv_file __file;
    } __fstat;
    struct {
      uv_file __file;
    } __fsync;
    struct {
      uv_file __file;
      int64_t __offset;
    } __ftruncate;
    struct {
      uv_file __file;
      double __atime;
      double __mtime;
    } __futime;
  } __arguments;
};

typedef void (*uv_fs_cb)(uv_fs_t *);

__BEGIN_DECLS
int uv_fs_close(uv_loop_t *, uv_fs_t *, uv_file, uv_fs_cb);
int uv_fs_fdatasync(uv_loop_t *, uv_fs_t *, uv_file, uv_fs_cb);
int uv_fs_fstat(uv_loop_t *, uv_fs_t *, uv_file, uv_fs_cb);
int uv_fs_fsync(uv_loop_t *, uv_fs_t *, uv_file, uv_fs_cb);
int uv_fs_ftruncate(uv_loop_t *, uv_fs_t *, uv_file, int64_t, uv_fs_cb);
int uv_fs_futime(uv_loop_t *, uv_fs_t *, uv_file, double, double, uv_fs_cb);
int uv_fs_read(uv_loop_t *, uv_fs_t *, uv_file, const uv_buf_t *, unsigned int,
               int64_t, uv_fs_cb);
void uv_fs_req_cleanup(uv_fs_t *);
int uv_fs_sendfile(uv_loop_t *, uv_fs_t *, uv_file, uv_file, int64_t, size_t,
                   uv_fs_cb);
int uv_fs_write(uv_loop_t *, uv_fs_t *, uv_file, const uv_buf_t *, unsigned int,
                int64_t, uv_fs_cb);
uv_os_fd_t uv_get_osfhandle(int);
__END_DECLS

//
// Thread pool work scheduling.
//

struct uv_work_s {
  _UV_WORK_FIELDS
};

typedef void (*uv_work_cb)(uv_work_t *);
typedef void (*uv_after_work_cb)(uv_work_t *, int);

__BEGIN_DECLS
int uv_queue_work(uv_loop_t *, uv_work_t *, uv_work_cb, uv_after_work_cb);
__END_DECLS

//
// DNS utility functions.
//

struct uv_getaddrinfo_s {
  _UV_WORK_FIELDS

  struct addrinfo *addrinfo;
};

struct uv_getnameinfo_s {
  _UV_WORK_FIELDS

  char host[57];
  char service[64];
};

typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t *, int, struct addrinfo *);
typedef void (*uv_getnameinfo_cb)(uv_getnameinfo_t *, int, const char *,
                                  const char *);

__BEGIN_DECLS
void uv_freeaddrinfo(struct addrinfo *);
int uv_getaddrinfo(uv_loop_t *, uv_getaddrinfo_t *, uv_getaddrinfo_cb,
                   const char *, const char *, const struct addrinfo *);
int uv_getnameinfo(uv_loop_t *, uv_getnameinfo_t *, uv_getnameinfo_cb,
                   const struct sockaddr *, int);
__END_DECLS

//
// Threading and synchronization utilities.
//

typedef __pthread_barrier_t uv_barrier_t;
typedef __pthread_cond_t uv_cond_t;
typedef __pthread_key_t uv_key_t;
typedef __pthread_lock_t uv_mutex_t;
typedef __pthread_once_t uv_once_t;
typedef __pthread_lock_t uv_rwlock_t;
typedef __sem_t uv_sem_t;
typedef __pthread_t uv_thread_t;

typedef void (*uv_thread_cb)(void *);

#define UV_ONCE_INIT \
  { _UINT32_C(0x80000000) }

__BEGIN_DECLS
void uv_barrier_destroy(uv_barrier_t *);
int uv_barrier_init(uv_barrier_t *barrier, unsigned int);
int uv_barrier_wait(uv_barrier_t *);
void uv_cond_broadcast(uv_cond_t *);
void uv_cond_destroy(uv_cond_t *);
int uv_cond_init(uv_cond_t *);
void uv_cond_signal(uv_cond_t *);
int uv_cond_timedwait(uv_cond_t *, uv_mutex_t *__mutex, uint64_t)
    __requires_exclusive(*__mutex);
void uv_cond_wait(uv_cond_t *, uv_mutex_t *__mutex)
    __requires_exclusive(*__mutex);
int uv_key_create(uv_key_t *);
void uv_key_delete(uv_key_t *);
void *uv_key_get(uv_key_t *);
void uv_key_set(uv_key_t *, void *);
void uv_mutex_destroy(uv_mutex_t *__handle) __requires_unlocked(*__handle);
int uv_mutex_init(uv_mutex_t *__handle) __requires_unlocked(*__handle);
int uv_mutex_init_recursive(uv_mutex_t *__handle)
    __requires_unlocked(*__handle);
void uv_mutex_lock(uv_mutex_t *__handle) __locks_exclusive(*__handle);
int uv_mutex_trylock(uv_mutex_t *__handle) __trylocks_exclusive(0, *__handle);
void uv_mutex_unlock(uv_mutex_t *__handle) __unlocks(*__handle);
void uv_once(uv_once_t *, void (*)(void));
void uv_rwlock_destroy(uv_rwlock_t *__rwlock) __requires_unlocked(*__rwlock);
int uv_rwlock_init(uv_rwlock_t *__rwlock) __requires_unlocked(*__rwlock);
void uv_rwlock_rdlock(uv_rwlock_t *__rwlock) __locks_shared(*__rwlock);
void uv_rwlock_rdunlock(uv_rwlock_t *__rwlock) __unlocks(*__rwlock);
int uv_rwlock_tryrdlock(uv_rwlock_t *__rwlock) __trylocks_shared(0, *__rwlock);
int uv_rwlock_trywrlock(uv_rwlock_t *__rwlock)
    __trylocks_exclusive(0, *__rwlock);
void uv_rwlock_wrlock(uv_rwlock_t *__rwlock) __locks_exclusive(*__rwlock);
void uv_rwlock_wrunlock(uv_rwlock_t *__rwlock) __unlocks(*__rwlock);
void uv_sem_destroy(uv_sem_t *);
int uv_sem_init(uv_sem_t *, unsigned int);
void uv_sem_post(uv_sem_t *);
int uv_sem_trywait(uv_sem_t *);
void uv_sem_wait(uv_sem_t *);
int uv_thread_create(uv_thread_t *, uv_thread_cb, void *);
int uv_thread_equal(const uv_thread_t *, const uv_thread_t *);
int uv_thread_join(uv_thread_t *);
uv_thread_t uv_thread_self(void);
__END_DECLS

//
// Miscellaneous utilities.
//

typedef struct {
  long tv_sec;
  long tv_usec;
} uv_timeval_t;

typedef struct {
  uv_timeval_t ru_utime;
  uv_timeval_t ru_stime;
} uv_rusage_t;

union uv_any_handle {
  uv_async_t async;
  uv_check_t check;
  uv_handle_t handle;
  uv_idle_t idle;
  uv_pipe_t pipe;
  uv_poll_t poll;
  uv_prepare_t prepare;
  uv_process_t process;
  uv_stream_t stream;
  uv_tcp_t tcp;
  uv_timer_t timer;
};

#define UV_IF_NAMESIZE 33

__BEGIN_DECLS
uv_buf_t uv_buf_init(char *, size_t);
int uv_getrusage(uv_rusage_t *);
uv_handle_type uv_guess_handle(uv_file);
uint64_t uv_hrtime(void);
int uv_if_indextoiid(unsigned int, char *, size_t *);
int uv_if_indextoname(unsigned int, char *, size_t *);
int uv_inet_ntop(int, const void *, char *, size_t);
int uv_inet_pton(int, const char *, void *);
int uv_ip4_addr(const char *, int, struct sockaddr_in *);
int uv_ip6_addr(const char *, int, struct sockaddr_in6 *);
int uv_ip4_name(const struct sockaddr_in *, char *, size_t);
int uv_ip6_name(const struct sockaddr_in6 *, char *, size_t);
int uv_os_getenv(const char *, char *, size_t *);
void uv_print_active_handles(uv_loop_t *, FILE *);
void uv_print_all_handles(uv_loop_t *, FILE *);
__END_DECLS

#undef _UV_HANDLE_FIELDS
#undef _UV_REQ_FIELDS
#undef _UV_STREAM_FIELDS
#undef _UV_WORK_FIELDS

#endif
