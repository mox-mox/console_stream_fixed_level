all: run

test:
	g++ -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -O3 -I .. test.cpp ../console_stream.cpp -DUSE_DEBUG_STREAM -DUSE_LOGGER_STREAM -DUSE_ERROR_STREAM -DUSE_COLOUR_STREAMS -o console_stream_test

run: test
	sh -c "./console_stream_test"

