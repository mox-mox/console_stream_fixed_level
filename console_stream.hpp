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

#ifndef CONSOLE_STREAM_HPP
#define CONSOLE_STREAM_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

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

template< class CharT, class Traits = std::char_traits<CharT>, bool active=true >
class Console_stream: public std::ostream
{
	private:
		/**
		 * A stream buffer that wraps each line within prefix and postfix.
		 */
		class LogStreamBuf: public std::stringbuf
		{
			std::string prefix;
			std::ostream&   output;
			std::string postfix;
			public:
				LogStreamBuf(std::string prefix, std::ostream&str_, std::string postfix) : prefix(prefix), output(str_), postfix(postfix)
				{}

				// When we sync the stream with the output.
				// 1) Output prefix then the buffer then the postfix
				// 2) Reset the buffer
				// 3) flush the actual output stream we are using.
				virtual int sync()
				{
					output<<"["<<std::setw(10)<<
					    std::chrono::duration_cast < std::chrono::seconds > (std::chrono::high_resolution_clock::now().time_since_epoch()).count()<<
					    "] "<<prefix<<str()<<postfix;
					str("");
					output.flush();
					return 0;
				}
		};

		LogStreamBuf buffer;
	public:
		// \x1b[37m is an ANSI-escape sequence, see https://en.wikipedia.org/wiki/ANSI_escape_code
		Console_stream(std::string prefix, std::ostream& str = std::cout, std::string postfix = "\x1b[37m") :
			std::ostream(&buffer), buffer(prefix, str, postfix)
		{ }
};


/**
 * Partial specialisation that disables the output
 */
template< class CharT, class Traits>
class Console_stream<CharT,Traits, false>: public std::ostream
{
	public:
		Console_stream(std::string prefix, std::ostream& str = std::cout, std::string postfix = "")
		{
			(void) prefix;
			(void) str;
			(void) postfix;
		}

		/**
		 * The function handling formatting functions like std::endl.
		 * This function is actually needed and has to be inlined to allow the compiler to completely optimise it away.
		 */
		__attribute__((always_inline)) inline __ostream_type& operator<<(__ostream_type& (*__pf)(__ostream_type&))
		{
			(void) __pf;
			return *this;
		}

		/**
		 * The function(s) handling formatting functions like std::endl.
		 * These functions are actually needed and have to be inlined to allow the compiler to completely optimise them away.
		 */
		template<typename T>
		__attribute__((always_inline)) inline Console_stream<CharT, Traits, false>& operator<<(T value)
		{
			(void) value;
			return *this;
		}
};

#if defined(USE_DEBUG_STREAM) && USE_DEBUG_STREAM != 0
	#undef USE_DEBUG_STREAM
	#define USE_DEBUG_STREAM true
#else
	#define USE_DEBUG_STREAM false
#endif

#if defined(USE_LOGGER_STREAM) && USE_LOGGER_STREAM != 0
	#undef USE_LOGGER_STREAM
	#define USE_LOGGER_STREAM true
#else
	#define USE_LOGGER_STREAM false
#endif

#if defined(USE_ERROR_STREAM) && USE_ERROR_STREAM != 0
	#undef USE_ERROR_STREAM
	#define USE_ERROR_STREAM true
#else
	#define USE_ERROR_STREAM false
#endif

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

#endif /* CONSOLE_STREAM_HPP */

