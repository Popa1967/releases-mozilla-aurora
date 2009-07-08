//
// Automatically generated by ipdlc.
// Edit at your own risk
//

#ifndef IFrameEmbeddingProtocol_h
#define IFrameEmbeddingProtocol_h

#include "base/basictypes.h"
#include "nscore.h"
#include "IPC/IPCMessageUtils.h"
#include "mozilla/ipc/MessageTypes.h"
#include "mozilla/ipc/ProtocolUtils.h"

namespace IFrameEmbeddingProtocol {


enum State {
    StateStart = 0,
    StateError,
    StateLast
};

enum MessageType {
    IFrameEmbeddingProtocolStart = IFrameEmbeddingProtocolMsgStart << 12,
    IFrameEmbeddingProtocolPreStart = (IFrameEmbeddingProtocolMsgStart << 12) - 1,
    Msg_init__ID,
    Reply_init__ID,
    Msg_loadURL__ID,
    Reply_loadURL__ID,
    Msg_move__ID,
    Reply_move__ID,
    IFrameEmbeddingProtocolEnd
};

class Msg_init :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Msg_init__ID
    };
    Msg_init(const MagicWindowHandle& parentWidget) :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
        IPC::WriteParam(this, parentWidget);
    }

    static bool Read(
                const Message* msg,
                MagicWindowHandle* parentWidget)
    {
        void* iter = 0;

        if (!(IPC::ReadParam(msg, &(iter), parentWidget))) {
            return false;
        }

        return true;
    }
};
class Reply_init :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Reply_init__ID
    };
    Reply_init() :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
    }

    static bool Read(const Message* msg)
    {
        return true;
    }
};
class Msg_loadURL :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Msg_loadURL__ID
    };
    Msg_loadURL(const String& uri) :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
        IPC::WriteParam(this, uri);
    }

    static bool Read(
                const Message* msg,
                String* uri)
    {
        void* iter = 0;

        if (!(IPC::ReadParam(msg, &(iter), uri))) {
            return false;
        }

        return true;
    }
};
class Reply_loadURL :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Reply_loadURL__ID
    };
    Reply_loadURL() :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
    }

    static bool Read(const Message* msg)
    {
        return true;
    }
};
class Msg_move :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Msg_move__ID
    };
    Msg_move(
                const PRUint32& x,
                const PRUint32& y,
                const PRUint32& width,
                const PRUint32& height) :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
        IPC::WriteParam(this, x);
        IPC::WriteParam(this, y);
        IPC::WriteParam(this, width);
        IPC::WriteParam(this, height);
    }

    static bool Read(
                const Message* msg,
                PRUint32* x,
                PRUint32* y,
                PRUint32* width,
                PRUint32* height)
    {
        void* iter = 0;

        if (!(IPC::ReadParam(msg, &(iter), x))) {
            return false;
        }

        if (!(IPC::ReadParam(msg, &(iter), y))) {
            return false;
        }

        if (!(IPC::ReadParam(msg, &(iter), width))) {
            return false;
        }

        if (!(IPC::ReadParam(msg, &(iter), height))) {
            return false;
        }

        return true;
    }
};
class Reply_move :
    public IPC::Message
{
private:
    typedef mozilla::ipc::String String;
    typedef mozilla::ipc::StringArray StringArray;

public:
    enum {
        ID = Reply_move__ID
    };
    Reply_move() :
        IPC::Message(MSG_ROUTING_NONE, ID, PRIORITY_NORMAL)
    {
    }

    static bool Read(const Message* msg)
    {
        return true;
    }
};


} // namespace IFrameEmbeddingProtocol

#endif // ifndef IFrameEmbeddingProtocol_h
