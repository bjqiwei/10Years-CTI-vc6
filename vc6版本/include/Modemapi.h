//Add by WGL 1998.11.26
#include "faxapi32.h"

#ifndef _MODEMAPI32_H
#define _MODEMAPI32_H

#define RESULT_MODEM_OK        0x0000
#define RESULT_MODEM_WORKING   0x0001
#define RESULT_MODEM_CONNECT   0x0002
#define RESULT_MODEM_ERROR     0x0003
#define RESULT_LINE_ERROR      0x0004
#define RESULT_MODEM_NOCARRIER 0x0005

#ifdef __cplusplus
extern "C" {
#endif
int WINAPI DJFax_SetModemInitString(char *InitString);
int WINAPI DJFax_SetModemMode(WORD wChnl,WORD Mode,char *DialString);
int WINAPI DJFax_CheckConnect(WORD wChnl);
int WINAPI DJFax_GetModemError(WORD wChnl);
void WINAPI DJFax_HangUp(WORD wChnl);
int WINAPI DJFax_CheckHangUp(WORD wChnl);

int WINAPI DJFax_ReadModemBuf(WORD wChnl,char *DataBuf);
int WINAPI DJFax_WriteModemBuf(WORD wChnl,char *DataBuf,int DataLen);

#ifdef __cplusplus
}
#endif


#define MAX_MODEM_LINE_NUM  32
#define MAX_RESULT_NUM      83
#define MAX_RESULT_COUNT    30
#define MAX_COMMAND_NUM      2
#define MAX_COMMAND_COUNT 1024

#define MAX_DIAL_NUM         256

#define MODE_ANSWER         0x00
#define MODE_CALL           0x01

#define STATUS_IDLE			0x00
#define STATUS_CONTROL		0x01
#define STATUS_SEND_MODEM	0x02
#define STATUS_SEND_COMMAND	0x03
#define STATUS_TO_CONNECT	0x05
#define STATUS_CONNECT		0x06
#define STATUS_DATA2COMMAND	0x07
#define STATUS_HANG_UP		0x08
struct ModemContrlBlock{
        int  Status;
        WORD Mode;
        char DialString[256];
        char Buffer[3096];
        int  BuffCount;
        int  CommandCount;
};

#endif	//_MODEMAPI32_H
