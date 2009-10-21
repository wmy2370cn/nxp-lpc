#ifndef __IPSTRUCTS_H__
#define __IPSTRUCTS_H__



#if _MSC_VER > 1000
#pragma once

#ifndef ANY_SIZE
#define ANY_SIZE 1
#endif

//
#define DEVICE_TYPE ULONG
#define FILE_DEVICE_BEEP                0x00000001
#define FILE_DEVICE_CD_ROM              0x00000002
#define FILE_DEVICE_CD_ROM_FILE_SYSTEM  0x00000003
#define FILE_DEVICE_CONTROLLER          0x00000004
#define FILE_DEVICE_DATALINK            0x00000005
#define FILE_DEVICE_DFS                 0x00000006
#define FILE_DEVICE_DISK                0x00000007
#define FILE_DEVICE_DISK_FILE_SYSTEM    0x00000008
#define FILE_DEVICE_FILE_SYSTEM         0x00000009
#define FILE_DEVICE_INPORT_PORT         0x0000000a
#define FILE_DEVICE_KEYBOARD            0x0000000b
#define FILE_DEVICE_MAILSLOT            0x0000000c
#define FILE_DEVICE_MIDI_IN             0x0000000d
#define FILE_DEVICE_MIDI_OUT            0x0000000e
#define FILE_DEVICE_MOUSE               0x0000000f
#define FILE_DEVICE_MULTI_UNC_PROVIDER  0x00000010
#define FILE_DEVICE_NAMED_PIPE          0x00000011
#define FILE_DEVICE_NETWORK             0x00000012
#define FILE_DEVICE_NETWORK_BROWSER     0x00000013
#define FILE_DEVICE_NETWORK_FILE_SYSTEM 0x00000014
#define FILE_DEVICE_NULL                0x00000015
#define FILE_DEVICE_PARALLEL_PORT       0x00000016
#define FILE_DEVICE_PHYSICAL_NETCARD    0x00000017
#define FILE_DEVICE_PRINTER             0x00000018
#define FILE_DEVICE_SCANNER             0x00000019
#define FILE_DEVICE_SERIAL_MOUSE_PORT   0x0000001a
#define FILE_DEVICE_SERIAL_PORT         0x0000001b
#define FILE_DEVICE_SCREEN              0x0000001c
#define FILE_DEVICE_SOUND               0x0000001d
#define FILE_DEVICE_STREAMS             0x0000001e
#define FILE_DEVICE_TAPE                0x0000001f
#define FILE_DEVICE_TAPE_FILE_SYSTEM    0x00000020
#define FILE_DEVICE_TRANSPORT           0x00000021
#define FILE_DEVICE_UNKNOWN             0x00000022
#define FILE_DEVICE_VIDEO               0x00000023
#define FILE_DEVICE_VIRTUAL_DISK        0x00000024
#define FILE_DEVICE_WAVE_IN             0x00000025
#define FILE_DEVICE_WAVE_OUT            0x00000026
#define FILE_DEVICE_8042_PORT           0x00000027
#define FILE_DEVICE_NETWORK_REDIRECTOR  0x00000028
#define FILE_DEVICE_BATTERY             0x00000029
#define FILE_DEVICE_BUS_EXTENDER        0x0000002a
#define FILE_DEVICE_MODEM               0x0000002b
#define FILE_DEVICE_VDM                 0x0000002c
#define FILE_DEVICE_MASS_STORAGE        0x0000002d

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3

#define FILE_ANY_ACCESS                 0
#define FILE_READ_ACCESS          ( 0x0001 )	// file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )	// file & pipe


//defines and structs taken from ntddpack.h
struct _PACKET_OID_DATA {
    ULONG Oid;
    ULONG Length;
    UCHAR Data[1];
}; 

typedef struct _PACKET_OID_DATA PACKET_OID_DATA, *PPACKET_OID_DATA;

/*#include <packoff.h> */
#define FILE_DEVICE_PROTOCOL        0x8000
#define IOCTL_PROTOCOL_QUERY_OID    CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_SET_OID      CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_STATISTICS   CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_RESET        CTL_CODE(FILE_DEVICE_PROTOCOL, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_READ         CTL_CODE(FILE_DEVICE_PROTOCOL, 4 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_WRITE        CTL_CODE(FILE_DEVICE_PROTOCOL, 5 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_MACNAME      CTL_CODE(FILE_DEVICE_PROTOCOL, 6 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_OPEN                  CTL_CODE(FILE_DEVICE_PROTOCOL, 7 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CLOSE                 CTL_CODE(FILE_DEVICE_PROTOCOL, 8 , METHOD_BUFFERED, FILE_ANY_ACCESS)

//defines and structs taken from packet32.h
//working modes
#define MODE_CAPT 0
#define MODE_STAT 1

//ioctls
#define	 pBIOCSETBUFFERSIZE 9592
#define	 pBIOCSETF 9030
#define  pBIOCGSTATS 9031
#define	 pBIOCSRTIMEOUT 7416
#define	 pBIOCSMODE 7412
#define	 pBIOCSWRITEREP 7413
#define	 pBIOCSMINTOCOPY 7414
#define	 pBIOCSETOID 2147483648
#define	 pBIOCQUERYOID 2147483652
#define	 pATTACHPROCESS 7117
#define	 pDETACHPROCESS 7118
#define  pBIOCEVNAME 7415

