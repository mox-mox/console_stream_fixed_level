/***********************************************************************
*                                                                      *
* (C) 2016, Moritz Nöltner-Augustin                                    *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 3 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the Free Software          *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
* USA                                                                  *
*                                                                      *
***********************************************************************/

#pragma once

#ifndef LOGGING_LEVEL
	#pragma message("warning:To use the notify stream, set the notification level.\n" \
	"	* Use -DLOGGING_LEVEL=0 to disable all notification output\n"                  \
	"	* Use -DLOGGING_LEVEL=1 to enable only critical output\n"                      \
	"	* Use -DLOGGING_LEVEL=2 to enable normal and critical output\n"                \
	"	* Use -DLOGGING_LEVEL=3 to enable all output.\n" )
	#define LOGGING_LEVEL 0
#endif

#define USE_DEBUG_STREAM false
#define USE_LOGGER_STREAM false
#define USE_ERROR_STREAM false

#if LOGGING_LEVEL >= 1
	#undef USE_ERROR_STREAM
	#define USE_ERROR_STREAM true
#endif

#if LOGGING_LEVEL >= 2
	#undef USE_LOGGER_STREAM
	#define USE_LOGGER_STREAM true
#endif

#if LOGGING_LEVEL >= 3
	#undef USE_DEBUG_STREAM
	#define USE_DEBUG_STREAM true
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
//#include <chrono>
#include <sstream>

/**
 * \brief A stream class providing formatted output
 *
 * This class wrapps around an output stream and provides automated formatted output with the
 * usual stream operators.
 * It is taken from <a href="http://stackoverflow.com/a/2212940/4360539">this post</a> at
 * Stackoverflow, with the addition of a postfix and pre- and postfix parameters for the
 * constructor.
 * Usage:
 *
 * logger << "logging some stuff" << std::endl;
 *
 * Will give something like: [      1000] (LL) logging some stuff
 * with "(LL) logging some stuff" in green, while
 *
 * error << "Oh noes, something went wrong!" << std::endl;
 *
 * Will give something like : [      8467] (EE) Oh noes, something went wrong!
 * with "(EE) Oh noes, something went wrong!" in red.
 * The number in square brackets will be the current value of the milliseconds counter.
 *
 */
template<class CharT, class Traits = std::char_traits<CharT>, bool active=true>
class Console_stream: public std::ostream
{
	private:
		/**
		 * A stream buffer that wraps each line within prefix and postfix.
		 */
		class stream_buf: public std::stringbuf
		{
			std::string prefix;
			std::ostream& output;
			std::string postfix;
			public:
				stream_buf(std::string prefix, std::ostream& str_, std::string postfix) : prefix(prefix), output(str_), postfix(postfix)
				{}

				// When we sync the stream with the output.
				// 1) Output prefix then the buffer then the postfix
				// 2) Reset the buffer
				// 3) flush the actual output stream we are using.
				// Active (with real output):     time
				//
				virtual int sync()
				{
					std::string line;
					std::stringstream ss(str());
					getline(ss, line, '\n');
					//output<<"["<<std::setw(10)<<
					//    std::chrono::duration_cast < std::chrono::seconds > (std::chrono::steady_clock::now().time_since_epoch()).count()<<
					//    "] "<<prefix<<line<<'\n';
					std::time_t t = std::time(nullptr);
					output<<"["<<std::put_time(std::localtime(&t), "%F/%T")<<"] "<<prefix<<line<<'\n';
					while (getline(ss, line, '\n'))
					{
						output<<"                      "<<prefix<<line<<'\n';
					}

					output<<postfix;
					str("");
					output.flush();
					return 0;
				}
		};

		stream_buf buffer;
	public:
		// \x1b[37m is an ANSI-escape sequence, see https://en.wikipedia.org/wiki/ANSI_escape_code
		Console_stream(std::string prefix, std::ostream& str = std::cout, std::string postfix = "\x1b[37m") :
			std::ostream(&buffer), buffer(prefix, str, postfix)
		{
		}
};


/**
 * Partial specialisation that disables the output
 */
template<class CharT, class Traits>
class Console_stream<CharT,Traits, false>: public std::ostream
{
	public:
		Console_stream(std::string, std::ostream& = std::cout, std::string = "")
		{
		}

		/**
		 * The function handling formatting functions like std::endl.
		 * This function is actually needed and has to be inlined to allow the compiler to completely optimise it away.
		 */
		__attribute__((always_inline)) inline __ostream_type& operator<<(__ostream_type& (*)(__ostream_type&))
		{
			return *this;
		}

		/**
		 * The function handling normal data insertion into the stream.
		 * These functions are actually needed and have to be inlined to allow the compiler to completely optimise them away.
		 */
		template<typename T>
		__attribute__((always_inline)) inline Console_stream<CharT, Traits, false>& operator<<(T)
		{
			return *this;
		}
};


extern Console_stream<char, std::char_traits<char>, USE_DEBUG_STREAM> debug;
extern Console_stream<char, std::char_traits<char>, USE_LOGGER_STREAM> logger;
extern Console_stream<char, std::char_traits<char>, USE_ERROR_STREAM> error;

#if defined(USE_COLOUR_STREAMS) && USE_COLOUR_STREAMS != 0
	extern Console_stream<char, std::char_traits<char>, true> red_stream;
	extern Console_stream<char, std::char_traits<char>, true> green_stream;
	extern Console_stream<char, std::char_traits<char>, true> yellow_stream;
	extern Console_stream<char, std::char_traits<char>, true> blue_stream;
	extern Console_stream<char, std::char_traits<char>, true> magenta_stream;
	extern Console_stream<char, std::char_traits<char>, true> cyan_stream;
	extern Console_stream<char, std::char_traits<char>, true> white_stream;
#endif
