#include "IVR_TaskQueue.h"

CIVRTaskQueue::CIVRTaskQueue()
{
    IVR_BufferQueue = new CIVRQueue();
}

CIVRTaskQueue::~CIVRTaskQueue()
{

}

//---------------------------------------------------------
//Record Buffer Related Tasks
//---------------------------------------------------------
void CIVRTaskQueue::Push_Bf_Task(const IVR_Task& task)
{
    unique_lock<mutex> lock(IVR_BufferMutex);

    IVR_BufferQueue->Enqueue(new CIVRQueueNode(task.IVR_TaskType,
                                               task.IVR_CameraId,
                                               task.IVR_CamType,
                                               task.IVR_FrameBuffer));
    lock.unlock();

    IVR_BFQueueCondition.notify_one();
}


void CIVRTaskQueue::Push_FrameBuffer(IVR_RenderBuffer &pIVR_FrameBuffer)
{
    unique_lock<mutex> lock(IVR_BufferMutex);

    IVR_BufferQueue->Enqueue(pIVR_FrameBuffer);
    lock.unlock();

    IVR_BFQueueCondition.notify_one();
}
//---------------------------------------------------------
//Pop a Buffer Task and Go...
//---------------------------------------------------------
IVR_Task CIVRTaskQueue::Pop_Bf_Task()
{
    IVR_Task buff;
    lock_guard<mutex> lock(IVR_BufferMutex);
    if (!IVR_BufferQueue->QueueEmpty())
    {
        CIVRQueueNode* ret = IVR_BufferQueue->Dequeue();
        if(ret)
        {
            buff.IVR_TaskType    = ret->IVR_TaskType;
            buff.IVR_CameraId    = ret->IVR_CameraId;
            buff.IVR_CamType     = ret->IVR_CamType;
            //Its possible we have a buffer without a valid buffer?
            if(ret->IVR_FrameBuffer.IVR_IsValid)
            {
               buff.IVR_FrameBuffer = ret->IVR_FrameBuffer;
            }
        }
    }
    return buff;
}

CIVRQueue::CIVRQueue()
{
    iIVR_QueueSize = 0;
    iIVR_PreviousSize=0;
    pIVR_QueueBegin = new CIVRQueueNode();
    pIVR_QueueEnd   = new CIVRQueueNode();

    pIVR_QueueBegin->pIVR_NextNode  = pIVR_QueueEnd;
    pIVR_QueueBegin->pIVR_PriorNode = NULL;
    pIVR_QueueBegin->IVR_Timestamp  = 00000000000000000;
    pIVR_QueueBegin->IVR_FrameBuffer.IVR_FrameID = 000000000;

    pIVR_QueueEnd->pIVR_PriorNode   = pIVR_QueueBegin;
    pIVR_QueueEnd->pIVR_NextNode    = NULL;
    pIVR_QueueEnd->IVR_Timestamp    = 99999999999999999;
    pIVR_QueueEnd->IVR_FrameBuffer.IVR_FrameID = 999999999;

    iIVR_Counter=0;
    iIVR_Max = 0;
    iIVR_Min = 0;
}


void CIVRQueue::Enqueue(CIVRQueueNode * no)
{
    CIVRQueueNode *novo = no;

    novo->IVR_FrameBuffer.IVR_FrameID = iIVR_Counter;
    /*******************************************************************/
    /*The Nodes are enqueued acoordingly with your priority            */
    /*in this case how much greater is the cost...less is your priority*/
    /*******************************************************************/
    CIVRQueueNode *temp = pIVR_QueueBegin;

    //we cannot have a empty begin node AND ENQUEUE NODE...
    if (temp == NULL || no == NULL)return;

    //Just check
    if(temp->pIVR_NextNode!=NULL)
    {
       while ((no->IVR_FrameBuffer.IVR_FrameID >= temp->IVR_FrameBuffer.IVR_FrameID) && temp != pIVR_QueueEnd)
       {
          temp = temp->pIVR_NextNode;
       }
    }

    /****************************************/
    /*Insert the node in the prior position */
    /****************************************/
    novo->pIVR_PriorNode = temp->pIVR_PriorNode;
    novo->pIVR_NextNode  = temp;
    temp->pIVR_PriorNode = novo;
    novo->pIVR_PriorNode->pIVR_NextNode = novo;

    iIVR_PreviousSize = iIVR_QueueSize;
    iIVR_QueueSize++;
    iIVR_Counter++;
    /******************************/
    /*Update the Feeding state    */
    /******************************/
    if(iIVR_QueueSize > 10 && (iIVR_PreviousSize < iIVR_QueueSize))
    {
       bIVR_Feeding = true;
       bIVR_Dumping = false;
    }
}

