// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
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

// <elf.h> - Executable and Linkable Format
//
// Values obtained from:
// http://www.sco.com/developers/gabi/latest/contents.html
// http://infocenter.arm.com/help/topic/com.arm.doc.ihi0056b/IHI0056B_aaelf64.pdf
// https://uclibc.org/docs/psABI-i386.pdf
// https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md
// http://www.x86-64.org/documentation/abi.pdf
//
// Extensions:
// - PT_ARM_EXIDX and PT_GNU_EH_FRAME:
//   Used for exception handling.
// - PT_GNU_RELRO:
//   Used for marking memory read-only after relocation.

#ifndef _ELF_H_
#define _ELF_H_

#include <_/types.h>

// Chapter 4 - Object Files: Introduction.

typedef __uint32_t Elf32_Addr;
typedef __uint32_t Elf32_Off;
typedef __uint16_t Elf32_Half;
typedef __uint32_t Elf32_Word;
typedef __int32_t Elf32_Sword;

typedef __uint64_t Elf64_Addr;
typedef __uint64_t Elf64_Off;
typedef __uint16_t Elf64_Half;
typedef __uint32_t Elf64_Word;
typedef __int32_t Elf64_Sword;
typedef __uint64_t Elf64_Xword;
typedef __int64_t Elf64_Sxword;

// Chapter 4 - Object Files: ELF Header.

#define EI_NIDENT 16  // Size of e_ident[].

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;
  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;

#define ET_NONE 0         // No file type.
#define ET_REL 1          // Relocatable file.
#define ET_EXEC 2         // Executable file.
#define ET_DYN 3          // Shared object file.
#define ET_CORE 4         // Core file.
#define ET_LOOS 0xfe00    // Operating system-specific.
#define ET_HIOS 0xfeff    // Operating system-specific.
#define ET_LOPROC 0xff00  // Processor-specific.
#define ET_HIPROC 0xffff  // Processor-specific.

