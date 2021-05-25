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
#include <json/json.h>


using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;
void DisplayJsonData(Json::Value);


//----< constructor sets port >--------------------------------------



Cosmetic cosmetic;


Json::Value RetunParseData(std::string str) {

	const auto rawJsonLength = static_cast<int>(str.length());

	JSONCPP_STRING err;
	Json::Value root;

	Json::CharReaderBuilder builder;
	    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	    if (!reader->parse(str.c_str(), str.c_str() + rawJsonLength, &root,
	        &err)) {
	        std::cout << "error" << std::endl;
	        return EXIT_FAILURE;
	    }

	return root;
}

void DisplayJsonData(Json::Value root) {

    try {
        std::cout << "======================================" << std::endl;
        std::cout << "ErrorMessage: " << root["ErrorMessage"].asString() << std::endl;
        std::cout << "TestName: " << root["TestName"].asString() << std::endl;
        std::cout << "LogLevel: " << root["LogLevel"].asString() << std::endl;
        std::cout << "Status: " << root["Status"].asString() << std::endl;
        std::cout << "Duration: " << root["Duration"].asString() << std::endl;
        std::cout << "======================================" << std::endl;
    }
    catch (...) {
        std::cout << "An error has occured\n";
    }
}

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
        //std::string MessageU = msg.toString();
        //std::cout << "\nMsg: " << MessageU << std::endl;

        Json::Value root = RetunParseData(msg.JSON()); // Getting our JSON data parsed

        std::cout << "\n\nRaw: " << msg.JSON() << std::endl; // Displaying our raw json

        DisplayJsonData(root); // Display Our Parse JSON

    
        if (msg.command() == "stop")
        {
            break;
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
    //startTest("LongRun4", LogLevel::Pass_Fail);
    //startTest("LongRun3", LogLevel::Pass_Fail);
    //startTest("LongRun2", LogLevel::Pass_Fail);
    //startTest("LongRun1", LogLevel::Pass_Fail);
    startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    //startTest("Mul: 4*0=4", LogLevel::Pass_Fail);
    //startTest("LongRun4", LogLevel::Pass_Fail);
    startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    startTest("Mul: 4*0=4", LogLevel::Pass_Fail);
    //startTest("Add: 4+0=4", LogLevel::Pass_Fail);
    startTest("Mul: 4*0=4", LogLevel::Pass_Fail);

    stopTest();

    StaticLogger<1>::flush();

    std::cout << "\n  press enter to quit test Harness";
    _getche();

    
    return 0;
}


