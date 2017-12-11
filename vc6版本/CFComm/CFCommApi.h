#ifndef CFCOMMAPI_H
#define CFCOMMAPI_H



typedef void(LPFUNCEXPORTDATA1)( LPVOID lpData,int nDataType,int bufLen);


const int CFCOMM_EVENT_CTI_MSG		= 1;
const int CFCOMM_EVENT_ACD_MSG		= 2;
const int CFCOMM_EVENT_IVR_MSG		= 3;
const int CFCOMM_EVENT_COMM_MSG	= 4;
const int CFCOMM_EVENT_AGENT_MSG	= 5;
const int CFCOMM_EVENT_SUP_MSG		= 6;
const int CFCOMM_EVENT_DBB_MSG		= 7;//DB Broker
const int CFCOMM_EVENT_SOCKET_MSG  = 8;

const int CFCOMM_EVENT_SYS_MSG		= 11;

const int CFCOMM_SOCKET_CLOSE_MSG  = 9;//soket close
const int CFCOMM_SOCKET_CONN_MSG  = 10;//socket connect

/*=============================================================
clent mode event type const define. Please 

================================================================*/





extern "C" _declspec(dllexport) bool InitCFCommDLL( unsigned int Port, const char* IP,  LPFUNCEXPORTDATA1 *p,  int nSMType, int nSMID );
extern "C" _declspec(dllexport) bool ExitCFCommDLL(void);
extern "C" _declspec(dllexport) bool ReadCommData(char *buf, int nLen);
extern "C" _declspec(dllexport) bool SendCommData(char *buf, int nLen);


#endif