#define EM_NONE 0
#define EM_M32 1
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8
#define EM_S370 9
#define EM_MIPS_RS3_LE 10
#define EM_PARISC 15
#define EM_VPP500 17
#define EM_SPARC32PLUS 18
#define EM_960 19
#define EM_PPC 20
#define EM_PPC64 21
#define EM_S390 22
#define EM_SPU 23
#define EM_V800 36
#define EM_FR20 37
#define EM_RH32 38
#define EM_RCE 39
#define EM_ARM 40
#define EM_ALPHA 41
#define EM_SH 42
#define EM_SPARCV9 43
#define EM_TRICORE 44
#define EM_ARC 45
#define EM_H8_300 46
#define EM_H8_300H 47
#define EM_H8S 48
#define EM_H8_500 49
#define EM_IA_64 50
#define EM_MIPS_X 51
#define EM_COLDFIRE 52
#define EM_68HC12 53
#define EM_MMA 54
#define EM_PCP 55
#define EM_NCPU 56
#define EM_NDR1 57
#define EM_STARCORE 58
#define EM_ME16 59
#define EM_ST100 60
#define EM_TINYJ 61
#define EM_X86_64 62
#define EM_PDSP 63
#define EM_PDP10 64
#define EM_PDP11 65
#define EM_FX66 66
#define EM_ST9PLUS 67
#define EM_ST7 68
#define EM_68HC16 69
#define EM_68HC11 70
#define EM_68HC08 71
#define EM_68HC05 72
#define EM_SVX 73
#define EM_ST19 74
#define EM_VAX 75
#define EM_CRIS 76
#define EM_JAVELIN 77
#define EM_FIREPATH 78
#define EM_ZSP 79
#define EM_MMIX 80
#define EM_HUANY 81
#define EM_PRISM 82
#define EM_AVR 83
#define EM_FR30 84
#define EM_D10V 85
#define EM_D30V 86
#define EM_V850 87
#define EM_M32R 88
#define EM_MN10300 89
#define EM_MN10200 90
#define EM_PJ 91
#define EM_OPENRISC 92
#define EM_ARC_COMPACT 93
#define EM_XTENSA 94
#define EM_VIDEOCORE 95
#define EM_TMM_GPP 96
#define EM_NS32K 97
#define EM_TPC 98
#define EM_SNP1K 99
#define EM_ST200 100
#define EM_IP2K 101
#define EM_MAX 102
#define EM_CR 103
#define EM_F2MC16 104
#define EM_MSP430 105
#define EM_BLACKFIN 106
#define EM_SE_C33 107
#define EM_SEP 108
#define EM_ARCA 109
#define EM_UNICORE 110
#define EM_EXCESS 111
#define EM_DXP 112
#define EM_ALTERA_NIOS2 113
#define EM_CRX 114
#define EM_XGATE 115
#define EM_C166 116
#define EM_M16C 117
#define EM_DSPIC30F 118
#define EM_CE 119
#define EM_M32C 120
#define EM_TSK3000 131
#define EM_RS08 132
#define EM_SHARC 133
#define EM_ECOG2 134
#define EM_SCORE7 135
#define EM_DSP24 136
#define EM_VIDEOCORE3 137
#define EM_LATTICEMICO32 138
#define EM_SE_C17 139
#define EM_TI_C6000 140
#define EM_TI_C2000 141
#define EM_TI_C5500 142
#define EM_TI_ARP32 143
#define EM_TI_PRU 144
#define EM_MMDSP_PLUS 160
#define EM_CYPRESS_M8C 161
#define EM_R32C 162
#define EM_TRIMEDIA 163
#define EM_QDSP6 164
#define EM_8051 165
#define EM_STXP7X 166
#define EM_NDS32 167
#define EM_ECOG1 168
#define EM_ECOG1X 168
#define EM_MAXQ30 169
#define EM_XIMO16 170
#define EM_MANIK 171
#define EM_CRAYNV2 172
#define EM_RX 173
#define EM_METAG 174
#define EM_MCST_ELBRUS 175
#define EM_ECOG16 176
#define EM_CR16 177
#define EM_ETPU 178
#define EM_SLE9X 179
#define EM_L10M 180
#define EM_K10M 181
#define EM_AARCH64 183
#define EM_AVR32 185
#define EM_STM8 186
#define EM_TILE64 187
#define EM_TILEPRO 188
#define EM_MICROBLAZE 189
#define EM_CUDA 190
#define EM_TILEGX 191
#define EM_CLOUDSHIELD 192
#define EM_COREA_1ST 193
#define EM_COREA_2ND 194
#define EM_ARC_COMPACT2 195
#define EM_OPEN8 196
#define EM_RL78 197
#define EM_VIDEOCORE5 198
#define EM_78KOR 199
#define EM_56800EX 200
#define EM_BA1 201
#define EM_BA2 202
#define EM_XCORE 203
#define EM_MCHP_PIC 204
#define EM_INTEL205 205
#define EM_INTEL206 206
#define EM_INTEL207 207
#define EM_INTEL208 208
#define EM_INTEL209 209
#define EM_KM32 210
#define EM_KMX32 211
#define EM_KMX16 212
#define EM_KMX8 213
#define EM_KVARC 214
#define EM_CDP 215
#define EM_COGE 216
#define EM_COOL 217
#define EM_NORC 218
#define EM_CSR_KALIMBA 219
#define EM_Z80 220
#define EM_VISIUM 221
#define EM_FT32 222
#define EM_MOXIE 223
#define EM_AMDGPU 224
#define EM_RISCV 243

#define EV_NONE 0     // Invalid version.
#define EV_CURRENT 1  // Current version.

#define EI_MAG0 0        // File identification.
#define EI_MAG1 1        // File identification.
#define EI_MAG2 2        // File identification.
#define EI_MAG3 3        // File identification.
#define EI_CLASS 4       // File class.
#define EI_DATA 5        // Data encoding.
#define EI_VERSION 6     // File version.
#define EI_OSABI 7       // Operating system/ABI identification.
#define EI_ABIVERSION 8  // ABI version.
#define EI_PAD 9         // Start of padding bytes.

