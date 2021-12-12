#ifndef CIVRTASKQUEUE_H
#define CIVRTASKQUEUE_H

#include "IVRLowLevelSDK_global.h"
#include "IVR_Frame.h"

class IVRLOWLEVELSDK_EXPORT CIVRQueueNode
{
public:
    CIVRQueueNode()
    {
        IVR_TaskType   =99999;
        IVR_CameraId   =99999;
        IVR_CamType    =99999;
        IVR_Timestamp  =99999;
        pIVR_NextNode  = pIVR_PriorNode = NULL;
    }

    ~CIVRQueueNode()
    {
        IVR_TaskType   =99999;
        IVR_CameraId   =99999;
        IVR_CamType    =99999;
        IVR_Timestamp  =99999;
        pIVR_NextNode  = pIVR_PriorNode = NULL;
    }

    CIVRQueueNode( const CIVRQueueNode &obj)
    {
        IVR_TaskType    = obj.IVR_TaskType;
        IVR_CameraId    = obj.IVR_CameraId;
        IVR_CamType     = obj.IVR_CamType;
        IVR_Timestamp   = obj.IVR_Timestamp;
        IVR_FrameBuffer = obj.IVR_FrameBuffer;
    }

    CIVRQueueNode(uint              pIVR_TaskType   , //0-BuffTask 1-TakeTask
                  uint              pIVR_CameraId   ,
                  uint              pIVR_CamType    ,
                  const CIVRFrame  &pIVR_FrameBuffer)
    {
        IVR_TaskType    = pIVR_TaskType;
        IVR_CameraId    = pIVR_CameraId;
        IVR_CamType     = pIVR_CamType;

        if(pIVR_FrameBuffer.IVR_IsValid)
        {
            IVR_Timestamp   = pIVR_FrameBuffer.IVR_Timestamp;
        }
        else
        {
            IVR_Timestamp   = QDateTime::currentMSecsSinceEpoch();
        }

        if(pIVR_FrameBuffer.IVR_IsValid)IVR_FrameBuffer = pIVR_FrameBuffer;

        pIVR_NextNode = pIVR_PriorNode = 0;
    }

    CIVRQueueNode(const CIVRFrame  &pIVR_FrameBuffer)
    {
        IVR_TaskType    = IVR_Task_Type_Buffer;
        IVR_CameraId    = pIVR_FrameBuffer.IVR_CameraID;
        IVR_CamType     = pIVR_FrameBuffer.IVR_CameraType;

        if(pIVR_FrameBuffer.IVR_IsValid)
        {
            IVR_Timestamp   = pIVR_FrameBuffer.IVR_Timestamp;
        }
        else
        {
            IVR_Timestamp   = QDateTime::currentMSecsSinceEpoch();
            qWarning() << IVR_Timestamp;
        }

        if(pIVR_FrameBuffer.IVR_IsValid)IVR_FrameBuffer = pIVR_FrameBuffer;

        pIVR_NextNode = pIVR_PriorNode = 0;
    }

    CIVRQueueNode(const IVR_RenderBuffer &pIVR_FrameBuffer)
    {
        IVR_TaskType    = IVR_Task_Type_Buffer;
        IVR_CameraId    = pIVR_FrameBuffer.IVR_CameraID;
        IVR_CamType     = pIVR_FrameBuffer.IVR_CameraType;

        if(pIVR_FrameBuffer.IVR_IsValid)
        {
            IVR_Timestamp   = pIVR_FrameBuffer.IVR_Timestamp;
        }
        else
        {
            IVR_Timestamp   = QDateTime::currentMSecsSinceEpoch();
            qWarning() << IVR_Timestamp;
        }

        if(pIVR_FrameBuffer.IVR_IsValid)IVR_FrameBuffer = pIVR_FrameBuffer;

        pIVR_NextNode = pIVR_PriorNode = 0;
    }

    uint              IVR_TaskType   =99999;
    uint              IVR_CameraId   =99999;
    uint              IVR_CamType    =99999;
    qint64            IVR_Timestamp  =99999;
    CIVRFrame         IVR_FrameBuffer;
    CIVRQueueNode   *pIVR_NextNode;
    CIVRQueueNode   *pIVR_PriorNode;
};

class IVRLOWLEVELSDK_EXPORT CIVRQueue
{
public:
      CIVRQueue();
     ~CIVRQueue()
     {
         QueueClean();
         delete pIVR_QueueBegin;
         delete pIVR_QueueEnd;
     }

    void                   QueueClean ();
    bool                   QueueEmpty () { return iIVR_QueueSize == 0; }
    bool                   ResetCounter(){ iIVR_Counter=0; return true;}
    void                   SetMaxSize (uint pIVR_MaxSize){iIVR_MaxSize = pIVR_MaxSize;}
    void                   Enqueue    (CIVRQueueNode *no);
    void                   Enqueue    (IVR_RenderBuffer &pIVR_FrameBuffer);
    void                   EnqueueMax (CIVRQueueNode *no);
    CIVRQueueNode *        Dequeue    ();
    CIVRQueueNode *        DequeueBack();
    void                   Remove     (CIVRQueueNode *no);
    bool                   Contain    (CIVRQueueNode *no) const;

