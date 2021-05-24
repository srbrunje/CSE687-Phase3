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


#include <chrono>       // for timing
#include <string>       // for std::string
#include <exception>    // for std::exception
#include <sstream>      // for std::stringstream in DblToStr()
#include <iomanip>      // for std::fixed and std::setpricision in DblToStr()

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
enum class LogLevel
{
	Pass_Fail,
	Pass_Fail_with_error_message,
	Pass_Fail_with_test_duration,
	Pass_Fail_with_error_message_and_test_duration
};


/** DblToStr - public
* Description: Converts a double into a string using a specified number of decimals along with
*              a comma separator if not deselected.
* Parameter 0: the double value to convert to a string
* Parameter 1: the number of decimal places to include (default=2)
* Parameter 2: use a comma separator (true, default) or don't (false)
* Return: the double value in std::string form
*/
static std::string DblToStr(const double aVal,
	const unsigned int aNumDecimalPlaces = 2,
	const bool useCommaSeparator = true)
{
	// Use a stringstream with iomanip to perform decimal place specification
	// and then place the given value into that stringstream
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(aNumDecimalPlaces) << aVal;

	// Conver the stringstream into a string
	std::string str = ss.str();

	// If using a comma separator, insert commas where appropriate
	if (useCommaSeparator) {
		// Start by finding the start of the number in the 1's column
		size_t pos = str.find('.'); // if the string contains a decimal point, start there
		if (pos == std::string::npos) {
			// If it doesn't, simply start at the end of the string and move backwards
			pos = str.length();
		}
		// Add a comma ever 3 positions left of the decimal place until the front is reached
		while (pos > 3 && pos != std::string::npos) {
			pos -= 3;
			str.insert(pos, 1, ',');
		}
	}
	return str;
}



/** FormatTimeString - public
* Description: Formats the time string. Scales the units to report units in 1000x incraments.
*              seconds, milli, micro, nano
* Parameter 0: The time to scale
* Return: The formated string
*/
static std::string FormatTimeString(double dDurration)
{
	std::string rtn = "";

	if (dDurration > 1000000) //scale to seconds
	{
		rtn = DblToStr(dDurration / 1000000) + " seconds";
	}
	else if (dDurration > 1000) //scale to milliseconds
	{
		rtn = DblToStr(dDurration / 1000) + " milliseconds";
	}
	else if (dDurration < 1) //scale to nanoseconds
	{
		rtn = DblToStr(dDurration * 1000) + " nanoseconds";
	}
	else //microseconds
	{
		rtn = DblToStr(dDurration) + " microseconds";
	}

	return rtn;
}



#endif // !__UTILS_H
