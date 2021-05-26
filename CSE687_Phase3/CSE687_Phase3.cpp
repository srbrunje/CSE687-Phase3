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

int main()
{
    SocketSystem ss;

    SUtils::Title("Start Test");

    Utilities::putline();

    StaticLogger<1>::attach(&std::cout);


    // Remove comment below to show extra details
    //StaticLogger<1>::start();

    //start the server
    TestServer testServer = TestServer();
    testServer.StartServer();   
    testServer.SetOutputFile("_output.txt");

    //start the reply socket
    std::thread reply(&TestServer::ProcessReplies, &testServer);
    reply.detach();

    //request a test
    testServer.StartTest("LongRun4", LogLevel::Pass_Fail);
    testServer.StartTest("LongRun3", LogLevel::Pass_Fail_with_error_message);
    testServer.StartTest("LongRun2", LogLevel::Pass_Fail_with_test_duration);
    testServer.StartTest("LongRun1", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testServer.StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    testServer.StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    testServer.StartTest("Div: 4/0=4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testServer.StartTest("LongRun4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    testServer.StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    testServer.StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    testServer.StartTest("Sub: 4-0=4", LogLevel::Pass_Fail);
    testServer.StartTest("Div: 4/0=4", LogLevel::Pass_Fail);
    testServer.StopTest();

    testServer.ReportResults();

    StaticLogger<1>::flush();
    std::cout << "\n  press enter to quit test Harness";
    _getche();

    
    return 0;
}

