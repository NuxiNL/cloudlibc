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

// <arpa/nameser.h> - Domain Name System parameters

#ifndef _ARPA_NAMESER_H_
#define _ARPA_NAMESER_H_

#define NS_CMPRSFLGS 0xc0  // Compression pointer flags.
#define NS_DEFAULTPORT 53  // UDP and TCP port used by DNS.
#define NS_HFIXEDSZ 12     // Header fixed size.
#define NS_MAXCDNAME 255   // Maximum compressed domain name length.
#define NS_MAXLABEL 63     // Maximum label length.
#define NS_PACKETSZ 512    // UDP packet size.
#define NS_QFIXEDSZ 4      // Query fixed size.
#define NS_RRFIXEDSZ 10    // Resource record fixed size.

// DNS CLASSes.
typedef enum {
  ns_c_in = 1,
} ns_class;

// Opcodes.
typedef enum {
  ns_o_query = 0,
} ns_opcode;

// Response codes.
typedef enum {
  ns_r_noerror = 0,
  ns_r_formerr = 1,
  ns_r_servfail = 2,
  ns_r_nxdomain = 3,
  ns_r_notimpl = 4,
  ns_r_refused = 5,
} ns_rcode;

// Resource Record (RR) TYPEs.
typedef enum {
  ns_t_a = 1,
  ns_t_ns = 2,
  ns_t_cname = 5,
  ns_t_soa = 6,
  ns_t_ptr = 12,
  ns_t_mx = 15,
  ns_t_txt = 16,
  ns_t_aaaa = 28,
  ns_t_srv = 33,
  ns_t_naptr = 35,
  ns_t_any = 255,
} ns_type;

// Compatibility definitions.
#define C_IN ns_c_in
#define FORMERR ns_r_formerr
#define HFIXEDSZ NS_HFIXEDSZ
#define INDIR_MASK NS_CMPRSFLGS
#define MAXCDNAME NS_MAXCDNAME
#define MAXLABEL NS_MAXLABEL
#define NAMESERVER_PORT NS_DEFAULTPORT
#define NOERROR ns_r_noerror
#define NOTIMP ns_r_notimpl
#define NXDOMAIN ns_r_nxdomain
#define PACKETSZ NS_PACKETSZ
#define QFIXEDSZ NS_QFIXEDSZ
#define QUERY ns_o_query
#define REFUSED ns_r_refused
#define RRFIXEDSZ NS_RRFIXEDSZ
#define SERVFAIL ns_r_servfail
#define T_A ns_t_a
#define T_AAAA ns_t_aaaa
#define T_CNAME ns_t_cname
#define T_MX ns_t_mx
#define T_NS ns_t_ns
#define T_PTR ns_t_ptr
#define T_TXT ns_t_txt

#endif