void CIVRQueue::Enqueue(IVR_RenderBuffer &pIVR_FrameBuffer)
{
    CIVRQueueNode *novo = new CIVRQueueNode(pIVR_FrameBuffer);

    novo->IVR_FrameBuffer.IVR_FrameID = iIVR_Counter;
    /*******************************************************************/
    /*The Nodes are enqueued acoordingly with your priority            */
    /*in this case how much greater is the cost...less is your priority*/
    /*******************************************************************/
    CIVRQueueNode *temp = pIVR_QueueBegin;

    //we cannot have a empty begin node AND ENQUEUE NODE...
    if (temp == nullptr || novo == nullptr)return;

    //Just check
    if(temp->pIVR_NextNode!=NULL)
    {
       while ((novo->IVR_FrameBuffer.IVR_FrameID >= temp->IVR_FrameBuffer.IVR_FrameID) && temp != pIVR_QueueEnd)
       {
          temp = temp->pIVR_NextNode;
       }
    }

    /****************************************/
    /*Insert the node in the prior position */
    /****************************************/
    novo->pIVR_PriorNode = temp->pIVR_PriorNode;
    novo->pIVR_NextNode  = temp;
    temp->pIVR_PriorNode = novo;
    novo->pIVR_PriorNode->pIVR_NextNode = novo;

    iIVR_PreviousSize = iIVR_QueueSize;
    iIVR_QueueSize++;
    iIVR_Counter++;
    /******************************/
    /*Update the Feeding state    */
    /******************************/
    if(iIVR_QueueSize > 10 && (iIVR_PreviousSize < iIVR_QueueSize))
    {
       bIVR_Feeding = true;
       bIVR_Dumping = false;
    }
}

void CIVRQueue::EnqueueMax(CIVRQueueNode * no)
{
    CIVRQueueNode *novo = no;

    //Caso o score seja mais baixo que o minimo
    if(novo->IVR_Timestamp < iIVR_Min)
    {
        return;
    }
    //No caso de chegarmos no balanço maximo do Bag...
    if((novo->IVR_Timestamp >= iIVR_Min && iIVR_QueueSize == iIVR_MaxSize))
    {
        DequeueBack();
    }
    /*******************************************************************/
    /*The Nodes are enqueued acoordingly with your priority            */
    /*in this case how much greater is the cost...less is your priority*/
    /*******************************************************************/
    CIVRQueueNode *temp = pIVR_QueueBegin;

    //we cannot have a empty begin node AND ENQUEUE NODE...
    if (temp == NULL || no == NULL)return;

    //Just check
    if(temp->pIVR_NextNode!=NULL)
    {
       while ((no->IVR_Timestamp < temp->IVR_Timestamp) && temp != pIVR_QueueEnd)
       {
          temp = temp->pIVR_NextNode;
       }
    }

    /****************************************/
    /*Insert the node in the prior position */
    /****************************************/
    novo->pIVR_PriorNode = temp->pIVR_PriorNode;
    novo->pIVR_NextNode  = temp;
    temp->pIVR_PriorNode = novo;
    novo->pIVR_PriorNode->pIVR_NextNode = novo;


    iIVR_QueueSize++;

    /******************************/
    /*Update the Min/Max Values   */
    /******************************/
    if(iIVR_QueueSize==1)
    {
        iIVR_Max = pIVR_QueueBegin->pIVR_NextNode->IVR_Timestamp;
        iIVR_Min = 0;
    }
    else
    {
       iIVR_Max = pIVR_QueueBegin->pIVR_NextNode ->IVR_Timestamp;
       iIVR_Min = pIVR_QueueEnd  ->pIVR_PriorNode->IVR_Timestamp;
    }
}

