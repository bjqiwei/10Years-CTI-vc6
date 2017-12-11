#ifndef _G723TOWAVE_H
#define _G723TOWAVE_H
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

//Mode- 0:6.3k 1:5.3K 
//返回值：转换后的WAVE数据流的字节数
//说明：用户自己分配InBuf和OutBuf的空间.
//如果是G.723(6.3K)的，则要求InBuf为24的整数个字节，如果是G.723(5.3K)，则要求InBuf为20的整数个字节
//OutBuf的大小应为240*n个字节，n=(InBuf字节数/24(或20)).
int WINAPI G723ToWave(char* InBuf,int iInBufSize,short int* OutBuf,int Mode);
int WINAPI WaveToG723(short* InBuf,int iInBufSize,char* OutBuf,int Mode);

//G723格式文件到WAVE格式文件的转换。
//G723File：g723文件名   WaveFile：转换后得到的WAVE文件的文件名   Mode: 0:6.3k 1:5.3K
int WINAPI G723FToWaveF(char* G723File,char* WaveFile,int Mode);
int WINAPI WaveFToG723F(char* WaveFile,char* G723File,int Mode);
/*
#ifdef __cplusplus
}
#endif
*/

#endif