    //States that make possible "retain" a certain portion os frames before atart process it in the Loop.
    bool                   isFeeding(){return bIVR_Feeding;}
    bool                   isDumping(){return bIVR_Dumping;}

private:

    qint64           iIVR_QueueSize;
    qint64           iIVR_PreviousSize;
    qint64           iIVR_MaxSize;
    qint64           iIVR_Counter;
    qint64           iIVR_Max;
    qint64           iIVR_Min;
    bool             bIVR_Feeding;
    bool             bIVR_Dumping;
    CIVRQueueNode *pIVR_QueueBegin;
    CIVRQueueNode *pIVR_QueueEnd;
};

class IVRLOWLEVELSDK_EXPORT IVR_Task
{
public:
    IVR_Task()
    {
        IVR_TaskType   =99999;
        IVR_CameraId   =99999;
        IVR_CamType    =99999;
        IVR_FrameBuffer=CIVRFrame();
    }

    ~IVR_Task()
    {
        IVR_TaskType   =99999;
        IVR_CameraId   =99999;
        IVR_CamType    =99999;
    }

    IVR_Task( const CIVRQueueNode &obj)
    {
        IVR_TaskType    = obj.IVR_TaskType;
        IVR_CameraId    = obj.IVR_CameraId;
        IVR_CamType     = obj.IVR_CamType;
        IVR_FrameBuffer = obj.IVR_FrameBuffer;
    }

    IVR_Task& operator = (const IVR_Task &t)
    {
        IVR_TaskType    = t.IVR_TaskType;
        IVR_CameraId    = t.IVR_CameraId;
        IVR_CamType     = t.IVR_CamType;
        IVR_FrameBuffer = t.IVR_FrameBuffer;
        IVR_RootFolder  = t.IVR_RootFolder;
        return *this;
    }

    IVR_Task(uint              pIVR_TaskType   , //0-BuffTask 1-TakeTask
             uint              pIVR_CameraId   ,
             uint              pIVR_CamType    ,
             QString           pIVR_RootFolder ,
             const CIVRFrame  &pIVR_FrameBuffer)
    {
        IVR_TaskType    = pIVR_TaskType;
        IVR_CameraId    = pIVR_CameraId;
        IVR_CamType     = pIVR_CamType;
        IVR_FrameBuffer = pIVR_FrameBuffer;
        IVR_RootFolder  = pIVR_RootFolder;
    }

    void ClearTask()
    {
        IVR_TaskType   =99999;
        IVR_CameraId   =99999;
        IVR_CamType    =99999;
        IVR_FrameBuffer=CIVRFrame();
        IVR_RootFolder   = ""   ;
    }

    uint             IVR_TaskType     = 99999;
    uint             IVR_CameraId     = 99999;
    uint             IVR_CamType      = 99999;
    CIVRFrame        IVR_FrameBuffer         ;
    QString          IVR_RootFolder   = ""   ;
};

class  IVRLOWLEVELSDK_EXPORT CIVRTaskQueue
{

public:
    CIVRTaskQueue();
   ~CIVRTaskQueue();

    CIVRQueue*                IVR_BufferQueue;
    mutex                     IVR_BufferMutex;
    condition_variable        IVR_BFQueueCondition;

    //---------------------------------------------------------
    //Record Buffer Related Tasks
    //---------------------------------------------------------
    void Push_Bf_Task(const IVR_Task& task);
    //---------------------------------------------------------
    //Record Buffer Related Tasks
    //---------------------------------------------------------
    void Push_FrameBuffer(IVR_RenderBuffer &pIVR_FrameBuffer);
    //---------------------------------------------------------
    //---------------------------------------------------------
    //Pop a Buffer Task and Go...
    //---------------------------------------------------------
    IVR_Task Pop_Bf_Task();

    //---------------------------------------------------------
    //Just an example to make the Pop wait a new message to start
    //pop again..
    //---------------------------------------------------------
    //BuffTask Pop_Bf_Task()
    //{
    //    unique_lock<mutex> lock(buffer_mutex_);
    //    while (buffertasks_.empty())
    //    {
    //        buffercond_.wait(lock);
    //    }
    //    BuffTask ret = buffertasks_.front();
    //    buffertasks_.pop_front();
    //
    //    return ret;
    //}
    //---------------------------------------------------------
    //Just an example to make the Pop anyway and send the
    //return..
    //---------------------------------------------------------
    //TakeTask Try_Pop_Tk_Task()
    //{
    //    TakeTask ret;
    //    lock_guard<mutex> lock(record_mutex_);
    //    if (!recordtasks_.empty())
    //    {
    //        ret = recordtasks_.front();
    //        recordtasks_.pop_front();
    //    }
    //    return ret;
    //}
    //---------------------------------------------------------
};

#endif // CIVRTASKQUEUE_H