#define ELFMAG0 0x7f  // e_ident[EI_MAG0].
#define ELFMAG1 'E'   // e_ident[EI_MAG1].
#define ELFMAG2 'L'   // e_ident[EI_MAG2].
#define ELFMAG3 'F'   // e_ident[EI_MAG3].

#define ELFCLASSNONE 0  // Invalid class.
#define ELFCLASS32 1    // 32-bit objects.
#define ELFCLASS64 2    // 64-bit objects.

#define ELFDATANONE 0  // Invalid data encoding.
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define ELFOSABI_NONE 0       // No extensions or unspecified.
#define ELFOSABI_HPUX 1       // Hewlett-Packard HP-UX.
#define ELFOSABI_NETBSD 2     // NetBSD.
#define ELFOSABI_GNU 3        // GNU.
#define ELFOSABI_LINUX 3      // Linux.
#define ELFOSABI_SOLARIS 6    // Sun Solaris.
#define ELFOSABI_AIX 7        // AIX.
#define ELFOSABI_IRIX 8       // IRIX.
#define ELFOSABI_FREEBSD 9    // FreeBSD.
#define ELFOSABI_TRU64 10     // Compaq TRU64 UNIX.
#define ELFOSABI_MODESTO 11   // Novell Modesto.
#define ELFOSABI_OPENBSD 12   // Open BSD.
#define ELFOSABI_OPENVMS 13   // Open VMS.
#define ELFOSABI_NSK 14       // Hewlett-Packard Non-Stop Kernel.
#define ELFOSABI_AROS 15      // Amiga Research OS.
#define ELFOSABI_FENIXOS 16   // The FenixOS highly scalable multi-core OS.
#define ELFOSABI_CLOUDABI 17  // Nuxi CloudABI.

// Chapter 4 - Introduction: Sections.

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_LOOS 0xff20
#define SHN_HIOS 0xff3f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_XINDEX 0xffff
#define SHN_HIRESERVE 0xffff

typedef struct {
  Elf32_Word sh_name;
  Elf32_Word sh_type;
  Elf32_Word sh_flags;
  Elf32_Addr sh_addr;
  Elf32_Off sh_offset;
  Elf32_Word sh_size;
  Elf32_Word sh_link;
  Elf32_Word sh_info;
  Elf32_Word sh_addralign;
  Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
  Elf64_Word sh_name;
  Elf64_Word sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
} Elf64_Shdr;

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_INIT_ARRAY 14
#define SHT_FINI_ARRAY 15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP 17
#define SHT_SYMTAB_SHNDX 18
#define SHT_LOOS 0x60000000
#define SHT_HIOS 0x6fffffff
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400
#define SHF_COMPRESSED 0x800
#define SHF_MASKOS 0x0ff00000
#define SHF_MASKPROC 0xf0000000

typedef struct {
  Elf32_Word ch_type;
  Elf32_Word ch_size;
  Elf32_Word ch_addralign;
} Elf32_Chdr;

typedef struct {
  Elf64_Word ch_type;
  Elf64_Word ch_reserved;
  Elf64_Xword ch_size;
  Elf64_Xword ch_addralign;
} Elf64_Chdr;

#define ELFCOMPRESS_ZLIB 1
#define ELFCOMPRESS_LOOS 0x60000000
#define ELFCOMPRESS_HIOS 0x6fffffff
#define ELFCOMPRESS_LOPROC 0x70000000
#define ELFCOMPRESS_HIPROC 0x7fffffff

#define GRP_COMDAT 0x1
#define GRP_MASKOS 0x0ff00000
#define GRP_MASKPROC 0xf0000000

// Chapter 4 - Introduction: Symbol Table.

#define STN_UNDEF 0

typedef struct {
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf32_Half st_shndx;
} Elf32_Sym;

typedef struct {
  Elf64_Word st_name;
  unsigned char st_info;
  unsigned char st_other;
  Elf64_Half st_shndx;
  Elf64_Addr st_value;
  Elf64_Xword st_size;
} Elf64_Sym;

