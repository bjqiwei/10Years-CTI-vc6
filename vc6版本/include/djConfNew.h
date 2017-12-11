#ifndef __DJCONFNEW_H__
#define __DJCONFNEW_H__

#include "djconf.h"


////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_CONF_BOARD_NUM			  2		//系统允许的最大会议板卡数(物理板卡)
#define CONF_CARD_NUM_PER_BOARD		  4     //每片会议卡包含的逻辑板卡数
#define MAX_CONF_CARD_NUM_NEW	      MAX_CONF_BOARD_NUM*CONF_CARD_NUM_PER_BOARD	//系统允许的最大会议逻辑板卡数
#define MAX_CONF_RES_NUM_PER_CARD     64	//每片会议逻辑板卡资源数
#define MAX_CONF_GROUP_NUM_PER_CARD   32	//每片会议逻辑板卡允许创建的最大会议组数
#define MAX_CONF_GROUP_NUM_NEW	      256	//系统允许创建的最大会议组数
#define MAX_MEMBER_NUM_PER_GROUP_NEW  512   //会议组中允许的最大成员数
#define MAX_SPEAKER_NUM_PER_GROUP     63	//会议组中允许拥有发言权的最大成员数
#define MAX_SPEAKING_NUM_PER_GROUP    6	    //会议组中允许同时说话的最大成员数



typedef enum {
	CONF_NULL,								//空闲(没有通道加入会议)
	CONF_LISTEN,							//只听不说(监听)
	CONF_SPEAKER_NORMAL,					//具有动态发言权
	CONF_SPEAKER_ALWAYS,					//总是具有发言权
	CONF_SPEAKER_ONLY						//只有发言权(只说不听，用于对会议放音)
}MEMBER_MODE;

typedef struct {
	WORD	wCardNum;						//逻辑板卡数
	WORD	wCardType;						//板卡类型
	WORD	wGroupNum;						//最大会议组数
	WORD    wNewConfFlag;					//新会议功能标志
	WORD	wGroupMaxMember;				//会议组中允许的最大成员数
	WORD	wGroupMaxSpeaker;				//会议组中允许具有发言权的最大成员数
	WORD	wGroupMaxSpeaking;				//会议组中允许同时说话的最大成员数
	WORD	wMaxSilenceTime;				//最大静默时长
	WORD	wJoinedByEnergy;				//抢占式发言标志
	WORD	wAddress[MAX_CONF_CARD_NUM];
	WORD	wStream[MAX_CONF_CARD_NUM];
} CONF_SYS_INFO_NEW;

typedef struct {
	WORD			wGroupID;				//会议组ID号
	WORD			wMemberID;				//会议组中成员ID号

	BYTE			cbChnlType;				//通道类型
	WORD			wChnlNo;				//通道号
	WORD			wResNo;                 //会议相应的输出时隙

	MEMBER_MODE		eMode;					//模式
	BOOL			bActive;				//当前处于活动状态标志

	char			cAtteVal;				//增益调整
	BYTE			cbNoiseSupp;			//静噪门限
	DWORD			dwVADKeepTime;			//VAD状态的时长
	BOOL			bVADFlag;				//是否处于说话状态
} MEMBER_CTRL;

typedef struct {
	WORD			wGroupID;				//会议组ID号
	
	WORD			wMaxMemberNum;			//允许最大会议成员数
	WORD			wMaxSpeakerNum;			//允许能够发言最大成员数
	WORD			wMaxSpeakingNum;		//允许同时发言最大成员数
	WORD			wMinSlienceTime;		//最小静默时间
	WORD			wJoinedByEnergy;		//抢占式发言标志

	WORD			wCardNo;				//资源预留的逻辑板卡号
    WORD            wGroupNo;               //会议组对应的逻辑卡的会议组号

	WORD			wMemberNum;				//当前会议成员数
	WORD			wResNum;				//当前占用资源数
	WORD			wListenNum;				//当前监听成员数
	WORD			wListenNo;              //会议当前监听的输出时隙
	WORD            wListenSlot;            //会议当前时隙索引
	WORD			wSpeakerNum;			//当前允许发言成员数
	WORD			wSpeakingNum;			//当前正在发言成员数
	
	MEMBER_CTRL	MemberCtrl[MAX_MEMBER_NUM_PER_GROUP_NEW];//指向会议成员数组
} GROUP_CTRL;

#ifndef __BUILD_FOR_SYS__

//functions:
#ifdef	__cplusplus
extern "C" {
#endif

//新会议初始化函数
int  WINAPI DJConf_InitConfCardNew(void);
void WINAPI DJConf_ReleaseConfCardNew(void);
void WINAPI DJConf_GetSysInfoNew(CONF_SYS_INFO_NEW *TmpCSIN);

//新会议基本函数
int  WINAPI DJConf_CreateConfGroup(int iMaxMember, int iMaxSpeaker, int iMaxSpeaking, 
								   int iMaxSilenceTime, int iJoinedByEnergy);
int  WINAPI DJConf_CreateConfGroupEx(int iGroupID, int iMaxMember, int iMaxSpeaker, 
								     int iMaxSpeaking, int iMaxSilenceTime, int iJoinedByEnergy);
int WINAPI DJConf_FreeConfGroup(int iGroupID);

int  WINAPI DJConf_JoinConfGroup(int iGroupID, BYTE cbChnlType, WORD wChnlNo, MEMBER_MODE eMode,
								 char chAtte, BYTE cbNoiseSupp, BOOL bCreateGroup);
int  WINAPI DJConf_ExitConfGroup(BYTE cbChnlType, WORD wChnlNo, BOOL bFreeGroupKeep);
int  WINAPI DJConf_ExitConfGroupEx(MEMBER_CTRL* pMemberCtrl, BOOL bFreeGroupKeep);

//新会议组/成员信息
int  WINAPI DJConf_GetGroupInfoNew(int iGroupID, GROUP_CTRL *TmpGroupCtrl);
int  WINAPI DJConf_GetGroupMemberInfo(int iGroupID, int iMemberID, MEMBER_CTRL *TmpMemberCtrl);
long WINAPI DJConf_GetChnlGroupInfoNew(BYTE cbChnlType, WORD wChnlNo);
int  WINAPI DJConf_SetChnlAtte(BYTE cbChnlType, WORD wChnlNo, char chAtte);
int  WINAPI DJConf_SetChnlAtteEx(MEMBER_CTRL* pMemberCtrl, char chAtte);

#ifdef	__cplusplus
}
#endif

#endif
////////////////////////////////////////////////////////////////////////////////////////////
#endif //#ifndef __DJCONFNEW_H__