#ifndef CIVRIFFREADER_H
#define CIVRIFFREADER_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"

class IVRLOWLEVELSDK_EXPORT CIVRIFFReader : public QObject
{
public:
    CIVRIFFReader();

    void IVR_AppendData   (IVR_FrameData &pData,QString path);
    void IVR_AppendData   (IVR_FrameData &pData,QString root,QString cam,uint take);
    bool IVR_ReadFrameData(                     QString path);
    bool IVR_ReadFrameData(QString root,QString cam,uint take);

    uint  IVR_GetAverageFPS();
    uint  IVR_GetBestFPS();
    uint  IVR_GetFPSByTimeApprox();

    //---------------------------------------------------------------------
    //Append in the frames file a single realtime frame
    //---------------------------------------------------------------------
    FILE *frameFile;
    QString filePath;
    vector<IVR_FrameData> IIFrameData;
};

#endif // CIVRIFFREADER_H