#define ELF32_ST_BIND(i) ((i) >> 4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))

#define ELF64_ST_BIND(i) ((i) >> 4)
#define ELF64_ST_TYPE(i) ((i)&0xf)
#define ELF64_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))

#define ELF32_ST_VISIBILITY(o) ((o)&0x3)
#define ELF64_ST_VISIBILITY(o) ((o)&0x3)

#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_LOOS 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_LOOS 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_HIPROC 15

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

// Chapter 4 - Introduction: Relocation.

typedef struct {
  Elf32_Addr r_offset;
  Elf32_Word r_info;
} Elf32_Rel;

typedef struct {
  Elf32_Addr r_offset;
  Elf32_Word r_info;
  Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct {
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
} Elf64_Rel;

typedef struct {
  Elf64_Addr r_offset;
  Elf64_Xword r_info;
  Elf64_Sxword r_addend;
} Elf64_Rela;

#define ELF32_R_SYM(i) ((i) >> 8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))

#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i)&0xffffffffL)
#define ELF64_R_INFO(s, t) (((s) << 32) + ((t)&0xffffffffL))

// Chapter 5 - Program Loading and Dynamic Linking: Program Header.

typedef struct {
  Elf32_Word p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
} Elf32_Phdr;

typedef struct {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;
  Elf64_Addr p_vaddr;
  Elf64_Addr p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
} Elf64_Phdr;

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7
#define PT_LOOS 0x60000000
#define PT_GNU_EH_FRAME 0x6474e550
#define PT_GNU_RELRO 0x6474e552
#define PT_HIOS 0x6fffffff
#define PT_LOPROC 0x70000000
#define PT_ARM_EXIDX 0x70000001
#define PT_HIPROC 0x7fffffff

#define PF_X 0x1                // Execute.
#define PF_W 0x2                // Write.
#define PF_R 0x4                // Read.
#define PF_MASKOS 0x0ff00000    // Unspecified.
#define PF_MASKPROC 0xf0000000  // Unspecified.

// Chapter 5 - Program Loading and Dynamic Linking: Dynamic Linking.

typedef struct {
  Elf32_Sword d_tag;
  union {
    Elf32_Word d_val;
    Elf32_Addr d_ptr;
  } d_un;
} Elf32_Dyn;

typedef struct {
  Elf64_Sxword d_tag;
  union {
    Elf64_Xword d_val;
    Elf64_Addr d_ptr;
  } d_un;
} Elf64_Dyn;

#define DT_NULL 0
#define DT_NEEDED 1
#define DT_PLTRELSZ 2
#define DT_PLTGOT 3
#define DT_HASH 4
#define DT_STRTAB 5
#define DT_SYMTAB 6
#define DT_RELA 7
#define DT_RELASZ 8
#define DT_RELAENT 9
#define DT_STRSZ 10
#define DT_SYMENT 11
#define DT_INIT 12
#define DT_FINI 13
#define DT_SONAME 14
#define DT_RPATH 15
#define DT_SYMBOLIC 16
#define DT_REL 17
#define DT_RELSZ 18
#define DT_RELENT 19
#define DT_PLTREL 20
#define DT_DEBUG 21
#define DT_TEXTREL 22
#define DT_JMPREL 23
#define DT_BIND_NOW 24
#define DT_INIT_ARRAY 25
#define DT_FINI_ARRAY 26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_RUNPATH 29
#define DT_FLAGS 30
#define DT_ENCODING 32
#define DT_PREINIT_ARRAY 32
#define DT_PREINIT_ARRAYSZ 33
#define DT_LOOS 0x6000000D
#define DT_HIOS 0x6ffff000
#define DT_LOPROC 0x70000000
#define DT_HIPROC 0x7fffffff

#define DF_ORIGIN 0x1
#define DF_SYMBOLIC 0x2
#define DF_TEXTREL 0x4
#define DF_BIND_NOW 0x8
#define DF_STATIC_TLS 0x10

// Relocation types from the ELF for the ARM 64-bit Architecture (AArch64).

#define R_AARCH64_NONE 0
#define R_AARCH64_ABS64 257
#define R_AARCH64_ABS32 258
#define R_AARCH64_ABS16 259
#define R_AARCH64_PREL64 260
#define R_AARCH64_PREL32 261
#define R_AARCH64_PREL16 262
#define R_AARCH64_MOVW_UABS_G0 263
#define R_AARCH64_MOVW_UABS_G0_NC 264
#define R_AARCH64_MOVW_UABS_G1 265
#define R_AARCH64_MOVW_UABS_G1_NC 266
#define R_AARCH64_MOVW_UABS_G2 267
#define R_AARCH64_MOVW_UABS_G2_NC 268
#define R_AARCH64_MOVW_UABS_G3 269
#define R_AARCH64_MOVW_SABS_G0 270
#define R_AARCH64_MOVW_SABS_G1 271
#define R_AARCH64_MOVW_SABS_G2 272
#define R_AARCH64_LD_PREL_LO19 273
#define R_AARCH64_ADR_PREL_LO21 274
#define R_AARCH64_ADR_PREL_PG_HI21 275
#define R_AARCH64_ADR_PREL_PG_HI21_NC 276
#define R_AARCH64_ADD_ABS_LO12_NC 277
#define R_AARCH64_LDST8_ABS_LO12_NC 278
#define R_AARCH64_LDST16_ABS_LO12_NC 284
#define R_AARCH64_LDST32_ABS_LO12_NC 285
#define R_AARCH64_LDST64_ABS_LO12_NC 286
#define R_AARCH64_LDST128_ABS_LO12_NC 299
#define R_AARCH64_TSTBR14 279
#define R_AARCH64_CONDBR19 280
#define R_AARCH64_JUMP26 282
#define R_AARCH64_CALL26 283
#define R_AARCH64_MOVW_PREL_G0 287
#define R_AARCH64_MOVW_PREL_G0_NC 288
#define R_AARCH64_MOVW_PREL_G1 289
#define R_AARCH64_MOVW_PREL_G1_NC 290
#define R_AARCH64_MOVW_PREL_G2 291
#define R_AARCH64_MOVW_PREL_G2_NC 292
#define R_AARCH64_MOVW_PREL_G3 293
#define R_AARCH64_MOVW_GOTOFF_G0 300
#define R_AARCH64_MOVW_GOTOFF_G0_NC 301
#define R_AARCH64_MOVW_GOTOFF_G1 302
#define R_AARCH64_MOVW_GOTOFF_G1_NC 303
#define R_AARCH64_MOVW_GOTOFF_G2 304
#define R_AARCH64_MOVW_GOTOFF_G2_NC 305
#define R_AARCH64_MOVW_GOTOFF_G3 306
#define R_AARCH64_GOTREL64 307
#define R_AARCH64_GOTREL32 308
#define R_AARCH64_GOT_LD_PREL19 309
#define R_AARCH64_LD64_GOTOFF_LO15 310
#define R_AARCH64_ADR_GOT_PAGE 311
#define R_AARCH64_LD64_GOT_LO12_NC 312
#define R_AARCH64_LD64_GOTPAGE_LO15 313
#define R_AARCH64_TLSGD_ADR_PREL21 512
#define R_AARCH64_TLSGD_ADR_PAGE21 513
#define R_AARCH64_TLSGD_ADD_LO12_NC 514
#define R_AARCH64_TLSGD_MOVW_G1 515
#define R_AARCH64_TLSGD_MOVW_G0_NC 516
#define R_AARCH64_TLSLD_ADR_PREL21 517
#define R_AARCH64_TLSLD_ADR_PAGE21 518
#define R_AARCH64_TLSLD_ADD_LO12_NC 519
#define R_AARCH64_TLSLD_MOVW_G1 520
#define R_AARCH64_TLSLD_MOVW_G0_NC 521
#define R_AARCH64_TLSLD_LD_PREL19 522
#define R_AARCH64_TLSLD_MOVW_DTPREL_G2 523
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1 524
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC 525
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0 526
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC 527
#define R_AARCH64_TLSLD_ADD_DTPREL_HI12 528
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12 529
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC 530
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12 531
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC 532
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12 533
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC 534
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12 535
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC 536
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12 537
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC 538
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12 572
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC 573
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G1 539
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC 540
#define R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21 541
#define R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC 542
#define R_AARCH64_TLSIE_LD_GOTTPREL_PREL19 543
#define R_AARCH64_TLSLE_MOVW_TPREL_G2 544
#define R_AARCH64_TLSLE_MOVW_TPREL_G1 545
#define R_AARCH64_TLSLE_MOVW_TPREL_G1_NC 546
#define R_AARCH64_TLSLE_MOVW_TPREL_G0 547
#define R_AARCH64_TLSLE_MOVW_TPREL_G0_NC 548
#define R_AARCH64_TLSLE_ADD_TPREL_HI12 549
#define R_AARCH64_TLSLE_ADD_TPREL_LO12 550
#define R_AARCH64_TLSLE_ADD_TPREL_LO12_NC 551
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12 552
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC 553
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12 554
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC 555
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12 556
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC 557
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12 558
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC 559
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12 570
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC 571
#define R_AARCH64_TLSDESC_LD_PREL19 560
#define R_AARCH64_TLSDESC_ADR_PREL21 561
#define R_AARCH64_TLSDESC_ADR_PAGE21 562
#define R_AARCH64_TLSDESC_LD64_LO12 563
#define R_AARCH64_TLSDESC_ADD_LO12 564
#define R_AARCH64_TLSDESC_OFF_G1 565
#define R_AARCH64_TLSDESC_OFF_G0_NC 566
#define R_AARCH64_TLSDESC_LDR 567
#define R_AARCH64_TLSDESC_ADD 568
#define R_AARCH64_TLSDESC_CALL 569
#define R_AARCH64_COPY 1024
#define R_AARCH64_GLOB_DAT 1025
#define R_AARCH64_JUMP_SLOT 1026
#define R_AARCH64_RELATIVE 1027
#define R_AARCH64_TLS_DTPREL64 1028
#define R_AARCH64_TLS_DTPMOD64 1029
#define R_AARCH64_TLS_TPREL64 1030
#define R_AARCH64_TLSDESC 1031
#define R_AARCH64_IRELATIVE 1032

// Relocation types from the System V Application Binary Interface
// Intel386 Architecture Processor Supplement.

#define R_386_NONE 0
#define R_386_32 1
#define R_386_PC32 2
#define R_386_GOT32 3
#define R_386_PLT32 4
#define R_386_COPY 5
#define R_386_GLOB_DAT 6
#define R_386_JUMP_SLOT 7
#define R_386_RELATIVE 8
#define R_386_GOTOFF 9
#define R_386_GOTPC 10
#define R_386_TLS_TPOFF 14
#define R_386_TLS_IE 15
#define R_386_TLS_GOTIE 16
#define R_386_TLS_LE 17
#define R_386_TLS_GD 18
#define R_386_TLS_LDM 19
#define R_386_16 20
#define R_386_PC16 21
#define R_386_8 22
#define R_386_PC8 23
#define R_386_TLS_GD_32 24
#define R_386_TLS_GD_PUSH 25
#define R_386_TLS_GD_CALL 26
#define R_386_TLS_GD_POP 27
#define R_386_TLS_LDM_32 28
#define R_386_TLS_LDM_PUSH 29
#define R_386_TLS_LDM_CALL 30
#define R_386_TLS_LDM_POP 31
#define R_386_TLS_LDO_32 32
#define R_386_TLS_IE_32 33
#define R_386_TLS_LE_32 34
#define R_386_TLS_DTPMOD32 35
#define R_386_TLS_DTPOFF32 36
#define R_386_TLS_TPOFF32 37
#define R_386_SIZE32 38
#define R_386_TLS_GOTDESC 39
#define R_386_TLS_DESC_CALL 40
#define R_386_TLS_DESC 41
#define R_386_IRELATIVE 42

// Relocation types from the RISC-V ELF psABI specification.

#define R_RISCV_NONE 0
#define R_RISCV_32 1
#define R_RISCV_64 2
#define R_RISCV_RELATIVE 3
#define R_RISCV_COPY 4
#define R_RISCV_JUMP_SLOT 5
#define R_RISCV_TLS_DTPMOD32 6
#define R_RISCV_TLS_DTPMOD64 7
#define R_RISCV_TLS_DTPREL32 8
#define R_RISCV_TLS_DTPREL64 9
#define R_RISCV_TLS_TPREL32 10
#define R_RISCV_TLS_TPREL64 11
#define R_RISCV_BRANCH 16
#define R_RISCV_JAL 17
#define R_RISCV_CALL 18
#define R_RISCV_CALL_PLT 19
#define R_RISCV_GOT_HI20 20
#define R_RISCV_TLS_GOT_HI20 21
#define R_RISCV_TLS_GD_HI20 22
#define R_RISCV_PCREL_HI20 23
#define R_RISCV_PCREL_LO12_I 24
#define R_RISCV_PCREL_LO12_S 25
#define R_RISCV_HI20 26
#define R_RISCV_LO12_I 27
#define R_RISCV_LO12_S 28
#define R_RISCV_TPREL_HI20 29
#define R_RISCV_TPREL_LO12_I 30
#define R_RISCV_TPREL_LO12_S 31
#define R_RISCV_TPREL_ADD 32
#define R_RISCV_ADD8 33
#define R_RISCV_ADD16 34
#define R_RISCV_ADD32 35
#define R_RISCV_ADD64 36
#define R_RISCV_SUB8 37
#define R_RISCV_SUB16 38
#define R_RISCV_SUB32 39
#define R_RISCV_SUB64 40
#define R_RISCV_GNU_VTINHERIT 41
#define R_RISCV_GNU_VTENTRY 42
#define R_RISCV_ALIGN 43
#define R_RISCV_RVC_BRANCH 44
#define R_RISCV_RVC_JUMP 45
#define R_RISCV_RVC_LUI 46
#define R_RISCV_GPREL_I 47
#define R_RISCV_GPREL_S 48
#define R_RISCV_TPREL_I 49
#define R_RISCV_TPREL_S 50
#define R_RISCV_RELAX 51
#define R_RISCV_SUB6 52
#define R_RISCV_SET6 53
#define R_RISCV_SET8 54
#define R_RISCV_SET16 55
#define R_RISCV_SET32 56

// Relocation types from the System V ABI for AMD64.

#define R_X86_64_NONE 0
#define R_X86_64_64 1
#define R_X86_64_PC32 2
#define R_X86_64_GOT32 3
#define R_X86_64_PLT32 4
#define R_X86_64_COPY 5
#define R_X86_64_GLOB_DAT 6
#define R_X86_64_JUMP_SLOT 7
#define R_X86_64_RELATIVE 8
#define R_X86_64_GOTPCREL 9
#define R_X86_64_32 10
#define R_X86_64_32S 11
#define R_X86_64_16 12
#define R_X86_64_PC16 13
#define R_X86_64_8 14
#define R_X86_64_PC8 15
#define R_X86_64_DTPMOD64 16
#define R_X86_64_DTPOFF64 17
#define R_X86_64_TPOFF64 18
#define R_X86_64_TLSGD 19
#define R_X86_64_TLSLD 20
#define R_X86_64_DTPOFF32 21
#define R_X86_64_GOTTPOFF 22
#define R_X86_64_TPOFF32 23
#define R_X86_64_PC64 24
#define R_X86_64_GOTOFF64 25
#define R_X86_64_GOTPC32 26
#define R_X86_64_SIZE32 32
#define R_X86_64_SIZE64 33
#define R_X86_64_GOTPC32_TLSDESC 34
#define R_X86_64_TLSDESC_CALL 35
#define R_X86_64_TLSDESC 36
#define R_X86_64_IRELATIVE 37

#endif
