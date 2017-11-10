// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

static char const *const aliases_0[] = {
    "ip",
    NULL,
};
static char const *const aliases_84[] = {
    "iptm",
    NULL,
};
static char const *const aliases_255[] = {
    "raw",
    NULL,
};
static const char *const aliases_none[] = {NULL};
static const struct protoent entries[] = {
    {
        .p_name = (char *)"hopopt",
        .p_aliases = (char **)&aliases_0,
        .p_proto = 0,
    },
    {
        .p_name = (char *)"icmp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 1,
    },
    {
        .p_name = (char *)"igmp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 2,
    },
    {
        .p_name = (char *)"ggp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 3,
    },
    {
        .p_name = (char *)"ipv4",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 4,
    },
    {
        .p_name = (char *)"st",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 5,
    },
    {
        .p_name = (char *)"tcp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 6,
    },
    {
        .p_name = (char *)"cbt",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 7,
    },
    {
        .p_name = (char *)"egp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 8,
    },
    {
        .p_name = (char *)"igp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 9,
    },
    {
        .p_name = (char *)"bbn-rcc-mon",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 10,
    },
    {
        .p_name = (char *)"nvp-ii",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 11,
    },
    {
        .p_name = (char *)"pup",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 12,
    },
    {
        .p_name = (char *)"argus",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 13,
    },
    {
        .p_name = (char *)"emcon",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 14,
    },
    {
        .p_name = (char *)"xnet",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 15,
    },
    {
        .p_name = (char *)"chaos",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 16,
    },
    {
        .p_name = (char *)"udp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 17,
    },
    {
        .p_name = (char *)"mux",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 18,
    },
    {
        .p_name = (char *)"dcn-meas",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 19,
    },
    {
        .p_name = (char *)"hmp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 20,
    },
    {
        .p_name = (char *)"prm",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 21,
    },
    {
        .p_name = (char *)"xns-idp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 22,
    },
    {
        .p_name = (char *)"trunk-1",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 23,
    },
    {
        .p_name = (char *)"trunk-2",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 24,
    },
    {
        .p_name = (char *)"leaf-1",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 25,
    },
    {
        .p_name = (char *)"leaf-2",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 26,
    },
    {
        .p_name = (char *)"rdp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 27,
    },
    {
        .p_name = (char *)"irtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 28,
    },
    {
        .p_name = (char *)"iso-tp4",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 29,
    },
    {
        .p_name = (char *)"netblt",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 30,
    },
    {
        .p_name = (char *)"mfe-nsp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 31,
    },
    {
        .p_name = (char *)"merit-inp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 32,
    },
    {
        .p_name = (char *)"dccp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 33,
    },
    {
        .p_name = (char *)"3pc",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 34,
    },
    {
        .p_name = (char *)"idpr",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 35,
    },
    {
        .p_name = (char *)"xtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 36,
    },
    {
        .p_name = (char *)"ddp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 37,
    },
    {
        .p_name = (char *)"idpr-cmtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 38,
    },
    {
        .p_name = (char *)"tp++",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 39,
    },
    {
        .p_name = (char *)"il",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 40,
    },
    {
        .p_name = (char *)"ipv6",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 41,
    },
    {
        .p_name = (char *)"sdrp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 42,
    },
    {
        .p_name = (char *)"ipv6-route",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 43,
    },
    {
        .p_name = (char *)"ipv6-frag",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 44,
    },
    {
        .p_name = (char *)"idrp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 45,
    },
    {
        .p_name = (char *)"rsvp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 46,
    },
    {
        .p_name = (char *)"gre",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 47,
    },
    {
        .p_name = (char *)"dsr",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 48,
    },
    {
        .p_name = (char *)"bna",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 49,
    },
    {
        .p_name = (char *)"esp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 50,
    },
    {
        .p_name = (char *)"ah",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 51,
    },
    {
        .p_name = (char *)"i-nlsp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 52,
    },
    {
        .p_name = (char *)"swipe",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 53,
    },
    {
        .p_name = (char *)"narp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 54,
    },
    {
        .p_name = (char *)"mobile",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 55,
    },
    {
        .p_name = (char *)"tlsp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 56,
    },
    {
        .p_name = (char *)"skip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 57,
    },
    {
        .p_name = (char *)"ipv6-icmp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 58,
    },
    {
        .p_name = (char *)"ipv6-nonxt",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 59,
    },
    {
        .p_name = (char *)"ipv6-opts",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 60,
    },
    {
        .p_name = (char *)"cftp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 62,
    },
    {
        .p_name = (char *)"sat-expak",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 64,
    },
    {
        .p_name = (char *)"kryptolan",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 65,
    },
    {
        .p_name = (char *)"rvd",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 66,
    },
    {
        .p_name = (char *)"ippc",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 67,
    },
    {
        .p_name = (char *)"sat-mon",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 69,
    },
    {
        .p_name = (char *)"visa",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 70,
    },
    {
        .p_name = (char *)"ipcv",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 71,
    },
    {
        .p_name = (char *)"cpnx",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 72,
    },
    {
        .p_name = (char *)"cphb",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 73,
    },
    {
        .p_name = (char *)"wsn",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 74,
    },
    {
        .p_name = (char *)"pvp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 75,
    },
    {
        .p_name = (char *)"br-sat-mon",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 76,
    },
    {
        .p_name = (char *)"sun-nd",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 77,
    },
    {
        .p_name = (char *)"wb-mon",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 78,
    },
    {
        .p_name = (char *)"wb-expak",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 79,
    },
    {
        .p_name = (char *)"iso-ip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 80,
    },
    {
        .p_name = (char *)"vmtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 81,
    },
    {
        .p_name = (char *)"secure-vmtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 82,
    },
    {
        .p_name = (char *)"vines",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 83,
    },
    {
        .p_name = (char *)"ttp",
        .p_aliases = (char **)&aliases_84,
        .p_proto = 84,
    },
    {
        .p_name = (char *)"nsfnet-igp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 85,
    },
    {
        .p_name = (char *)"dgp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 86,
    },
    {
        .p_name = (char *)"tcf",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 87,
    },
    {
        .p_name = (char *)"eigrp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 88,
    },
    {
        .p_name = (char *)"ospfigp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 89,
    },
    {
        .p_name = (char *)"sprite-rpc",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 90,
    },
    {
        .p_name = (char *)"larp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 91,
    },
    {
        .p_name = (char *)"mtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 92,
    },
    {
        .p_name = (char *)"ax.25",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 93,
    },
    {
        .p_name = (char *)"ipip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 94,
    },
    {
        .p_name = (char *)"micp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 95,
    },
    {
        .p_name = (char *)"scc-sp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 96,
    },
    {
        .p_name = (char *)"etherip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 97,
    },
    {
        .p_name = (char *)"encap",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 98,
    },
    {
        .p_name = (char *)"gmtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 100,
    },
    {
        .p_name = (char *)"ifmp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 101,
    },
    {
        .p_name = (char *)"pnni",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 102,
    },
    {
        .p_name = (char *)"pim",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 103,
    },
    {
        .p_name = (char *)"aris",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 104,
    },
    {
        .p_name = (char *)"scps",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 105,
    },
    {
        .p_name = (char *)"qnx",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 106,
    },
    {
        .p_name = (char *)"a/n",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 107,
    },
    {
        .p_name = (char *)"ipcomp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 108,
    },
    {
        .p_name = (char *)"snp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 109,
    },
    {
        .p_name = (char *)"compaq-peer",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 110,
    },
    {
        .p_name = (char *)"ipx-in-ip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 111,
    },
    {
        .p_name = (char *)"vrrp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 112,
    },
    {
        .p_name = (char *)"pgm",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 113,
    },
    {
        .p_name = (char *)"l2tp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 115,
    },
    {
        .p_name = (char *)"ddx",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 116,
    },
    {
        .p_name = (char *)"iatp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 117,
    },
    {
        .p_name = (char *)"stp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 118,
    },
    {
        .p_name = (char *)"srp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 119,
    },
    {
        .p_name = (char *)"uti",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 120,
    },
    {
        .p_name = (char *)"smp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 121,
    },
    {
        .p_name = (char *)"sm",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 122,
    },
    {
        .p_name = (char *)"ptp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 123,
    },
    {
        .p_name = (char *)"isis-over-ipv4",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 124,
    },
    {
        .p_name = (char *)"fire",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 125,
    },
    {
        .p_name = (char *)"crtp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 126,
    },
    {
        .p_name = (char *)"crudp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 127,
    },
    {
        .p_name = (char *)"sscopmce",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 128,
    },
    {
        .p_name = (char *)"iplt",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 129,
    },
    {
        .p_name = (char *)"sps",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 130,
    },
    {
        .p_name = (char *)"pipe",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 131,
    },
    {
        .p_name = (char *)"sctp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 132,
    },
    {
        .p_name = (char *)"fc",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 133,
    },
    {
        .p_name = (char *)"rsvp-e2e-ignore",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 134,
    },
    {
        .p_name = (char *)"mobility-header",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 135,
    },
    {
        .p_name = (char *)"udplite",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 136,
    },
    {
        .p_name = (char *)"mpls-in-ip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 137,
    },
    {
        .p_name = (char *)"manet",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 138,
    },
    {
        .p_name = (char *)"hip",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 139,
    },
    {
        .p_name = (char *)"shim6",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 140,
    },
    {
        .p_name = (char *)"wesp",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 141,
    },
    {
        .p_name = (char *)"rohc",
        .p_aliases = (char **)&aliases_none,
        .p_proto = 142,
    },
    {
        .p_name = (char *)"reserved",
        .p_aliases = (char **)&aliases_255,
        .p_proto = 255,
    },
};
