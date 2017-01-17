// some structs defs grabbed from the internet mostly

#ifndef _SMB_H
#define _SMB_H


typedef unsigned char u8, __u8;
typedef unsigned short u16, __u16;
typedef unsigned long u32, __u32, __le32;

typedef u16 __le16;


#pragma pack(push, 1)
struct smb_hdr {
	__u32 smb_buf_length;   /* big endian on wire *//* BB length is only two
													or three u8s - with one or two u8 type preceding it that are
													zero - we could mask the type u8 off just in case BB */
	__u8 Protocol[4];
	__u8 Command;
	union {
		struct {
			__u8 ErrorClass;
			__u8 Reserved;
			__le16 Error;
		};
		__le32 CifsError;
	};
	__u8 Flags;
	__le16 Flags2;          /* note: le */
	__le16 PidHigh;
	union {
		struct {
			__le32 SequenceNumber;  /* le */
			__u32 Reserved; /* zero */
		} Sequence;
		__u8 SecuritySignature[8];      /* le */
	} Signature;
	__u8 pad[2];
	__u16 Tid;
	__le16 Pid;
	__u16 Uid;
	__u16 Mid;
	__u8 WordCount;
};

typedef struct negotiate_req {
	struct smb_hdr hdr;     /* wct = 0 */
							// __u16 w1;
							// __u16 w2;
	__le16 u8Count;
	unsigned char DialectsArray[1];
} NEGOTIATE_REQ;


typedef struct lanman_neg_rsp {
	struct smb_hdr hdr;     /* wct = 13 */
	__le16 DialectIndex;
	__le16 SecurityMode;
	__le16 MaxBufSize;
	__le16 MaxMpxCount;
	__le16 MaxNumberVcs;
	__le16 RawMode;
	__le32 SessionKey;
	struct {
		__le16 Time;
		__le16 Date;
	} SrvTime;
	__le16 ServerTimeZone;
	__le16 EncryptionKeyLength;
	__le16 Reserved;
	__u16  u8Count;
	unsigned char EncryptionKey[1];
} LANMAN_NEG_RSP;

#define READ_RAW_ENABLE 1
#define WRITE_RAW_ENABLE 2
#define RAW_ENABLE (READ_RAW_ENABLE | WRITE_RAW_ENABLE)


//  SMB2
typedef struct negotiate_rsp {
	//struct smb_hdr hdr;     /* wct = 17 */

	char smbh[64 + 4];

	__le16	neg_length;
	__u8	SecurityMode;
	__le16	DialectIndex; /* 0xFFFF = no dialect acceptable */
	__u16	reserved;
	unsigned char GUID[16];
	__le32	Capabilities;    /* see below */
	__le32	MaxTransactSize;
	__le32	MaxReadSize;
	__le32	MaxWriteSize;
	__u8 r;
	__le32 SystemTimeLow;
	__le32 SystemTimeHigh;
	__le32 ServerTimeLow;
	__le32 ServerTimeHigh;

	// rest is not interestting to us



} NEGOTIATE_RSP;



typedef struct _smb1_negotiate_rsp {
	struct smb_hdr hdr;     /* wct = 17 */
	__le16 DialectIndex; /* 0xFFFF = no dialect acceptable */
	__u8 SecurityMode;
	__le16 MaxMpxCount;
	__le16 MaxNumberVcs;
	__le32 MaxBufferSize;
	__le32 MaxRawSize;
	__le32 SessionKey;
	__le32 Capabilities;    /* see below */
	__le32 SystemTimeLow;
	__le32 SystemTimeHigh;
	__le16 ServerTimeZone;
	__u8 EncryptionKeyLength;
	__u16 u8Count;
	union {
		unsigned char EncryptionKey[1]; /* cap extended security off */
										/* followed by Domain name - if extended security is off */
										/* followed by 16 u8s of server GUID */
										/* then security blob if cap_extended_security negotiated */
		struct {
			unsigned char GUID[16];
			unsigned char SecurityBlob[1];
		} extended_response;
	} u;
} SMB1_NEGOTIATE_RSP;



#pragma pack(pop)





#endif#pragma once
