#ifndef	_NEWCONF_H
#define	_NEWCONF_H


#define		A_MAX_CONF_CARD_NUM							16
#define		A_MAX_CONF_RESOURCE_NUM_PER_CARD			32
#define		A_MAX_CONF_RESOURCE_NUM						(A_MAX_CONF_RESOURCE_NUM_PER_CARD*A_MAX_CONF_CARD_NUM)
#define		A_MAX_CONF_GROUP_NUM						(16*A_MAX_CONF_CARD_NUM)
#define		A_MAX_MEMBER_NUM_PER_GROUP_MASS				512


#define		A_CONF_MODE_ADD				0
#define		A_CONF_MODE_LISTEN			1
#define		A_CONF_MODE_SPEAK			2
#define		A_CONF_MODE_BALANCE_RECORD	3

#define		CONF_CHANNEL_TYPE_TRUNK		0
#define		CONF_CHANNEL_TYPE_USER		1
#define		CONF_CHANNEL_TYPE_VOICE		2	
#define		CONF_CHANNEL_TYPE_TIMESLOT	3

#define		CONF_CHANNEL_TYPE_ANALOG	CONF_CHANNEL_TYPE_USER

typedef struct {
	WORD	wCardNum;
	WORD	wCardType;
	WORD	wGroupNum;
	WORD	wAddress[A_MAX_CONF_CARD_NUM];
	WORD	wStream[A_MAX_CONF_CARD_NUM];
} A_CONF_SYS_INFO;

typedef struct {
	BYTE	cbMode;
	BYTE	cbChnlType;
	WORD	wChnlNo;
	WORD	wResNo;
	WORD    wLDNo;
} A_MEMBER_INFO;

typedef struct {
	WORD	wMemberNum;
	WORD	wResNum;
	WORD	wListenNum;
	A_MEMBER_INFO	MemberInfo[A_MAX_MEMBER_NUM_PER_GROUP_MASS];
} A_GROUP_INFO_MASS;

#ifdef	__cplusplus
extern "C" {
#endif

int		WINAPI NewConf_InitConfCard ( void );
void	WINAPI NewConf_ReleaseConfCard ( void );

void	WINAPI NewConf_GetSysInfo ( A_CONF_SYS_INFO *TmpCSI );

int		WINAPI NewConf_EnterConf ( WORD wGroupNo, BYTE cbMode, BYTE cbChnlType, WORD wChnlNo, 
					   int iAtte, int iNoiseSupp );
int		WINAPI NewConf_LeaveConf ( BYTE cbChnlType, WORD wChnlNo, WORD wStartGroup );

int		WINAPI NewConf_LeaveBalanceRecord ( int iRecChnl );

int		WINAPI NewConf_GetAvailResNum ( WORD wConfCardNo );
int		WINAPI NewConf_GetGroupInfo ( WORD wGroupNo, A_GROUP_INFO_MASS *TmpGroupInfo );

void	WINAPI NewConf_Set_CtrlWord ( WORD wCtrl );

#ifdef	__cplusplus
}
#endif

#endif