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

// A little test program to see if the console streams work as expected
#include "utils.h"
#include "console_stream.hpp"

__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, true> logger_active("\x1b[32m(aa) ");
__attribute__ ((init_priority (101))) Console_stream<char, std::char_traits<char>, false> logger_passive("\x1b[32m(pp) ");


int main()
{
	// Check whether the inactivated streams are really optimised away:
	const uint64_t ITERATIONS = 100;
	const uint64_t LOOPS = 100;
	uint64_t t0, t1;
	uint64_t t_offset = 0;
	uint64_t t_print = 0;
	uint64_t t_noprint = 0;
	uint64_t t_none = 0;

	for(uint64_t loops=0; loops<LOOPS; loops++)
	{
		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			logger_active << "foobar " << std::setw(10) << i << std::endl;
		}
		rdtsc(t1);
		t_print += t1-t0;

		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			logger_passive << "foobar " << std::setw(10) << i << std::endl;
		}
		rdtsc(t1);
		t_noprint += t1-t0;

		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			if(0) logger_active << "foobar " << std::setw(10) << i << std::endl;
		}
		rdtsc(t1);
		t_none += t1-t0;

		rdtsc(t0);
		rdtsc(t1);
		t_offset += t1-t0;
	}


	std::cout<<"Active (with real output):     "<<t_print<<'\n';
	// If optimisaton is enabled with the compiler, the three times without output should be identical.
	std::cout<<"Passive (output deactivated):  "<<t_noprint<<'\n';
	std::cout<<"None (with an if(0) prepended: "<<t_none<<std::endl;
	std::cout<<"Measurement offset:            "<<t_offset<<'\n';



	// Do check if the streams output data as expected
	debug<<"Hello"<<std::endl;
	logger<<"these are some"<<std::endl;
	error<<"fancy output streams."<<std::endl;

	red_stream<<"red_stream"<<std::endl;
	green_stream<<"green_stream"<<std::endl;
	yellow_stream<<"yellow_stream"<<std::endl;
	blue_stream<<"blue_stream"<<std::endl;
	magenta_stream<<"magenta_stream"<<std::endl;
	cyan_stream<<"cyan_stream"<<std::endl;
	white_stream<<"white_stream"<<std::endl;


	return 0;
}