CIVRQueueNode * CIVRQueue::Dequeue()
{
    /******************************************/
    /*we are trying remove from the beggining.*/
    /*a empty queue , is that means we are not*/
    /*dunmping anymore...                     */
    /******************************************/
    if (iIVR_QueueSize <= 0)
    {
        bIVR_Feeding = false;
        bIVR_Dumping = false;
        return nullptr;
    }

    CIVRQueueNode *temp = new CIVRQueueNode();

    pIVR_QueueBegin->pIVR_NextNode->pIVR_NextNode->pIVR_PriorNode = pIVR_QueueBegin;

    temp = pIVR_QueueBegin->pIVR_NextNode;

    pIVR_QueueBegin->pIVR_NextNode = pIVR_QueueBegin->pIVR_NextNode->pIVR_NextNode;

    //decrease the queue sizes
    iIVR_PreviousSize = iIVR_QueueSize;
    iIVR_QueueSize--;

    /******************************/
    /*Update the Feeding state    */
    /******************************/
    if(iIVR_QueueSize < 10 && (iIVR_PreviousSize > iIVR_QueueSize))
    {
       bIVR_Feeding = false;
       bIVR_Dumping = true;
    }

    return temp;
}

CIVRQueueNode * CIVRQueue::DequeueBack()
{
    CIVRQueueNode *temp = new CIVRQueueNode();
    /************************************/
    /*we allways remove from the end....*/
    /************************************/
    if (iIVR_QueueSize > 0)
    {
        pIVR_QueueEnd->pIVR_PriorNode->pIVR_PriorNode->pIVR_NextNode = pIVR_QueueEnd;

        temp = pIVR_QueueEnd->pIVR_PriorNode;

        pIVR_QueueEnd->pIVR_PriorNode = pIVR_QueueEnd->pIVR_PriorNode->pIVR_PriorNode;
    }

    /******************************/
    /*Update the Min/Max Values   */
    /******************************/
    iIVR_Max = (uint)pIVR_QueueBegin->pIVR_NextNode->IVR_Timestamp;
    iIVR_Min = (uint)pIVR_QueueEnd->pIVR_PriorNode->IVR_Timestamp;

    iIVR_QueueSize--;

    return temp;
}

void CIVRQueue::Remove(CIVRQueueNode* no)
{
    /*************************************/
    /*Exclude without criteria...        */
    /*Atenção! verificar caso nao exista */
    /*************************************/
    CIVRQueueNode *temp = pIVR_QueueBegin;
    while (no->IVR_Timestamp != temp->IVR_Timestamp)
    {
        temp = temp->pIVR_NextNode;
    }

    temp->pIVR_NextNode->pIVR_PriorNode = temp->pIVR_PriorNode;
    temp->pIVR_PriorNode->pIVR_NextNode = temp->pIVR_NextNode;

    /******************************/
    /*Update the Min/Max Values   */
    /******************************/
    iIVR_Max = (uint)pIVR_QueueBegin->pIVR_NextNode->IVR_Timestamp;
    iIVR_Min = (uint)pIVR_QueueEnd->pIVR_PriorNode->IVR_Timestamp;

    iIVR_QueueSize--;

    delete temp;
}

/**************************************/
/*check if the node exists            */
/**************************************/
bool CIVRQueue::Contain(CIVRQueueNode* no) const
{
    CIVRQueueNode *temp = pIVR_QueueBegin;

    while (temp != pIVR_QueueEnd)
    {
        if (no->IVR_Timestamp == temp->IVR_Timestamp)
            return true;
        temp = temp->pIVR_NextNode;
    }
    return false;
}

/**************************************/
/*Clean-Up the Queue                  */
/**************************************/
void CIVRQueue::QueueClean()
{
    while (!QueueEmpty())
    {
        Dequeue();
    }

    iIVR_Max = 0;
    iIVR_Min = 0;

}
