#ifndef CIVRIMAGEHANDLER_H
#define CIVRIMAGEHANDLER_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_CartoonFilter.h"
#include "IVR_BinaryFilter.h"
#include "IVR_DilateFilter.h"
#include "IVR_OldFilmFilter.h"
#include "IVR_PencilSketchFilter.h"
#include "IVR_SepiaFilter.h"
#include "IVR_EmbossFilter.h"
#include "IVR_DuoToneFilter.h"
#include "IVR_WarmFilter.h"
#include "IVR_ColdFilter.h"
#include "IVR_GothamFilter.h"
#include "IVR_SharpenFilter.h"
#include "IVR_DetailFilter.h"
#include "IVR_InvertFilter.h"
#include "IVR_StylizeFilter.h"
#include "IVR_GhostFilter.h"


class IVRLOWLEVELSDK_EXPORT CIVRImageHandler : public QObject
{
public:
    CIVRImageHandler();

    CIVRImageHandler( const CIVRImageHandler & copy )
    {
        IVR_ImageName  = copy.IVR_ImageName;
        IVR_Size       = copy.IVR_Size;
        IVR_Brightness = copy.IVR_Brightness;
        IVR_Contrast   = copy.IVR_Contrast;
        IVR_ImageData  = copy.IVR_ImageData.clone();

        //Warning!
        //If you find out some effects not being applyed as expected comming from Copied Cameras...
        //We need implement copy constructors and atrib operator for each effect and perform a
        //dynamic_cast<FilterType> for both during the copy...The Filter array stores pointers and
        //will generate leaks if we destroy the original camera...
        for(auto filter : copy.filterArray)
        {
           IVR_AddFilter(filter->IVR_FilterType);
        }
    }

    CIVRImageHandler& operator = (const CIVRImageHandler &t)
    {
        IVR_ImageName  = t.IVR_ImageName;
        IVR_Size       = t.IVR_Size;
        IVR_Brightness = t.IVR_Brightness;
        IVR_Contrast   = t.IVR_Contrast;
        IVR_ImageData  = t.IVR_ImageData.clone();

        //Warning!
        //If you find out some effects not being applyed as expected comming from Copied Cameras...
        //We need implement copy constructors and atrib operator for each effect and perform a
        //dynamic_cast<FilterType> for both during the copy...The Filter array stores pointers and
        //will generate leaks if we destroy the original camera...
        for(auto filter : t.filterArray)
        {
           IVR_AddFilter(filter->IVR_FilterType);
        }

        return *this;
    }

    //The Image Name are provided when we want mantain a list of images for training
    QString IVR_ImageName;

    //Informs the Size of the Image comming from the Hardware(Usefull to rescale the Image after a Processing)
    QVector2D IVR_Size;
    uint      IVR_PixelSize;

    //Informs the Brightness Balance
    int       IVR_Brightness;
    //Informs the Contrast Balance
    double    IVR_Contrast;

    //Image Handler data sources
    Mat IVR_ImageData;

    //Loading Methods
    bool IVR_LoadImage   (QString pFullPath);
    bool IVR_LoadEffect  (QString effectName);
    bool IVR_LoadImageW64(QString pFullPath);
    bool IVR_LoadImageBUF(const char * pIVRBUFEncodedBuffer, int pIVRBUFSize);
    bool IVR_LoadImageBUF(const char * pIVRBUFEncodedBuffer, int pIVRWidth, int pIVRHeight, int pIVRPixelSize);
    bool IVR_LoadImageW64(const char * pIVRW64EncodedBuffer, int pIVRW64Size);
    bool IVR_LoadImageMAT(const Mat& pIVRMatImage);

    bool IVR_SaveImage (QString pFullPath);
    bool IVR_SaveEffect(QString effectName);

    void IVR_SetSize       (int    pIVRWidth     ,int pIVRHeight);
    void IVR_SetBrightness (int    pIVRBrightness               );
    void IVR_SetContrast   (double pIVRContrast                 );

    void IVR_SetFilterParameters(float pIVR_Treshold01,
                                 float pIVR_Treshold02,
                                 int   pIVR_ApertureSize,
                                 bool  pIVR_UseL2Gradient,
                                 int   pIVR_KernelSize,
                                 float pIVR_SigmaS,
                                 float pIVR_SigmaR,
                                 int   pIVR_Iteractions,
                                 int   pIVR_LightIntensity,
                                 int   pIVR_ToneChannel01,
                                 int   pIVR_ToneChannel02,
                                 float pIVR_ExpValue,
                                 float pIVR_ShadeFactor,
                                 bool  pIVR_ColorOutput,
                                 int   pIVR_R,
                                 int   pIVR_G,
                                 int   pIVR_B,
                                 int   pIVR_Transparency);

    void IVR_AddFilter(int pIVRFilterType);
    bool IVR_ApplyFilters();
    void IVR_CleanUpFilters();

private:

    vector<CIVRImageFilter *> filterArray;


};

#endif // CIVRIMAGEHANDLER_H
