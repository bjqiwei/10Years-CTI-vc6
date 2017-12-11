#ifndef _DJ_DFM_DLL_H_
#define _DJ_DFM_DLL_H_


typedef	struct {
	int		BaseSerialPort;
	int		TotalSerialNum;
	int		TotalCardNum;

	char	UnUsed[64-(3*sizeof(int))];
} TYPE_DJDFM_INFO;


#define	DJ_MODEM_STATUS_ONHOOK					0
#define	DJ_MODEM_STATUS_OFFHOOK_ATA				1
#define	DJ_MODEM_STATUS_OFFHOOK_ATD				2	
#define	DJ_MODEM_STATUS_OFFHOOK_DATA			3
#define	DJ_MODEM_STATUS_OFFHOOK_COMMAND			4
#define	DJ_MODEM_STATUS_OFFHOOK_ONLINE_COMMAND	5


#define		MAX_DIAL_BUFFER_SIZE	20

#ifdef __cplusplus
extern "C" {
#endif

int		WINAPI	DFM_CheckReady ( TYPE_DJDFM_INFO	*pDfmInfo );
void	WINAPI	DFM_CloseDriver ( void );

DWORD	WINAPI	DFM_ReadPassword ( int	DFM_CardID );

int		WINAPI	DFM_GetTimeSlot ( int	DFM_ChnlID );
int		WINAPI	DFM_ConnectFromTS ( int DFM_ChnlID, int iTS );
int		WINAPI	DFM_DisconnectTS ( int DFM_ChnlID );

int		WINAPI	DFM_SetRingState ( int DFM_ChnlID, int RingState );
int		WINAPI	DFM_GetModemLineStatus ( int DFM_ChnlID );
int		WINAPI	DFM_GetDialNumber ( int DFM_ChnlID, char *DialBuf );

int		WINAPI	DFM_GetDebugInfo ( int DFM_ChnlID, BYTE *pBuffer );

#ifdef __cplusplus
}
#endif


#endif
