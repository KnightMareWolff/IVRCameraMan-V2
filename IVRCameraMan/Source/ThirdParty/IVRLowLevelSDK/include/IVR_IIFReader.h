#ifndef CIVRIIFREADER_H
#define CIVRIIFREADER_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Shrinker.h"

class IVRLOWLEVELSDK_EXPORT CIVRIIFReader : public QObject
{
public:
    CIVRIIFReader();


    void IVR_AppendData (IVR_RenderBuffer &pData,QString path);
    void IVR_AppendData (IVR_RenderBuffer &pData,QString root,QString cam,uint take,uint frame);

    bool IVR_ReadImageData(QString path,bool compressionEnabled);
    bool IVR_ReadImageData(QString root,QString cam,uint take,uint frame,bool compressionEnabled);

    bool IVR_IsValid(){return isValid;}

    void IVR_DeleteFile()
    {
        QFile deleteImageFile(filePath);
        deleteImageFile.remove();
    }
    //---------------------------------------------------------------------
    //Append in the frames file a single realtime frame
    //---------------------------------------------------------------------
    QString filePath;
    FILE *imageFile;
    bool  isValid=false;
    IVR_RenderBuffer ivrBuffer;
    CIVRShrinker *shrinker;

};

#endif // CIVRIIFREADER_H
