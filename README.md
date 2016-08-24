# console_stream
Output stream wrapper that adds formatting and can be muted.

```c++
logger<<"some log output and a number: "<<std::setw(10)<<99<<"."<<std::endl;
```
Will print something like
```
[1471615467] (LL) some log output and a number:       99.
```
in green text where 1471615467 is the current system time in seconds, and (LL) is a prefix to identify the stream as the logging stream.

## Why would I want this?
Well a line like
```c++
debug<<__FILE__<<':'<<__LINE__<<"::"<<__func__<<"(): Ooopsie, something went wrong."<<std::endl;
```
is certainly nicer than 
```c++
#ifdef DEBUG
	std::cout<<print_the_time()<<": "<<__FILE__<<':'<<__LINE__<<"::"<<__func__<<"(): Ooopsie, something went wrong."<<std::endl;
#endif
```


## Usage

 1. Compilation
  * in the code #include console_stream.hpp
  * add console_stream.cpp to the compiled source files
  * #define USE_DEBUG_STREAM, USE_LOGGER_STREAM or USE_ERROR_STREAM to enable streams for debugging-, logging or error output respectively. This should be done as a compiler flag like -DUSE_ERROR_STREAM
  * #define USE_COLOUR_STREAMS to get some streams with coloured output.
 2. Code
  * Normal case: Use like std::cout. On std::endl, a new line prepended with system time and an optional prefix like (EE) for the error output is started, inserting '\n' results in a linebreak without prepending the time stamp.
  * Muting: Production software will usually not want debug information clobbering the output. Therefore, the debugging, logging and error streams can be muted by not defining their respective USE_*_STREAM constants. The muted output statements can remain in the code as the compiler will remove them when optimising the code (For g++, the code is completely removed with the settings -Os, -O2 and -O3).

### Advanced usage, internals
Like std::ofstream, the streams are templated to allow setting the underlying character type (usually char). Additionally, there is a boolean template parameter called "active" which determines wether the stream will output any data. When set to false, the stream has no internal buffer and the shift-in operators have no side-effects, allowing the compiler to remove the output statements.
The constructor of the streams takes 3 arguments:
 1. A string to prepend to every line. This can be used to print ANSI-escape sequences, for example to change the text colour.
 2. A stream buffer. Usually, one would use std::cout for a console output, but for a stream writing to a log file, that files' std::ofstream can be used.
 3. A string to append after each line, usefull to reset the colour to normal.
