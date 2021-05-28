#include "TestClient.h"

using namespace MsgPassingCommunication;

TestClient::TestClient(const std::string clientName, const EndPoint::Port clientPort)
{
    std::cout << "create test client class\n";    
    _numMsgsSent = 0;
    _myName = clientName;
    _myPort = clientPort;

    _clientEP = EndPoint("localhost", _myPort);

    _serverEP = EndPoint("localhost", 9890);

    StartListenerThread();
}

bool TestClient::SetOutputFile(const std::string& aFilePath)
{
    return false;
}

void TestClient::SetOutputToFile(const bool bOutput)
{
}

void TestClient::SetOutputStream(std::ostream& aStream)
{
}

void TestClient::SetOutputToStream(const bool bOutput)
{
}

void TestClient::StartTests()
{
    StartTest("LongRun4", LogLevel::Pass_Fail);
    StartTest("LongRun3", LogLevel::Pass_Fail_with_error_message);
    StartTest("LongRun2", LogLevel::Pass_Fail_with_test_duration);
    StartTest("LongRun1", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    StartTest("Div: 4/0=4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    StartTest("LongRun4", LogLevel::Pass_Fail_with_error_message_and_test_duration);
    StartTest("Add: 4+0=4", LogLevel::Pass_Fail_with_error_message);
    StartTest("Mul: 4*0=4", LogLevel::Pass_Fail_with_test_duration);
    StartTest("Sub: 4-0=4", LogLevel::Pass_Fail);
    StartTest("Div: 4/0=4", LogLevel::Pass_Fail);
    StopTest();
}

void TestClient::StartTest(const std::string& aTestName, const LogLevel aLogLevel)
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Request");
    comm.start();

    // create the message
    Message testRequest(_serverEP, _clientEP);
    testRequest.SetName(aTestName);
    testRequest.SetValue("logLevel", (int)aLogLevel);

    testRequest.SetAuthor(_myName);

    // send the message
    _numMsgsSent++;
    comm.postMessage(testRequest);
    comm.stop();
}

void TestClient::StopTest()
{
    //create the comm connection
    Comm comm(EndPoint("localhost", 9891), "Send Test Stop Request");
    comm.start();    

    // create the message
    Message testRequest(_serverEP, _clientEP);
    testRequest.SetName("stop"); //the name of the test to run
    testRequest.SetCommand("stop");
    testRequest.SetAuthor(_myName);

    // send the message
    comm.postMessage(testRequest);
    comm.stop();
}

void TestClient::StartListenerThread()
{
    std::thread reply(&TestClient::ProcessReplies, this);
    reply.detach();
}

void TestClient::ProcessReplies()
{
    EndPoint listenerServerEP("localhost", 9893);
    Comm comm(listenerServerEP, "Client Status");
    comm.start();

    Message msg, rply;
    rply.SetName("reply");
    size_t count = 0;
    while (true)
    {
        // display each incoming message, and track the number
        msg = comm.getMessage();
        std::cout << "\n" + comm.name() + " received Test Result: " << msg.GetName();

        if (msg.GetCommand() == "stop") {
            break;
        }

        _msgsRcvd.enQ(msg);
    }
    comm.stop();
}

void TestClient::ReportResults()
{
    // Wait until all replies have been heard - or a max waiting time is up
    timing::hack beginWait = timing::now();
    while (_msgsRcvd.size() < _numMsgsSent) {
        if (timing::duration_us(beginWait) > 10000000) { // 10s wait limit
            break;
        }
    }

    int i = 1;
    _lgr.ClearContents(); // erase any old data stored in the output file (if any)
    while (_msgsRcvd.size() > 0) {
        _lgr.LogMessage("\n--------------------------------------------------------------\nTest "
            + std::to_string(i++) + " of " + std::to_string(_numMsgsSent) + ":\n\n");
        _lgr.LogResult(_msgsRcvd.deQ());
        _lgr.LogMessage("--------------------------------------------------------------\n\n");
    }
}