#define  pBIOCSTIMEZONE 7471

//Network type structure
typedef struct NetType
{
	UINT LinkType;
	UINT LinkSpeed;
}NetType;


// Alignment macros.  Packet_WORDALIGN rounds up to the next 
// even multiple of Packet_ALIGNMENT. 
#define Packet_ALIGNMENT sizeof(int)
#define Packet_WORDALIGN(x) (((x)+(Packet_ALIGNMENT-1))&~(Packet_ALIGNMENT-1))



struct bpf_program {
	UINT bf_len;
	struct bpf_insn *bf_insns;
};
struct bpf_insn {
	USHORT	code;
	UCHAR 	jt;
	UCHAR 	jf;
	int k;
};
struct bpf_stat {
	UINT bs_recv;		///< number of packets received 
	UINT bs_drop;		///< number of packets dropped 
};

struct bpf_hdr {
	struct timeval	bh_tstamp;		///<time stamp 
	UINT	bh_caplen;				///<length of captured portion 
	UINT	bh_datalen;				///<original length of packet
	USHORT		bh_hdrlen;			///<length of bpf header (this struct plus alignment padding)
};


#define        DOSNAMEPREFIX   TEXT("Packet_")
#define        MAX_LINK_NAME_LENGTH   64
#define        NMAX_PACKET 65535  

typedef struct _ADAPTER  { 
						   HANDLE hFile;
                           TCHAR  SymbolicLink[MAX_LINK_NAME_LENGTH];
						   int NumWrites;
						   HANDLE ReadEvent;
						   UINT ReadTimeOut;
						 }  ADAPTER, *LPADAPTER;

typedef struct _PACKET {  
						  HANDLE       hEvent;
                          OVERLAPPED   OverLapped;
                          PVOID        Buffer;
                          UINT         Length;
						  UINT         ulBytesReceived;
						  BOOLEAN      bIoComplete;
						}  PACKET, *LPPACKET;




//iphelper structs
typedef struct _MIB_TCPROW_EX
{
    DWORD	dwState;		// MIB_TCP_STATE_*
    DWORD	dwLocalAddr;
    DWORD	dwLocalPort;
    DWORD	dwRemoteAddr;
    DWORD	dwRemotePort;
	DWORD	dwProcessId;
} MIB_TCPROW_EX, *PMIB_TCPROW_EX;

typedef struct _MIB_TCPTABLE_EX
{
    DWORD			dwNumEntries;
    MIB_TCPROW_EX	table[ANY_SIZE];
} MIB_TCPTABLE_EX, *PMIB_TCPTABLE_EX;

typedef struct _MIB_UDPROW_EX
{
    DWORD	dwLocalAddr;
    DWORD	dwLocalPort;
	DWORD	dwProcessId;
} MIB_UDPROW_EX, *PMIB_UDPROW_EX;

typedef struct _MIB_UDPTABLE_EX
{
    DWORD			dwNumEntries;
    MIB_UDPROW_EX	table[ANY_SIZE];
} MIB_UDPTABLE_EX, *PMIB_UDPTABLE_EX;
/*
DWORD
WINAPI
AllocateAndGetTcpExTableFromStack(
	OUT PMIB_TCPTABLE_EX	*pTcpTableEx,
	IN BOOL					bOrder,
	IN HANDLE				hAllocHeap,
	IN DWORD				dwAllocFlags,
	IN DWORD				dwProtocolVersion; // 2 - TCP, 23 - TCPv6 (size of *pTcpTableEx must be 56!)
);
*/
typedef DWORD (WINAPI *PROCALLOCATEANDGETTCPEXTABLEFROMSTACK)(PMIB_TCPTABLE_EX*,BOOL,HANDLE,DWORD,DWORD);

/*
DWORD
WINAPI
AllocateAndGetUdpExTableFromStack(
	OUT PMIB_UDPTABLE_EX	*pUdpTable,
	IN BOOL					bOrder,
	IN HANDLE				hAllocHeap,
	IN DWORD				dwAllocFlags,
	IN DWORD				dwProtocolVersion; // 2 - UDP, 23 - UDPv6 (size of *pUdpTable must be 28!)
);
*/
typedef DWORD (WINAPI *PROCALLOCATEANDGETUDPEXTABLEFROMSTACK)(PMIB_UDPTABLE_EX*,BOOL,HANDLE,DWORD,DWORD);

// IP header
#pragma pack(push, 1) //The IP_HEADER and ICMP_HEADER should be alligned on 1 byte boundaries
typedef struct tagIP_HEADER 
{
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum
	unsigned int sourceIP;
	unsigned int destIP;
} IP_HEADER;
typedef IP_HEADER FAR* LPIP_HEADER;

// ICMP header
typedef struct tagICMP_HEADER 
{
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
} ICMP_HEADER;
typedef ICMP_HEADER FAR* LPICMP_HEADER;
#pragma pack(pop)




#endif // _MSC_VER > 1000

#endif __IPSTRUCTS_H__

