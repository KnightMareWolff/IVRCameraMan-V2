#include "IVR_MediaHandler.h"

CIVRMediaHandler::CIVRMediaHandler()
{

}

void CIVRMediaHandler::IVR_OpenInpMedia(const QString pFilename)
{
    int ret;

    IVR_InputFilename = pFilename;

    if ((ret = avformat_open_input(&IVR_InputFmtContext, IVR_InputFilename.toStdString().c_str() , 0, 0)) < 0)
    {
        qWarning() << QString("Could not open input file: ") + IVR_InputFilename + QString(" ErrorCode: %1 \n").arg(ret);
    }

    if ((ret = avformat_find_stream_info(IVR_InputFmtContext, 0)) < 0)
    {
        qWarning() << QString("Failed to retrieve input stream information - ErrorCode: %1 \n").arg(ret);
    }

    IVR_NumberOfStreams = IVR_InputFmtContext->nb_streams;
    IVR_StreamsMap = (int *)av_mallocz_array(IVR_NumberOfStreams, sizeof(*IVR_StreamsMap));
    if (!IVR_StreamsMap)
    {
        ret = AVERROR(ENOMEM);
        qWarning() << QString("Failed to reserve memory for Streams - ErrorCode: %1 \n").arg(ret);
    }
}

void CIVRMediaHandler::IVR_OpenSndMedia(const QString pFilename)
{
    int ret;

    IVR_SoundFilename = pFilename;

    if ((ret = avformat_open_input(&IVR_SoundFmtContext, IVR_SoundFilename.toStdString().c_str() , 0, 0)) < 0)
    {
        qWarning() << QString("Could not open input file: ") + IVR_SoundFilename + QString(" ErrorCode: %1 \n").arg(ret);
    }

    if ((ret = avformat_find_stream_info(IVR_SoundFmtContext, 0)) < 0)
    {
        qWarning() << QString("Failed to retrieve input stream information - ErrorCode: %1 \n").arg(ret);
    }

    IVR_NumberOfSoundStreams = IVR_SoundFmtContext->nb_streams;
    IVR_SoundStreamsMap = (int *)av_mallocz_array(IVR_NumberOfSoundStreams, sizeof(*IVR_SoundStreamsMap));
    if (!IVR_SoundStreamsMap)
    {
        ret = AVERROR(ENOMEM);
        qWarning() << QString("Failed to reserve memory for Sound Streams - ErrorCode: %1 \n").arg(ret);
    }
}

void CIVRMediaHandler::IVR_OpenAuxMedia(const QString pFilename)
{
    int ret;

    IVR_AuxilFilename = pFilename;

    if ((ret = avformat_open_input(&IVR_AuxilFmtContext, IVR_AuxilFilename.toStdString().c_str() , 0, 0)) < 0)
    {
        qWarning() << QString("Could not open input file: ") + IVR_AuxilFilename + QString(" ErrorCode: %1 \n").arg(ret);
    }

    if ((ret = avformat_find_stream_info(IVR_AuxilFmtContext, 0)) < 0)
    {
        qWarning() << QString("Failed to retrieve input stream information - ErrorCode: %1 \n").arg(ret);
    }

    IVR_NumberOfAuxilStreams = IVR_AuxilFmtContext->nb_streams;
    IVR_AuxilStreamsMap = (int *)av_mallocz_array(IVR_NumberOfAuxilStreams, sizeof(*IVR_AuxilStreamsMap));
    if (!IVR_AuxilStreamsMap)
    {
        ret = AVERROR(ENOMEM);
        qWarning() << QString("Failed to reserve memory for Auxiliary Streams - ErrorCode: %1 \n").arg(ret);
    }
}

void CIVRMediaHandler::IVR_OpenOutMedia(const QString pFilename)
{
    int ret;

    IVR_OutputFilename = pFilename;

    avformat_alloc_output_context2(&IVR_OutputFmtContext, NULL, NULL, IVR_OutputFilename.toStdString().c_str());
    if (!IVR_OutputFmtContext)
    {
        ret = AVERROR_UNKNOWN;
        qWarning() << QString("Failed to create output context - ErrorCode: %1 \n").arg(ret);
    }

    IVR_OutputFormat = IVR_OutputFmtContext->oformat;

}

void CIVRMediaHandler::IVR_FinishOp()
{
    avformat_close_input(&IVR_InputFmtContext);
    avformat_close_input(&IVR_SoundFmtContext);
    avformat_close_input(&IVR_AuxilFmtContext);

    /* close output */
    if (IVR_OutputFmtContext && !(IVR_OutputFormat->flags & AVFMT_NOFILE))
        avio_closep(&IVR_OutputFmtContext->pb);
    avformat_free_context(IVR_OutputFmtContext);

    //free stream map
    av_freep(&IVR_StreamsMap);
    av_freep(&IVR_AuxilStreamsMap);
    av_freep(&IVR_SoundStreamsMap);

}

