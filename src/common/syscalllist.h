// Copyright (c) 2015 Nuxi, https://nuxi.nl/
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

SYSCALL_1_1(0, clock_res_get, cloudabi_clockid_t, cloudabi_timestamp_t);
SYSCALL_2_1(1, clock_time_get, cloudabi_clockid_t, cloudabi_timestamp_t,
            cloudabi_timestamp_t);
SYSCALL_3_0(2, condvar_signal, _Atomic(cloudabi_condvar_t) *,
            cloudabi_futexscope_t, cloudabi_nthreads_t);
SYSCALL_1_0(3, fd_close, cloudabi_fd_t);
SYSCALL_1_1(4, fd_create1, cloudabi_filetype_t, cloudabi_fd_t);
SYSCALL_1_2(5, fd_create2, cloudabi_filetype_t, cloudabi_fd_t, cloudabi_fd_t);
SYSCALL_1_0(6, fd_datasync, cloudabi_fd_t);
SYSCALL_1_1(7, fd_dup, cloudabi_fd_t, cloudabi_fd_t);
SYSCALL_4_1(8, fd_pread, cloudabi_fd_t, const cloudabi_iovec_t *,
            cloudabi_size_t, cloudabi_filesize_t, cloudabi_size_t);
SYSCALL_4_1(9, fd_pwrite, cloudabi_fd_t, const cloudabi_ciovec_t *,
            cloudabi_size_t, cloudabi_filesize_t, cloudabi_size_t);
SYSCALL_3_1(10, fd_read, cloudabi_fd_t, const cloudabi_iovec_t *,
            cloudabi_size_t, cloudabi_size_t);
SYSCALL_2_0(11, fd_replace, cloudabi_fd_t, cloudabi_fd_t);
SYSCALL_3_1(12, fd_seek, cloudabi_fd_t, cloudabi_filedelta_t, cloudabi_whence_t,
            cloudabi_filesize_t);
SYSCALL_2_0(13, fd_stat_get, cloudabi_fd_t, cloudabi_fdstat_t *);
SYSCALL_3_0(14, fd_stat_put, cloudabi_fd_t, const cloudabi_fdstat_t *,
            cloudabi_fdsflags_t);
SYSCALL_1_0(15, fd_sync, cloudabi_fd_t);
SYSCALL_3_1(16, fd_write, cloudabi_fd_t, const cloudabi_ciovec_t *,
            cloudabi_size_t, cloudabi_size_t);
SYSCALL_4_0(17, file_advise, cloudabi_fd_t, cloudabi_filesize_t,
            cloudabi_filesize_t, cloudabi_advice_t);
SYSCALL_3_0(18, file_allocate, cloudabi_fd_t, cloudabi_filesize_t,
            cloudabi_filesize_t);
SYSCALL_4_0(19, file_create, cloudabi_fd_t, const char *, cloudabi_size_t,
            cloudabi_filetype_t);
SYSCALL_6_0(20, file_link, cloudabi_lookup_t, const char *, cloudabi_size_t,
            cloudabi_fd_t, const char *, cloudabi_size_t);
SYSCALL_5_1(21, file_open, cloudabi_lookup_t, const char *, cloudabi_size_t,
            cloudabi_oflags_t, const cloudabi_fdstat_t *, cloudabi_fd_t);
SYSCALL_4_1(22, file_readdir, cloudabi_fd_t, void *, cloudabi_size_t,
            cloudabi_dircookie_t, cloudabi_size_t);
SYSCALL_5_1(23, file_readlink, cloudabi_fd_t, const char *, cloudabi_size_t,
            char *, cloudabi_size_t, cloudabi_size_t);
SYSCALL_6_0(24, file_rename, cloudabi_fd_t, const char *, cloudabi_size_t,
            cloudabi_fd_t, const char *, cloudabi_size_t);
