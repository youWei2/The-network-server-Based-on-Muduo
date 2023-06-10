#include <mymuduo/TcpServer.h>

#include <mymuduo/AsyncLogging.h>
#include <mymuduo/Logger.h>
#include <mymuduo/Buffer.h>
#include <string>
#include <iostream>

using namespace mymuduo;

off_t kRollSize = 500*1000*1000;

AsyncLogging* g_asyncLog = NULL;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->append(msg, len);
}

class MuduoServer
{

public:
    MuduoServer(EventLoop* loop, 
                const InetAddress& addr,
                const std::string& name)
        : server_(loop, addr, name),
          loop_(loop)
    {
        server_.setConnectionCallback(
            std::bind(&MuduoServer::onConnection, this, std::placeholders::_1));
        server_.setMessageCallback(
            std::bind(&MuduoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)    
        );
        server_.setThreadNum(3);
    }

    void setThreadNum(int threadNum)
    {
        server_.setThreadNum(threadNum);
    }

    void start()
    {
        server_.start();
    }

private:

    void onConnection(const TcpConnectionPtr &conn)
    {
        if(conn->connected())
        {
            LOG_INFO("conn UP : %s", conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("conn DOWN : %s", conn->peerAddress().toIpPort().c_str());
            conn->shutdown();
        }

    }

    void onMessage(const TcpConnectionPtr& conn,
                    Buffer *buffer, 
                    Timestamp time)
    {
        std::string msg = buffer->retrieveAllAsString();
        conn->send(msg);
        // conn->shutdown();
    }

    EventLoop *loop_;
    TcpServer server_;
};

int main(int argc, char** argv)
{

    char name[256] = { '\0' };
    strncpy(name, argv[0], sizeof name - 1);
    AsyncLogging log(::basename(name), kRollSize);
    log.start();
    g_asyncLog = &log;
    Logger::setOutPut(asyncOutput);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 8000);
    MuduoServer server(&loop, addr, "MuduoServer-01");
    server.start();
    loop.loop();

    return 0;
}   