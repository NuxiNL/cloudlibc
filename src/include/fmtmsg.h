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

// <fmtmsg.h> - message display structures

#ifndef _FMTMSG_H_
#define _FMTMSG_H_

#include <_/cdefs.h>

// Major Classifications.
#define MM_HARD 0L  // Source of the condition is hardware.
#define MM_SOFT 0L  // Source of the condition is software.
#define MM_FIRM 0L  // Source of the condition is firmware.

// Message Source Subclassifications.
#define MM_APPL 0L   // Condition detected by application.
#define MM_UTIL 0L   // Condition detected by utility.
#define MM_OPSYS 0L  // Condition detected by operating system.

// Display Subclassifications.
#define MM_PRINT 0L    // Display message on standard error.
#define MM_CONSOLE 0L  // Display message on system console.

// Status Subclassifications.
#define MM_RECOVER 0L  // Recoverable error.
#define MM_NRECOV 0L   // Non-recoverable error.

// Severities.
#define MM_NOSEV 0    // No severity level provided for the message.
#define MM_HALT 1     // Error causing application to halt.
#define MM_ERROR 2    // Application has encountered a non-fatal fault.
#define MM_WARNING 3  // Application has detected unusual non-error condition.
#define MM_INFO 4     // Informative message.

// Null values.
#define MM_NULLMC 0L            // Null classification.
#define MM_NULLLBL ((char *)0)  // Null label.
#define MM_NULLSEV 0            // Null severity.
#define MM_NULLTXT ((char *)0)  // Null text.
#define MM_NULLACT ((char *)0)  // Null action.
#define MM_NULLTAG ((char *)0)  // Null tag.

// Return values.
#define MM_OK 1     // The function succeeded.
#define MM_NOTOK 2  // The function failed completely.
#define MM_NOMSG 3  // Unable to generate a message on standard error.
#define MM_NOCON 4  // Unable to generate a console message.

__BEGIN_DECLS
int fmtmsg(long, const char *, int, const char *, const char *, const char *);
__END_DECLS

#endif