bool CIVRMediaHandler::IVR_RemuxMedia(const QString pOriFileName, const QString pDstFileName)
{
    int ret;

    //temporary packet for transfer operations(this packet handle the interleaved media frame)
    AVPacket pkt;

    IVR_OpenInpMedia(pOriFileName);
    IVR_OpenOutMedia(pDstFileName);

    //Move the stream data to the destination
    int stream_index = 0;
    for (int i = 0; i < IVR_NumberOfStreams ; i++)
    {
        AVStream *out_stream;
        AVStream *in_stream = IVR_InputFmtContext->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        //Check if is a valid Stream
        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            IVR_StreamsMap[i] = -1;
            continue;
        }

        IVR_StreamsMap[i] = stream_index++;

        //creates a new fresh stream in the output
        out_stream = avformat_new_stream(IVR_OutputFmtContext, NULL);
        if (!out_stream)
        {
            ret = AVERROR_UNKNOWN;
            qWarning() << QString("Failed allocating output stream - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        //copy the codec parameters to the new stream
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0)
        {
            qWarning() << QString("Failed to copy codec parameters - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        out_stream->codecpar->codec_tag = 0;
    }

    //Physically Open the Output for writing
    if (!(IVR_OutputFormat->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&IVR_OutputFmtContext->pb, IVR_OutputFilename.toStdString().c_str() , AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            qWarning() << QString("Could not open output file: ") + IVR_OutputFilename + QString(" ErrorCode: %1 \n").arg(ret);
            return false;
        }
    }

    //Write the media header
    ret = avformat_write_header(IVR_OutputFmtContext, NULL);
    if (ret < 0)
    {
        qWarning() << QString("Error occurred trying write file header - ErrorCode: %1 \n").arg(ret);
        return false;
    }



    //Move and write the media frames into the output
    while (1)
    {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(IVR_InputFmtContext, &pkt);
        if (ret < 0)
            break;

        in_stream  = IVR_InputFmtContext->streams[pkt.stream_index];
        if (pkt.stream_index >= IVR_NumberOfStreams ||
            IVR_StreamsMap[pkt.stream_index] < 0)
        {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = IVR_StreamsMap[pkt.stream_index];
        out_stream       = IVR_OutputFmtContext->streams[pkt.stream_index];

        /* copy packet */
        pkt.pts      = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.dts      = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos      = -1;

        //Write the interleaved frame
        ret = av_interleaved_write_frame(IVR_OutputFmtContext, &pkt);
        if (ret < 0)
        {
            qWarning() << QString("Error muxing packet - ErrorCode: %1 \n").arg(ret);
            break;
        }
        av_packet_unref(&pkt);
    }

    //Write Media Trailler
    av_write_trailer(IVR_OutputFmtContext);

    //Finish the Operation
    IVR_FinishOp();

    return true;
}

bool CIVRMediaHandler::IVR_CatMedia(const QString pFirstVidFName,const QString pSecondVidFName,const QString pOutputFileName)
{
    int ret;

    //temporary packet for transfer operations(this packet handle the interleaved media frame)
    AVPacket pkt;

    IVR_OpenInpMedia(pFirstVidFName);
    IVR_OpenAuxMedia(pSecondVidFName);
    IVR_OpenOutMedia(pOutputFileName);

    //For concatenation both files must have the same number of streams
    if(IVR_NumberOfStreams != IVR_NumberOfAuxilStreams)return false;

    //Create the streams in the output copying the same codec parameters
    int stream_index = 0;
    for (int i = 0; i < IVR_NumberOfStreams ; i++)
    {
        AVStream *out_stream;
        AVStream *in_stream = IVR_InputFmtContext->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        //Check if is a valid Stream
        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            IVR_StreamsMap[i] = -1;
            continue;
        }

        IVR_StreamsMap[i] = stream_index++;

        //creates a new fresh stream in the output
        out_stream = avformat_new_stream(IVR_OutputFmtContext, NULL);
        if (!out_stream)
        {
            ret = AVERROR_UNKNOWN;
            qWarning() << QString("Failed allocating output stream - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        //copy the codec parameters to the new stream
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0)
        {
            qWarning() << QString("Failed to copy codec parameters - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        out_stream->codecpar->codec_tag = 0;
    }

    //Physically Open the Output for writing
    if (!(IVR_OutputFormat->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&IVR_OutputFmtContext->pb, IVR_OutputFilename.toStdString().c_str() , AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            qWarning() << QString("Could not open output file: ") + IVR_OutputFilename + QString(" ErrorCode: %1 \n").arg(ret);
            return false;
        }
    }

    //Write the media header
    ret = avformat_write_header(IVR_OutputFmtContext, NULL);
    if (ret < 0)
    {
        qWarning() << QString("Error occurred trying write file header - ErrorCode: %1 \n").arg(ret);
        return false;
    }

    //Move and write the media frames into the output
    int64_t next_pts = 0;
    int64_t next_dts = 0;
    int64_t actual_pts, actual_dts , actual_duration;
    AVStream *in_stream, *out_stream;
    while (1)
    {
        ret = av_read_frame(IVR_InputFmtContext, &pkt);
        if (ret < 0)
            break;

        in_stream  = IVR_InputFmtContext->streams[pkt.stream_index];
        if (pkt.stream_index >= IVR_NumberOfStreams ||
            IVR_StreamsMap[pkt.stream_index] < 0)
        {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = IVR_StreamsMap[pkt.stream_index];
        out_stream       = IVR_OutputFmtContext->streams[pkt.stream_index];

        actual_pts      = pkt.pts;
        actual_dts      = pkt.dts;
        actual_duration = pkt.duration;

        /* copy packet */
        pkt.pts      = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.dts      = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos      = -1;

        //Write the interleaved frame
        ret = av_interleaved_write_frame(IVR_OutputFmtContext, &pkt);
        if (ret < 0)
        {
            qWarning() << QString("Error muxing packet - ErrorCode: %1 \n").arg(ret);
            break;
        }
        av_packet_unref(&pkt);
    }

    next_dts = actual_dts + (1 * actual_duration);
    next_pts = actual_pts + (1 * actual_duration);


    //Mpve the packets from the auxiliary media from the last point.
    while (1)
    {
        ret = av_read_frame(IVR_AuxilFmtContext, &pkt);
        if (ret < 0)
            break;

        in_stream  = IVR_AuxilFmtContext->streams[pkt.stream_index];
        if (pkt.stream_index >= IVR_NumberOfAuxilStreams ||
            IVR_AuxilStreamsMap[pkt.stream_index] < 0)
        {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = IVR_AuxilStreamsMap[pkt.stream_index];
        out_stream       = IVR_OutputFmtContext->streams[pkt.stream_index];


        pkt.pts = next_pts + pkt.pts;
        pkt.dts = next_dts + pkt.dts;

        /* copy packet */
        pkt.pts      = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.dts      = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos      = -1;

        //Write the interleaved frame
        ret = av_interleaved_write_frame(IVR_OutputFmtContext, &pkt);
        if (ret < 0)
        {
            qWarning() << QString("Error muxing packet - ErrorCode: %1 \n").arg(ret);
            break;
        }
        av_packet_unref(&pkt);
    }

    //Write Media Trailler
    av_write_trailer(IVR_OutputFmtContext);

    //Finish the Operation
    IVR_FinishOp();

    return true;
}

bool CIVRMediaHandler::IVR_MuxMedia(const QString pVideoFileName,const QString pAudioFileName,const QString pOutputFileName)
{
    int ret,audio_ret,video_ret;

    //temporary packet for transfer operations(this packet handle the interleaved media frame)
    AVPacket video_pkt,audio_pkt;

    IVR_OpenInpMedia(pVideoFileName);
    IVR_OpenOutMedia(pOutputFileName);
    IVR_OpenSndMedia(pAudioFileName);

    //To be possible a correct mux operation the Video and Sound Media must have 1 atream each.
    if(IVR_NumberOfStreams > 1 && IVR_NumberOfSoundStreams > 1)
    {
        qWarning() << QString("The Video and Audio Input Media must have just 1 stream. \n");
        return false;
    }

    //Move the Video Media stream data to the destination

    int stream_index = 0;

    for (int i = 0; i < IVR_NumberOfStreams ; i++)
    {
        AVStream *out_stream;
        AVStream *video_stream = IVR_InputFmtContext->streams[i];
        AVCodecParameters *video_codecpar = video_stream->codecpar;

        //Check if is a valid Stream
        if (video_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            video_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            video_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            IVR_StreamsMap[i] = -1;
            continue;
        }

        IVR_StreamsMap[i] = stream_index++;

        //creates a new fresh stream in the output
        out_stream = avformat_new_stream(IVR_OutputFmtContext, NULL);
        if (!out_stream)
        {
            ret = AVERROR_UNKNOWN;
            qWarning() << QString("Failed allocating output stream - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        //copy the codec parameters to the new stream
        ret = avcodec_parameters_copy(out_stream->codecpar, video_codecpar);
        if (ret < 0)
        {
            qWarning() << QString("Failed to copy codec parameters - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        out_stream->codecpar->codec_tag = 0;
    }

    //Move the Audio Media stream data to the destination
    for (int i = 0; i < IVR_NumberOfSoundStreams ; i++)
    {
        AVStream *out_stream;
        AVStream *audio_stream = IVR_SoundFmtContext->streams[i];
        AVCodecParameters *audio_codecpar = audio_stream->codecpar;

        //Check if is a valid Stream
        if (audio_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            audio_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            audio_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            IVR_SoundStreamsMap[i] = -1;
            continue;
        }

        IVR_SoundStreamsMap[i] = stream_index++;

        //creates a new fresh stream in the output
        out_stream = avformat_new_stream(IVR_OutputFmtContext, NULL);
        if (!out_stream)
        {
            ret = AVERROR_UNKNOWN;
            qWarning() << QString("Failed allocating output stream - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        //copy the codec parameters to the new stream
        ret = avcodec_parameters_copy(out_stream->codecpar, audio_codecpar);
        if (ret < 0)
        {
            qWarning() << QString("Failed to copy codec parameters - ErrorCode: %1 \n").arg(ret);
            return false;
        }

        out_stream->codecpar->codec_tag = 0;
    }

    //Physically Open the Output for writing
    if (!(IVR_OutputFormat->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&IVR_OutputFmtContext->pb, IVR_OutputFilename.toStdString().c_str() , AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            qWarning() << QString("Could not open output file: ") + IVR_OutputFilename + QString(" ErrorCode: %1 \n").arg(ret);
            return false;
        }
    }

    //Write the media header
    ret = avformat_write_header(IVR_OutputFmtContext, NULL);
    if (ret < 0)
    {
        qWarning() << QString("Error occurred trying write file header - ErrorCode: %1 \n").arg(ret);
        return false;
    }



    //Move and write the video frames into the output

    while (1)
    {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(IVR_InputFmtContext, &video_pkt);
        if (ret < 0)
            break;

        in_stream  = IVR_InputFmtContext->streams[video_pkt.stream_index];
        if (video_pkt.stream_index >= IVR_NumberOfStreams ||
            IVR_StreamsMap[video_pkt.stream_index] < 0)
        {
            av_packet_unref(&video_pkt);
            continue;
        }

        video_pkt.stream_index = IVR_StreamsMap[video_pkt.stream_index];
        out_stream       = IVR_OutputFmtContext->streams[video_pkt.stream_index];

        // copy packet
        video_pkt.pts      = av_rescale_q_rnd(video_pkt.pts     , in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        video_pkt.dts      = av_rescale_q_rnd(video_pkt.dts     , in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        video_pkt.duration = av_rescale_q    (video_pkt.duration, in_stream->time_base, out_stream->time_base);
        video_pkt.pos      = -1;

        //Write the interleaved frame
        ret = av_interleaved_write_frame(IVR_OutputFmtContext, &video_pkt);
        if (ret < 0)
        {
            qWarning() << QString("Error muxing packet - ErrorCode: %1 \n").arg(ret);
            break;
        }
        av_packet_unref(&video_pkt);
    }


    //Move and write the video frames into the output
    while (1)
    {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(IVR_SoundFmtContext, &audio_pkt);
        if (ret < 0)
            break;

        in_stream  = IVR_SoundFmtContext->streams[audio_pkt.stream_index];
        if (audio_pkt.stream_index >= IVR_NumberOfSoundStreams ||
            IVR_SoundStreamsMap[audio_pkt.stream_index] < 0)
        {
            av_packet_unref(&audio_pkt);
            continue;
        }

        audio_pkt.stream_index = IVR_SoundStreamsMap[audio_pkt.stream_index];
        out_stream             = IVR_OutputFmtContext->streams[audio_pkt.stream_index];

        /* copy packet */
        audio_pkt.pts      = av_rescale_q_rnd(audio_pkt.pts     , in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        audio_pkt.dts      = av_rescale_q_rnd(audio_pkt.dts     , in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF);
        audio_pkt.duration = av_rescale_q    (audio_pkt.duration, in_stream->time_base, out_stream->time_base);
        audio_pkt.pos      = -1;

        //Write the interleaved frame
        ret = av_interleaved_write_frame(IVR_OutputFmtContext, &audio_pkt);
        if (ret < 0)
        {
            qWarning() << QString("Error muxing packet - ErrorCode: %1 \n").arg(ret);
            break;
        }
        av_packet_unref(&audio_pkt);
    }

    //Write Media Trailler
    av_write_trailer(IVR_OutputFmtContext);

    //Finish the Operation
    IVR_FinishOp();

    return true;
}
