#ifndef NORMARCHBOARDS_H
#define NORMARCHBOARDS_H

extern "C" _declspec(dllexport) bool NMB_Init(int nTotalChnl,/*通道数*/ 
											  int nBoardsType=0,/*信令类别:0单纯模拟卡;2一号信令；3数字一号信令；4数字7号信令TUP;5数字7号信令ISUP;*/
											  int nFax=0,/*是否有传真：0无传真；1有传真*/ 
											  int nChangeVoice=0,/*是否有变声：0无变声；1有变声*/
											  int conference=0,/*是否有增强会议：0普通会议；1增强会议*/
											  int nSwitchType=0/*通道间连接方式：0普通连接；1会议连接*/
											  );/*初始化板卡*/
//通道放音函数
extern "C" _declspec(dllexport) int  NMB_GetChnlType( int nIndex/*通道序号*/ );/*取得通道类型*/
extern "C" _declspec(dllexport) bool NMB_AddIndexPlay( int nIndex, CString strFileName );//增加索引放音文件
extern "C" _declspec(dllexport) bool NMB_StartIndexPlay( int nIndex );//开始一次文件索引放音
extern "C" _declspec(dllexport) bool NMB_CheckIndexPlayEnd (int nIndex );//检查索引文件放音是否结束
extern "C" _declspec(dllexport) bool NMB_StopIndexPlay( int nIndex );//开始一次文件索引放音

//会议放音函数
extern "C" _declspec(dllexport) bool NMB_StartConfPlay( int nGroupNo/*会议号*/,char *FileName );//开始会议放音
extern "C" _declspec(dllexport) void NMB_StopConfPlay( int nGroupNo  );//停止会议放音
extern "C" _declspec(dllexport) bool NMB_CheckConfPlayEnd( int nGroupNo );//检查会议放音是否结束

//通道录音函数
extern "C" _declspec(dllexport) bool NMB_StartRecordFile(int nIndex,CString strFileName,DWORD RecordLen );//开始文件录音
extern "C" _declspec(dllexport) bool NMB_CheckRecordFileEnd(int nIndex );//检查文件录音是否结束
extern "C" _declspec(dllexport) void NMB_StopRecordFile(int nIndex);//停止文件录音

//会议录音函数
extern "C" _declspec(dllexport) bool NMB_StartConfRecord( int nGroupNo,CString strFileName ,DWORD Length));//开始会议录音
extern "C" _declspec(dllexport) bool NMB_CheckConfRecordEnd( int nGroupNo );//检查会议录音是否结束
extern "C" _declspec(dllexport) void NMB_StopConfRecord( int nGroupNo );//停止会议录音

//DTMF函数
extern "C" _declspec(dllexport) void NMB_InitDtmfBuf( int nIndex );//清空系统的DTMF缓冲区
extern "C" _declspec(dllexport) bool NMB_GetDtmfString( int nIndex,CString &strDtmf );//得到通道的DTMF按键串，并从缓冲区中移去
extern "C" _declspec(dllexport) bool NMB_SendDtmfString( int nIndex, CString &strDtmf );//发送DTMF按键串

extern "C" _declspec(dllexport) bool NMB_GetCallerIDStr( int nIndex, CString &strCallerID/*主叫号码*/, CString &strCalledID/*被叫号码*/ );//取得主被叫

//extern "C" _declspec(dllexport) int NMB_GetChnlState( int nIndex );//得到通道状态

#endif 