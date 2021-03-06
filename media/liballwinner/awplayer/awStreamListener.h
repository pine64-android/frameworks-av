#ifndef AW_STREAM_LISTENER_H
#define AW_STREAM_LISTENER_H

#include <media/IStreamSource.h>
#include <media/stagefright/foundation/ALooper.h>
#include <binder/MemoryDealer.h>

using namespace android;

#define kNumBuffers (16)
#define kBufferSize (4*1024)

struct awStreamListener : public BnStreamListener
{
    awStreamListener(const sp<IStreamSource> &source, ALooper::handler_id targetID, size_t numBuffers = kNumBuffers, size_t bufferSize = kBufferSize);

    virtual void queueBuffer(size_t index, size_t size);
    virtual void issueCommand(Command cmd, bool synchronous, const sp<AMessage> &extra);

    void start();
    void stop();
    void clearBuffer();

    ssize_t read(void *data, size_t size, sp<AMessage> *extra);
    ssize_t copy(void *data, size_t size, sp<AMessage> *extra);
    size_t getCachedSize();
    size_t getCacheCapacity(){return mNumBuffers*mBufferSize;};
    bool isReceiveEOS();

private:
    struct QueueEntry
    {
        bool mIsCommand;

        size_t mIndex;
        size_t mSize;
        size_t mOffset;

        enum Command mCommand;
        sp<AMessage> mExtra;
    };

    Mutex mLock;

    sp<IStreamSource>    mSource;
    ALooper::handler_id  mTargetID;
    sp<MemoryDealer>     mMemoryDealer;
    Vector<sp<IMemory> > mBuffers;
    List<QueueEntry>     mQueue;
    bool                 mEOS;
    bool                 mSendDataNotification;
    bool                 mReceiveEOS;
    size_t                  mNumBuffers;
    size_t                  mBufferSize;
    size_t				 mAvailableSize;

    DISALLOW_EVIL_CONSTRUCTORS(awStreamListener);
};

#endif // AW_STREAM_LISTENER_H

