#ifndef __StrucDefine_H__
#define __StrucDefine_H__

//By default, program compiled at IBM-compatible PC	has the address alignment issues which don't exist 
//in C51 environment. For convenience of data-handling,we use the compact-pack mode for storing the
//exchanging data-buffer used between PC and EliteIV device.For more ,pls refer to sample 17.

#ifdef _WIN32
#pragma pack(push, 1)
#endif 

#define IO_PACKAGE_BUF_SIZE 86 //IO package size = sizeof(IO_PACKAGE)

/* command group definition */
#define CMD_DBL_COMPVARKEY 0x00 // CompareVarKey( int nKey1,int nKey2)
#define CMD_DBL_ISTHISKEY 0x01 // if it is the key
#define CMD_DBL_GET_KEY 0x02 //Get License Key
#define CMD_DBL_GET_CHNLCOUNT 0x03 //Get channel count
#define CMD_DBL_GET_ISFAX 0x04 //if have fax
#define CMD_DBL_GET_ISCONF 0x05 //if have conference
#define CMD_DBL_GET_SYSTYPE 0x06 //system type	  0 IVR 1:CTI
#define CMD_DBL_GET_CARDTYPE 0x07 //Card type	  0 dongji 1:dialogic
#define CMD_DBL_GET_DTMF 0x08 //Get DTMF
#define CMD_DBL_ANALOGTRUNK 0x09 //have AnalogTrunk ?
#define CMD_DBL_DIGITALTRUNK 0x10 //have DigitalTrunk ?	
#define CMD_DBL_CHANGEVOICE 0x11 //change voice ?
#define CMD_DBL_SS7PCMTYPE 0x12 //PCM type  ? 0:SS7 TUP  1:SS7 ISUP 2: SS7 TUP and ISUP
#define CMD_DBL_COMPANY_NAME 0x13 //公司简称
#define CMD_DBL_HETONG_BIANHAO 0x14 //合同编号
#define CMD_DBL_XUKE_RIQI 0x15 //许可日期
#define CMD_DBL_DAYU 0x16 //大于
#define CMD_DBL_XIAOYU 0x17 //小于


/* common error code */
#define ERR_SUCCESS				0x00	// success
#define ERR_SES					0x80	// SES function execution error
#define ERR_INVALID_PARAMETER	0x81	// invalid parameter


/* IO package */
typedef struct _IO_PACKAGE
{	
	unsigned char cmd;	
	unsigned char len;
	unsigned char szKey[24];//Key
	unsigned char szParam1[20];//input/Output
	unsigned char szParam2[20];//input/Output
	unsigned char szResult[20];//Output
}IO_PACKAGE;

#ifdef _WIN32
#pragma pack(pop)
#endif


#endif //__StrucDefine_H__