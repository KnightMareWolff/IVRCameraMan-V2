#ifndef CIVRLAUNCHERUI_H
#define CIVRLAUNCHERUI_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_BackStageUI.h"

class CIVRLauncherUI : public QObject
{
    CIVRBackStageUI *IVR_BackstageTool=nullptr;
      // other components
      //..
    public:
      virtual bool event( QEvent *ev )
      {
          if( ev->type() == QEvent::User )
          {
             if(!IVR_BackstageTool)
             {
                IVR_BackstageTool = new CIVRBackStageUI;
                IVR_BackstageTool->show();
                return true;
             }
             else
             {
                IVR_BackstageTool->show();
             }
             return true;
          }
          return false;
      }

    bool hasValidUI()
    {
        if(IVR_BackstageTool)return true;
        return false;
    }
    void attachInterfaceInstance(IVRLowLevelSDK *pInterface)
    {
        if(IVR_BackstageTool)
        {
            IVR_BackstageTool->pInterfaceRef = pInterface;
        }
    }

    void sendLogString(QString pLogString)
    {
        if(IVR_BackstageTool)
        {
            IVR_BackstageTool->pIVR_OutputLog->insertPlainText(pLogString);
        }
    }
};

#endif // CIVRLAUNCHERUI_H