SYSCALL_2_0(25, file_stat_fget, cloudabi_fd_t, cloudabi_filestat_t *);
SYSCALL_3_0(26, file_stat_fput, cloudabi_fd_t, const cloudabi_filestat_t *,
            cloudabi_fsflags_t);
SYSCALL_4_0(27, file_stat_get, cloudabi_lookup_t, const char *, cloudabi_size_t,
            cloudabi_filestat_t *);
SYSCALL_5_0(28, file_stat_put, cloudabi_lookup_t, const char *, cloudabi_size_t,
            const cloudabi_filestat_t *, cloudabi_fsflags_t);
SYSCALL_5_0(29, file_symlink, const char *, cloudabi_size_t, cloudabi_fd_t,
            const char *, cloudabi_size_t);
SYSCALL_4_0(30, file_unlink, cloudabi_fd_t, const char *, cloudabi_size_t,
            cloudabi_ulflags_t);
SYSCALL_2_0(31, lock_unlock, _Atomic(cloudabi_lock_t) *, cloudabi_futexscope_t);
SYSCALL_3_0(32, mem_advise, void *, cloudabi_size_t, cloudabi_advice_t);
SYSCALL_2_0(33, mem_lock, const void *, cloudabi_size_t);
SYSCALL_6_1(34, mem_map, void *, cloudabi_size_t, cloudabi_mprot_t,
            cloudabi_mflags_t, cloudabi_fd_t, cloudabi_filesize_t, void *);
SYSCALL_3_0(35, mem_protect, void *, cloudabi_size_t, cloudabi_mprot_t);
SYSCALL_3_0(36, mem_sync, void *, cloudabi_size_t, cloudabi_msflags_t);
SYSCALL_2_0(37, mem_unlock, const void *, cloudabi_size_t);
SYSCALL_2_0(38, mem_unmap, void *, cloudabi_size_t);
SYSCALL_5_1(39, poll, cloudabi_fd_t, const cloudabi_subscription_t *,
            cloudabi_size_t, cloudabi_event_t *, cloudabi_size_t,
            cloudabi_size_t);
SYSCALL_5_0(40, proc_exec, cloudabi_fd_t, const void *, cloudabi_size_t,
            const cloudabi_fd_t *, cloudabi_size_t);
SYSCALL_1_N(41, proc_exit, cloudabi_exitcode_t);
SYSCALL_0_2(42, proc_fork, cloudabi_fd_t, cloudabi_tid_t);
SYSCALL_1_0(43, proc_raise, cloudabi_signal_t);
SYSCALL_2_0(44, random_get, void *, cloudabi_size_t);
SYSCALL_2_1(45, sock_accept, cloudabi_fd_t, cloudabi_sockstat_t *,
            cloudabi_fd_t);
SYSCALL_4_0(46, sock_bind, cloudabi_fd_t, cloudabi_fd_t, const char *,
            cloudabi_size_t);
SYSCALL_4_0(47, sock_connect, cloudabi_fd_t, cloudabi_fd_t, const char *,
            cloudabi_size_t);
SYSCALL_2_0(48, sock_listen, cloudabi_fd_t, cloudabi_backlog_t);
SYSCALL_3_0(49, sock_recv, cloudabi_fd_t, const cloudabi_recv_in_t *,
            cloudabi_recv_out_t *);
SYSCALL_3_0(50, sock_send, cloudabi_fd_t, const cloudabi_send_in_t *,
            cloudabi_send_out_t *);
SYSCALL_2_0(51, sock_shutdown, cloudabi_fd_t, cloudabi_sdflags_t);
SYSCALL_3_0(52, sock_stat_get, cloudabi_fd_t, cloudabi_sockstat_t *,
            cloudabi_ssflags_t);
SYSCALL_1_1(53, thread_create, cloudabi_threadattr_t *, cloudabi_tid_t);
SYSCALL_2_N(54, thread_exit, _Atomic(cloudabi_lock_t) *, cloudabi_futexscope_t);
SYSCALL_1_0(55, thread_tcb_set, void *);
SYSCALL_0_0(56, thread_yield);
