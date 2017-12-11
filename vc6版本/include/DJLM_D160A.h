#ifndef DJLM_D160A_H
#define DJLM_D160A_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// definition of Record Block Flag
#define		RECORD_BLOCK_FLAG_NORMAL	0
#define		RECORD_BLOCK_FLAG_LAST		1

// definition of WaveFormatType
#define	RAW_DATA_OF_A_LAW		0
#define	WAVE_FORMAT_A_LAW		1
#define	WAVE_FORMAT_PCM_8		2

typedef	struct {
	int		TotalChannelNumber;
	int		BufferSize;
	int		WaveFormatType;
	int		PackRate;
	char	UnUsed[48];
} TYPE_DJLM_D160A_INFO;

int	WINAPI	DJLM_D160A_GetInfo ( TYPE_DJLM_D160A_INFO *pInfo );
int	WINAPI	DJLM_D160A_Init (void);



#ifdef __cplusplus
}
#endif


#endif
