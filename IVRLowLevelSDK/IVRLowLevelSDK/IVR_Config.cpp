#include "IVR_Config.h"

QApplication*  CIVRConfig::IVR_QtApp=nullptr;
bool           CIVRConfig::IVR_GUIActive=false;

void FireThread()
{
    while(CIVRConfig::IVR_GUIActive)
    {
        CIVRConfig::IVR_QtApp->processEvents();
    }
}

CIVRConfig::CIVRConfig(bool bOpenOfflineThread)
{
    // Code placed here will run in the game thread
    int   argc = 1;
    char* argv[2] = { (char*)"dummy", {} };

    if(bOpenOfflineThread)
    {
        IVR_QtApp = new QApplication(argc, argv);
        if(IVR_QtApp)
        {
            IVR_GUIActive = true;
            IVR_Thread = QtConcurrent::run(FireThread);
        }
    }
}

CIVRConfig::CIVRConfig()
{

}
