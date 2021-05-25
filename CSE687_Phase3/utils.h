#ifndef __UTILS_H
#define __UTILS_H


/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* Date: 04-26-2021
*
* File: utils.h
*
* Data Defined: namespace timing
*               enum class LogLevel
*
* Description: Provides a space for commonly
*              used data and functions, while
*              protecting them with a namespace.
*
***********************************************/
#include <chrono> // for timing
#include <string> // for to_string function
#include <exception>    // std::exception
#include <iostream>

// Set of shorthands for timing purposes
namespace timing {

	typedef std::chrono::high_resolution_clock clock;
	typedef clock::time_point                  hack;

	static hack now() {
		return clock::now();
	}

	static double duration_us(hack aStartTime, hack aEndTime = now()) {
		return std::chrono::duration<double, std::micro>(aEndTime - aStartTime).count();
	}
}

// Enum class to define the level of logging
// Since it is a class, it has scoping protection
// i.e. must use "LogLevel::" to access it
enum class LogLevel
{
	Pass_Fail = 1,
	Pass_Fail_with_error_message = 2,
	Pass_Fail_with_test_duration = 3,
	Pass_Fail_with_error_message_and_test_duration = 4,
	Pass_Fail_With_error_message_and_expected_value = 5
};

/** FormatTimeString - private
	* Description: Formats the time string. Scales the units to report units in 1000x incraments.
	*              seconds, milli, micro, nano, pico
	* Parameter 0: The time to scale
	* Return: The formated string
	*/

static std::string FormatTimeString(double dDurration)
{
	std::string rtn = "";

	if (dDurration > 1000000) //scale to seconds
	{
		rtn = std::to_string(dDurration / 1000000) + " seconds";
	}
	else if (dDurration > 1000) //scale to milliseconds
	{
		rtn = std::to_string(dDurration / 1000) + " milliseconds";
	}
	else if (dDurration < 1) //scale to nanoseconds
	{
		rtn = std::to_string(dDurration * 1000) + " nanoseconds";
	}
	else //milliseconds
	{
		rtn = std::to_string(dDurration) + " microseconds";
	}

	return rtn;
}

/** removeNewLine - private
 * Description: searches the string and looks for '\n', if found.
 * We will simply skip ot
 * Parameter 0: json string
 * Return: The new formated string
*/
static std::string removeNewLine(std::string str) {

	std::string newString{ "" };

	for (int x = 0; x < str.length(); x++)
		if (str[x] != '\n') newString += str[x];


	return  newString;
}




#endif // !__UTILS_H