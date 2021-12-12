#include "IVR_BackStageUI.h"

CIVRBackStageUI::CIVRBackStageUI(QWidget *parent) : QWidget(parent)
{
    pIVR_OutputLog = new QTextEdit(parent);
    pIVR_TextFormat.setFont(QFont("Courier New",8, QFont::Courier));
    pIVR_OutputLog->setCurrentCharFormat(pIVR_TextFormat);
    pIVR_OutputLog->setTextColor(QColor(0,0,0));
    pIVR_OutputLog->setTextBackgroundColor(QColor(255,255,255));

    tabWidget = new QTabWidget();
    tabWidget->addTab(pIVR_OutputLog, QString("Backstage Console"));

    tabWidget->setUsesScrollButtons(true);
    TabLayout = new QVBoxLayout;
    TabLayout->addWidget(tabWidget);
    setLayout(TabLayout);

    pIVR_CurrentTab    = 0;

    connect(tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(LoadIndex(int)));
}

void CIVRBackStageUI::LoadIndex(int pagina)
{
    tabWidget->setCurrentIndex(pagina);
    switch(pagina)
    {
    case 0:{pIVR_CurrentTab    = 0;}break;
    case 1:{pIVR_CurrentTab    = 0;}break;
    case 2:{pIVR_CurrentTab    = 0;}break;
    case 3:{pIVR_CurrentTab    = 0;}break;
    case 4:{pIVR_CurrentTab    = 0;}break;
    }
}



void CIVRBackStageUI::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    tabWidget->resize(QSize(width(),height()));
}
