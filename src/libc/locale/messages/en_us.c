// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <errno.h>
#include <netdb.h>
#include <regex.h>
#include <signal.h>

const struct lc_messages __messages_en_us = {
    .yesexpr = "^[yY]",
    .noexpr = "^[nN]",

    .gai_strerror =
        {
                [0] = "Success",
                [EAI_AGAIN] = "Temporary failure in name resolution",
                [EAI_BADFLAGS] = "Invalid value for ai_flags",
                [EAI_FAIL] = "Non-recoverable failure in name resolution",
                [EAI_FAMILY] = "ai_family not supported",
                [EAI_MEMORY] = "Memory allocation failure",
                [EAI_NONAME] = "hostname nor servname provided, or not known",
                [EAI_SERVICE] = "servname not supported for ai_socktype",
                [EAI_SOCKTYPE] = "ai_socktype not supported",
                [EAI_SYSTEM] = "System error returned in errno",
                [EAI_OVERFLOW] = "Argument buffer overflow",
        },

    .regerror =
        {
                [0] = "Success", [REG_BADBR] = "Content of \"\\{\\}\" invalid",
                [REG_BADPAT] = "Invalid regular expression",
                [REG_BADRPT] =
                    "'?', '*', or '+' not preceded by valid regular expression",
                [REG_EBRACE] = "\"\\{\\}\" imbalance",
                [REG_EBRACK] = "\"[]\" imbalance",
                [REG_ECOLLATE] = "Invalid collating element referenced",
                [REG_ECTYPE] = "Invalid character class type referenced",
                [REG_EESCAPE] = "Trailing backslash character in pattern",
                [REG_EPAREN] = "\"\\(\\)\" or \"()\" imbalance",
                [REG_ERANGE] = "Invalid endpoint in range expression",
                [REG_ESPACE] = "Out of memory",
                [REG_ESUBREG] = "Number invalid or in error",
                [REG_NOMATCH] = "Failed to match",
        },

    .strerror =
        {
                [0] = "Success", [EACCES] = "Permission denied",
                [EAFNOSUPPORT] = "Address family not supported",
                [EAGAIN] = "Resource unavailable, try again",
                [EBADF] = "Bad file descriptor",
                [EBUSY] = "Device or resource busy",
                [EDEADLK] = "Resource deadlock would occur",
                [EEXIST] = "File exists", [EFAULT] = "Bad address",
                [EFBIG] = "File too large", [EINVAL] = "Invalid argument",
                [EIO] = "I/O error", [EISDIR] = "Is a directory",
                [ELOOP] = "Too many levels of symbolic links",
                [EMFILE] = "File descriptor value too large",
                [EMLINK] = "Too many links",
                [ENAMETOOLONG] = "Filename too long",
                [ENFILE] = "Too many files open in system",
                [ENOENT] = "No such file or directory",
                [ENOEXEC] = "Executable file format error",
                [ENOMEM] = "Not enough space",
                [ENOSPC] = "No space left on device",
                [ENOSYS] = "Function not implemented",
                [ENOTCAPABLE] = "Capabilities insufficient",
                [ENOTDIR] = "Not a directory or a symbolic link to a directory",
                [ENOTEMPTY] = "Directory not empty",
                [ENOTSOCK] = "Not a socket",
                [ENXIO] = "No such device or address",
                [EOVERFLOW] = "Value too large to be stored in data type",
                [EPERM] = "Operation not permitted", [EPIPE] = "Broken pipe",
                [EROFS] = "Read-only file system", [ESPIPE] = "Invalid seek",
                [EXDEV] = "Cross-device link",
        },

    .strsignal =
        {
                [0] = "Unknown signal", [SIGABRT] = "Process abort signal",
                [SIGBUS] = "Bus error", [SIGFPE] = "Floating point exception",
                [SIGSEGV] = "Segmentation fault",
        },

    .unknown_error = "Unknown error",
};
