#ifndef CIVRCONFIG_H
#define CIVRCONFIG_H

#if defined _WIN32 || defined _WIN64
    #define IVRLOWLEVELSDK_EXPORT __declspec(dllexport)
#else
    #include <stdio.h>
#endif

#ifndef IVRLOWLEVELSDK_EXPORT
    #define IVRLOWLEVELSDK_EXPORT
#endif

#pragma warning( disable : 4251 4244 4267 4005)

//Comment/ucomment when its bein used inside Unreal
#include "QtWidgets/qapplication.h"
#include "QtWidgets/qwidget.h"
#include "QtWidgets/qtextedit.h"
#include "QtWidgets/QVBoxLayout"
#include "QtWidgets/qtabwidget.h"
#include "QtCore/qstring.h"
#include "QtCore/qqueue.h"
#include "QtCore/qobject.h"
#include "QtWidgets/qmessagebox.h"
#include "QtGui/qvector2d.h"
#include "QtGui/qvector3d.h"
#include "QtGui/qimage.h"
#include "QtCore/qfile.h"
#include "QtCore/qdatastream.h"
#include "QtWidgets/qfiledialog.h"
#include "QtCore/qfuture.h"
#include "QtConcurrent/QtConcurrent"
#include "QtCore/qelapsedtimer.h"

#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "opencv2/core.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/videostab.hpp"
#include "opencv2/xfeatures2d.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/avassert.h"
#include "libavutil/channel_layout.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

extern "C" {
#include "quicklz.h"
}

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace QtConcurrent;

class IVRLOWLEVELSDK_EXPORT CIVRConfig
{
public:
    CIVRConfig(                       );

    //Configuration Folders
    static QString IVR_RootFolder        ;
    static QString IVR_ConfigFolder      ;
    static bool    IVR_DebbugMode        ;
    static uint    IVR_MSecToWait        ;
    static int     IVR_Width             ;
    static int     IVR_Height            ;
    static const size_t  IVR_GQueueSize  ;
};

#endif // CIVRCONFIG_H
