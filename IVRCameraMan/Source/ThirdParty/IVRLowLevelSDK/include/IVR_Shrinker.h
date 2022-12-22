#ifndef CIVRSHRINKER_H
#define CIVRSHRINKER_H

#include "IVRLowLevelSDK_global.h"

class IVRLOWLEVELSDK_EXPORT CIVRShrinker : public QObject
{
public:
    CIVRShrinker();
   ~CIVRShrinker();

    bool    Shrink  (char *pData,ulong pSize);
    bool    DeShrink(char *pData,ulong pOriginalSize);

    char * GetData();
    ulong  GetSize();
    bool   IsShrinked();

private:
    bool                   IVR_IsShrinked;
    char  *                IVR_ShrinkedData;
    ulong                  IVR_ShrinkedSize;
    ulong                  IVR_OriginalSize;
    uint                   IVR_CompressorType;

    qlz_state_compress     * state_compress;
    qlz_state_decompress   * state_decompress;
};

#endif // CIVRSHRINKER_H
