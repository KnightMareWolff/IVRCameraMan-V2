#include "IVR_Config.h"

QString       CIVRConfig::IVR_RootFolder;
QString       CIVRConfig::IVR_ConfigFolder;
bool          CIVRConfig::IVR_DebbugMode;
uint          CIVRConfig::IVR_MSecToWait;
int           CIVRConfig::IVR_Width     ;
int           CIVRConfig::IVR_Height    ;
const size_t  CIVRConfig::IVR_GQueueSize=2000;

CIVRConfig::CIVRConfig()
{

}
