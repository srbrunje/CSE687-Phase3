#pragma once
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

class TestServer
{
public:
	
	
	TestServer();

	void StartServer();

	void StopServer();

	void InitilizeTestServer();

private:
	
	//void ProcessReplies();
	
	TestManager* mgr;
	bool shutdown;

};

