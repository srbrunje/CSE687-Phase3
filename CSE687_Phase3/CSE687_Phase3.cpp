// CSE687_Phase3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                 //
// ver 2.0                                                         //
// Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////

#include "Comm.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>
#include "TestManager.h"
#include "ExampleTest.h"
#include "ClientHandler.h"
#include "TestServer.h"

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;



//----< constructor sets port >--------------------------------------



Cosmetic cosmetic;



//the reply process thread
void ProcessReplies()
{
    EndPoint serverEP("localhost", 9893);
  //  SocketSystem ss;

    Comm comm(serverEP, "Client Status");

    comm.start();

    Message msg, rply;
    rply.name("reply");
    size_t count = 0;
    while (true)
    {
        // display each incoming message

        msg = comm.getMessage();
        std::cout << "\n  " + comm.name() + " received Test Status: " << msg.name();

        if (msg.containsKey("file"))  // is this a file message?
        {
            if (msg.contentLength() == 0)
                std::cout << "\n  " + comm.name() + " received file \"" + msg.file() + "\" from " + msg.name();
        }
        else  // non-file message
        {          

            if (msg.command() == "stop")
            {
                break;
            }
        }
    }

    comm.stop();
}




void startTest(std::string testName, LogLevel logLevel)
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();
    EndPoint serverEP("localhost", 9890);
    EndPoint clientEP("localhost", 9891);

    // create the message
    Message testRequest(serverEP, clientEP);
    testRequest.name(testName); //the name of the test to run
    
    //the log level
    testRequest.logLevel(logLevel);

    //set status
    comm.postMessage(testRequest);

    comm.stop();

}

void stopTest()
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();
    EndPoint serverEP("localhost", 9890);
    EndPoint clientEP("localhost", 9891);

    // create the message
    Message testRequest(serverEP, clientEP);
    testRequest.name("stop"); //the name of the test to run

    //the log level
    testRequest.command("stop");

    //set status
    comm.postMessage(testRequest);

    comm.stop();
}

int main()
{
    SocketSystem ss;

    SUtils::Title("Start Test");

    Utilities::putline();

    StaticLogger<1>::attach(&std::cout);

    ///////////////////////////////////////////////////////////////////
    // remove comment on line below to show many of the gory details
    //
   // StaticLogger<1>::start();

    //start the server
    TestServer testServer = TestServer();
    testServer.StartServer();   
    

    //start the reply socket
    std::thread reply(ProcessReplies);
    reply.detach();

    //request a test
    startTest("LongRun4", LogLevel::Pass_Fail);
    startTest("LongRun3", LogLevel::Pass_Fail);
    startTest("LongRun2", LogLevel::Pass_Fail);
    startTest("LongRun1", LogLevel::Pass_Fail);
    startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    startTest("Mul: 4*0=4", LogLevel::Pass_Fail);
    startTest("LongRun4", LogLevel::Pass_Fail);
    startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    startTest("Mul: 4*0=4", LogLevel::Pass_Fail);
    startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    startTest("Mul: 4*0=4", LogLevel::Pass_Fail);

    stopTest();

    StaticLogger<1>::flush();
    std::cout << "\n  press enter to quit test Harness";
    _getche();

    
    return 0;
}

