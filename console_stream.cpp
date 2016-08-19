#include "console_stream.hpp"

// Initialise the actual streams.
// By setting init_priority to the lowest possible level, the notifications can be safely used in constructors.

__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, USE_DEBUG_STREAM> debug("\x1b[33m(DD) ");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, USE_LOGGER_STREAM> logger("\x1b[32m(LL) ");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, USE_ERROR_STREAM> error("\x1b[31m(EE) ");

#if defined(USE_COLOUR_STREAMS) && USE_COLOUR_STREAMS != 0
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> red_stream("\x1b[31m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> green_stream("\x1b[32m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> yellow_stream("\x1b[33m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> blue_stream("\x1b[34m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> magenta_stream("\x1b[35m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> cyan_stream("\x1b[36m");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> white_stream("\x1b[37m");
#endif



