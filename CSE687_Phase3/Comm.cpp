#include "Comm.h"

#include "ClientHandler.h"

using namespace MsgPassingCommunication;
using namespace Sockets;

const std::string saveFilePath = "../SaveFiles";
const std::string sendFilePath = "../SendFiles";
const size_t BlockSize = 1024;
Socket::byte rwBuffer[BlockSize];

Receiver::Receiver(EndPoint ep, const std::string& name) : listener(ep.port), rcvrName(name)
{
    StaticLogger<1>::write("\n  -- starting Receiver");
}
//----< returns reference to receive queue >-------------------------

BlockingQueue<Message>* Receiver::queue()
{
    return &rcvQ;
}
//----< starts listener thread running callable object >-------------

template<typename CallableObject>
void Receiver::start(CallableObject& co)
{
    listener.start(co);
}
//----< stops listener thread >--------------------------------------

void Receiver::stop()
{
    listener.stop();
}
//----< retrieves received message >---------------------------------

Message Receiver::getMessage()
{
    StaticLogger<1>::write("\n  -- " + rcvrName + " deQing message");
    return rcvQ.deQ();
}
//----< constructor initializes endpoint object >--------------------

Sender::Sender(const std::string& name) : sndrName(name)
{
    lastEP = EndPoint();  // used to detect change in destination
}
//----< destructor waits for send thread to terminate >--------------

Sender::~Sender()
{
    if (sendThread.joinable())
        sendThread.join();
}
//----< starts send thread deQ, inspect, and send loop >-------------

void Sender::start()
{
    std::function <void()> threadProc = [&]() {
        while (true)
        {
            Message msg = sndQ.deQ();

            if (msg.command() == "quit")
            {
                StaticLogger<1>::write("\n  -- send thread shutting down");
                return;
            }
            StaticLogger<1>::write("\n  -- " + sndrName + " send thread sending " + msg.name());
            std::string msgStr = msg.toString();

            if (msg.to().address != lastEP.address || msg.to().port != lastEP.port)
            {
                connecter.shutDown();
                //connecter.close();
                StaticLogger<1>::write("\n  -- attempting to connect to new endpoint: " + msg.to().toString());
                if (!connect(msg.to()))
                {
                    StaticLogger<1>::write("\n can't connect");
                    continue;
                }
                else
                {
                    StaticLogger<1>::write("\n  connected to " + msg.to().toString());
                }
            }
            if (msg.containsKey("file"))
            {
                sendFile(msg);
            }
            else
            {
                bool sendRslt = connecter.send(msgStr.length(), (Socket::byte*)msgStr.c_str());
            }
        }
    };
    std::thread t(threadProc);
    sendThread = std::move(t);
}
//----< stops send thread by posting quit message >------------------

void Sender::stop()
{
    Message msg;
    msg.name("quit");
    msg.command("quit");
    postMessage(msg);
    connecter.shutDown();
}
//----< attempts to connect to endpoint ep >-------------------------

bool Sender::connect(EndPoint ep)
{
    lastEP = ep;
    return connecter.connect(ep.address, ep.port);
}
//----< posts message to send queue >--------------------------------

void Sender::postMessage(Message msg)
{
    sndQ.enQ(msg);
}
//----< sends binary file >------------------------------------------
/*
*  - not implemented yet
*/
bool Sender::sendFile(Message msg)
{
    if (!msg.containsKey("file"))
        return false;
    std::string fileSpec = sendFilePath + "/" + msg.file();
    std::ifstream sendFile(fileSpec, std::ios::binary);
    if (!sendFile.good())
        return false;
    while (true)
    {
        sendFile.read(rwBuffer, BlockSize);
        size_t blockSize = (size_t)sendFile.gcount();
        msg.contentLength(blockSize);
        std::string msgString = msg.toString();
        connecter.sendString(msgString);
        if (blockSize == 0)
            break;
        connecter.send(blockSize, rwBuffer);
    }
    sendFile.close();
    return true;
}



Comm::Comm(EndPoint ep, const std::string& name) : rcvr(ep, name), sndr(name), commName(name) {}

void Comm::start()
{
    BlockingQueue<Message>* pQ = rcvr.queue();
    ClientHandler* pCh = new ClientHandler(pQ, commName);
    /*
      There is a trivial memory leak here.
      This ClientHandler is a prototype used to make ClientHandler copies for each connection.
      Those are not created on the heap, and are destroyed when the connection closes.
      Only one Client handler prototype is created for each Comm object and will live until
      the end of the program.

      I will clean this up in the next version.
    */
    rcvr.start(*pCh);
    sndr.start();
}

void Comm::stop()
{
    rcvr.stop();
    sndr.stop();
}

void Comm::postMessage(Message msg)
{
    sndr.postMessage(msg);
}

Message Comm::getMessage()
{
    return rcvr.getMessage();
}

std::string Comm::name()
{
    return commName;
}
