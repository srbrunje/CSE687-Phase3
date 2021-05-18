#include "TestServer.h"

#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>

TestServer::TestServer()
{

    std::cout << "create test server class\n";
    bool shutdown = false;

    //create the tests
    mgr = TestManager::getInstance();
}


/// <summary>
/// setup all of the tests
/// </summary>
void TestServer::InitilizeTestServer()
{
    
    //mgr.SetOutputFile("output.txt"); // Define your output here, either relative or absolute
    mgr->SetOutputToStream(true);


    // Math with integers
    AddFunctor<int> iAdd(4, 0, 4); // should PASS 4 + 0 == 4
    SubFunctor<int> iSub(4, 0, 4); // should PASS 4 - 0 == 4
    MulFunctor<int> iMul(4, 0, 4); // should FAIL 4 * 0 != 4
    DivFunctor<int> iDiv(4, 0, 4); // should FAIL with divide by zero exception

    LongRunFunctor lRun(1, 200);

    mgr->CreateTest(iAdd, LogLevel::Pass_Fail, "Add: 4+0=4", "Error: this ouput should have printed out 4");
    mgr->CreateTest(iSub, LogLevel::Pass_Fail_with_error_message, "Sub: 4-0=4");
    mgr->CreateTest(iMul, LogLevel::Pass_Fail_with_error_message_and_test_duration, "Mul: 4*0=4", "Custom error, idk what went wrong to be honest");
    mgr->CreateTest(iDiv, LogLevel::Pass_Fail_with_error_message_and_test_duration, "Div: 4/0=4");

    mgr->CreateTest(lRun, LogLevel::Pass_Fail_with_error_message, "LongRun");
}


void runTest(TestManager* mgr, int testNumber)
{
    mgr->runTest(testNumber);
}


void ListenerThread(TestManager* mgr)
{
    EndPoint serverEP("localhost", 9890);
   

    Comm comm(serverEP, "Server Listener");

    comm.start();

    Message msg, rply;
    rply.name("reply");
    size_t count = 0;
    while (true)
    {
        // display each incoming message

        msg = comm.getMessage();
        std::cout << "\n  " + comm.name() + " received Test Request: " << msg.name();

        //get the test number
        int testNumber = mgr->FindTestNumber(msg.name());

        if (testNumber >= 0)
        {
            //set the new log level
           

           //run the test
           std::thread t1(runTest, mgr, testNumber);
           t1.detach();

           //mgr->runTest(testNumber);

        }

        if (msg.command() == "stop")
        {
            break;
        }

    }

    comm.stop();

}

void TestServer::StartServer()
{
 
    //initilize the tests
    InitilizeTestServer();

    //start the listner thread
    std::thread testServer(ListenerThread,mgr);
    testServer.detach();   
    
}

void TestServer::StopServer()
{
}

