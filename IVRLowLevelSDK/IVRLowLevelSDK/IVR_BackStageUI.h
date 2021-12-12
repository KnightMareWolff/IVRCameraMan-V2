#ifndef CIVRBACKSTAGEUI_H
#define CIVRBACKSTAGEUI_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_LowLevelSDK.h"

class CIVRBackStageUI : public QWidget
{
    Q_OBJECT
public:
    explicit CIVRBackStageUI(QWidget *parent = nullptr);

    QTextEdit      *pIVR_OutputLog;
    QTextCharFormat pIVR_TextFormat;

    QVBoxLayout *TabLayout;
    QTabWidget  *tabWidget;

    int          pIVR_CurrentTab;

    IVRLowLevelSDK* pInterfaceRef;

protected:
    void resizeEvent(QResizeEvent *event);

signals:

public slots:
    void LoadIndex(int pagina);

};

#endif // CIVRBACKSTAGEUI_H
