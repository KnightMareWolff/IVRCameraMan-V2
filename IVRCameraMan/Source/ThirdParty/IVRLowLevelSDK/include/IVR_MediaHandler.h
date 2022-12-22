#ifndef CIVRMEDIAHANDLER_H
#define CIVRMEDIAHANDLER_H

#include "IVRLowLevelSDK_global.h"

class IVRLOWLEVELSDK_EXPORT CIVRMediaHandler : public QObject
{
public:
    CIVRMediaHandler();

    //--------------------------------------------------------------------------------
    //Remux all media streams from the Origin into a new Destination media.
    //--------------------------------------------------------------------------------
    //Warning:
    //We are using the main ffmpeg encoders/decoders and some types of media are used
    //only for sound , or are not supported by some players.
    //The list of sucessfull formats used with this function are:
    // .mp4 | .vlc | .mkv | .mov | .mp2 / .mp3 | .wmv | .mpg
    //
    //The function expects the full path to the file + filename + extension
    //--------------------------------------------------------------------------------
    //Example of Use:
    //IVR_RemuxMedia("VideoInput.mp4","VideoOutput.mkv");
    //--------------------------------------------------------------------------------
    bool IVR_RemuxMedia(const QString pOriFileName  ,const QString pDstFileName);

    //-----------------------------------------------------------------------
    //Mix a Video File with a Sound File (Muxing)
    //-----------------------------------------------------------------------
    //Warning:
    //The function expect a fresh new realtime video to be mixed with an
    //audio file.
    //So, it works well only when you have a video without sound and an
    //audio media data like an mp3.
    //Most of media files are limited in their streams and if you try mix 2
    //audio media, for example, the first one will be considered , and in
    //other situations a crash may be occour, since this depends on the codec
    //encoder being used and detected by ffmpeg
    //We are planning extend mixing operations for video/audio data in other
    //classes, since this operations are related to recording stuff and not
    //media manipulation/handling.
    //
    //The function expects the full path to the file + filename + extension
    //----------------------------------------------------------------------
    //Example of use:
    //IVR_MuxMedia("VideoInput.mp4" ,"AudioInput.mp3","VideoOutput.mp4");
    //--------------------------------------------------------------------
    bool IVR_MuxMedia  (const QString pVideoFileName,const QString pAudioFileName ,const QString pOutputFileName);

    //-----------------------------------------------------------------------
    //DeMux a Video File in a Sound File & Video File
    //-----------------------------------------------------------------------
    //Warning:
    //
    //The function expects the full path to the file + filename + extension
    //----------------------------------------------------------------------
    //Example of use:
    //IVR_DemuxMedia("VideoInput.mp4" ,"AudioOutput.mp3","VideoOutput.mp4");
    //--------------------------------------------------------------------
    bool IVR_DemuxMedia (const QString pVideoFileName,const QString pAudioOutputFileName ,const QString pVideoOutputFileName);

    //-----------------------------------------------------------------------
    //Concatenates two Media Files one after other (with ReMuxing)
    //-----------------------------------------------------------------------
    //Warning:
    //The function expect two video only realtime videos to be concatenated
    //
    //Most of media files are limited in their streams and if you try mix 2
    //videos with big sounds media, for example, the first one will be
    //considered , and in other situations a crash may be occour, since
    //this depends on the codecs rules.
    //
    //The function expects the full path to the file + filename + extension
    //----------------------------------------------------------------------
    //Example of use:
    //IVR_CatMedia("VideoInput01.mp4" ,"VideoInput02.mp4","VideoOutput.mp4");
    //--------------------------------------------------------------------
    bool IVR_CatMedia  (const QString pFirstVidFName,const QString pSecondVidFName,const QString pOutputFileName);

    //-----------------------------------------------------------------------
    //Concatenates two Video Files one after other (Frame by Frame)
    //-----------------------------------------------------------------------
    //Warning:
    //The function expect two video only realtime videos to be concatenated
    //
    //All streams like sound or titles will be ignored.
    //
    //The function expects the full path to the file + filename + extension
    //----------------------------------------------------------------------
    //Example of use:
    //IVR_CatMedia("VideoInput01.mp4" ,"VideoInput02.mp4","VideoOutput.mp4");
    //--------------------------------------------------------------------
    bool IVR_CatVideo  (const QString pFirstVidFName,const QString pSecondVidFName,const QString pOutputFileName);

private:

    void IVR_OpenInpMedia(const QString pFilename);
    void IVR_OpenOutMedia(const QString pFilename);
    void IVR_OpenOutVideo(const QString pFilename);
    void IVR_OpenOutAudio(const QString pFilename);
    void IVR_OpenSndMedia(const QString pFilename);
    void IVR_OpenAuxMedia(const QString pFilename);
    void IVR_FinishOp();

    AVOutputFormat  *IVR_OutputFormat      = NULL;
    AVOutputFormat  *IVR_OutputFormatVideo = NULL;
    AVOutputFormat  *IVR_OutputFormatAudio = NULL;

    AVFormatContext *IVR_InputFmtContext   = NULL;
    AVFormatContext *IVR_AuxilFmtContext   = NULL;
    AVFormatContext *IVR_OutputFmtContext  = NULL;
    AVFormatContext *IVR_SoundFmtContext   = NULL;
    AVFormatContext *IVR_OutputVideoContext= NULL;
    AVFormatContext *IVR_OutputAudioContext= NULL;

    AVPacket IVR_MediaPacket;

    QString IVR_InputFilename;
    QString IVR_OutputFilename;
    QString IVR_SoundFilename;
    QString IVR_AuxilFilename;
    QString IVR_OutputVideoName;
    QString IVR_OutputAudioName;


    int *IVR_StreamsMap      = NULL;
    int  IVR_NumberOfStreams = 0;

    int *IVR_AuxilStreamsMap = NULL;
    int  IVR_NumberOfAuxilStreams = 0;

    int *IVR_SoundStreamsMap      = NULL;
    int  IVR_NumberOfSoundStreams = 0;
};

#endif // CIVRMEDIAHANDLER_H
