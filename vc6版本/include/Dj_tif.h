#ifndef __H_DJTIFF_H__
#define __H_DJTIFF_H__
#ifdef __cplusplus
extern "C" {
#endif
int WINAPI DJCvt_Bfx2Tiff(char *Bfxfilename,char *Tifffilename);
int WINAPI DJCvt_Tiff2Bfx(char *Tifffilename,char *Bfxfilename);
#ifdef __cplusplus 
}
#endif

#endif