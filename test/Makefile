all: run

test:
	g++ -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -O3 -I .. test.cpp ../console_stream.cpp -DLOGGING_LEVEL=0 -DUSE_COLOUR_STREAMS=1 -o console_stream_test

run: test
	sh -c "./console_stream_test"

