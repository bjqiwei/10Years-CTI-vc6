#ifndef CONF95_H
#define CONF95_H

#define	MAX_GROUP_NUM		10
#define MAX_CH_PER_GROUP	32

#define	ATTE_0DB                0x00
#define	ATTE_MINUS_3DB		0x40
#define	ATTE_MINUS_6DB		0x80

#define	NOISE_NONE		0x00
#define	NOISE_9_OF_4096		0x10
#define	NOISE_16_OF_4096	0x20
#define	NOISE_32_OF_4096	0x30


#define	TOTAL_CONF_RESOURCE_NUMBER		32

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI EnableConfCard(int *ConfAddr,int TotalCard);
void WINAPI DisableConfCard(void);
int WINAPI AddChnl ( int ConfNo, int ChannelNo, int ChnlAtte, int NoiseSupp );
int WINAPI SubChnl ( int ConfNo, int ChannelNo );
int WINAPI AddListenChnl ( int ConfNo, int ChannelNo );
int WINAPI SubListenChnl ( int ConfNo, int ChannelNo );
int WINAPI DConf_Adjust_CtrlWord ( WORD wChnl, int wCtrl );

// add for New-D160 conf mode
int WINAPI DConf_EnableConfCard(void);
void WINAPI DConf_DisableConfCard(void);
int WINAPI DConf_AddRecListenChnl ( int ConfNo, int ChannelNo );
int WINAPI DConf_SubRecListenChnl ( int ConfNo, int ChannelNo );
int	WINAPI DConf_GetResNumber (void);

// add for TimeSlot Conf, 2003.10.02
int	WINAPI DConf_AddChnl_TimeSlot ( int ConfNo, WORD wTimeSlot, int ChnlAtte, int NoiseSupp, WORD *pTS_CONF );
int	WINAPI DConf_SubChnl_TimeSlot ( int ConfNo, WORD wTS_Out );
int	WINAPI DConf_AddListenChnl_TimeSlot ( int ConfNo, WORD *pTS_CONF );

// add for Conf Play, 2003.12.23
int WINAPI DConf_AddPlayChnl ( int ConfNo, int ChannelNo, int ChnlAtte, int NoiseSupp );
int WINAPI DConf_SubPlayChnl ( int ConfNo, int ChannelNo );

#ifdef __cplusplus
}
#endif

#endif	//CONF95_H
