#ifndef _DJE1TTS_H
#define _DJE1TTS_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void	WINAPI	DJTTS_IsReleaseVer(void);

int		WINAPI	DJTTS_Init(void);
void	WINAPI	DJTTS_Release(void);
int		WINAPI DJTTS_GetNowTTSNum(void);

int		WINAPI	DJTTS_StartPlayText ( int trunkID, char *pText, int size, int IsFemale );
int		WINAPI	DJTTS_CheckPlayTextEnd ( int trunkID );
void	WINAPI	DJTTS_StopPlayText ( int trunkID );

int		WINAPI	DJTTS_CvtTextToVocFile ( char *pText, int size, int IsFemale, char *pVocName );

#ifdef __cplusplus
}
#endif


#endif

