/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of libopenpst.
* 
* libopenpst is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* libopenpst is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with libopenpst. If not, see <http://www.gnu.org/licenses/>.
*
* @file definitions.h
* @package openpst/libopenpst
* @brief standard definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#if defined(_WIN32) && !defined(__MINGW32__)
	#include "win_stdint.h"
	#include "win_inttypes.h"
	#include  <windows.h>
	#define PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

#else
	#include <stdint.h>
	#include <unistd.h>
	#define PACKED( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef DEBUG
#define LOGD(...) fprintf(stderr, __VA_ARGS__); 
#else
#define LOGD(...)
#endif

#define LOGE(...) fprintf(stderr, __VA_ARGS__); 
#define LOGI(...) fprintf(stdout, __VA_ARGS__); 

#ifndef sleep
#ifdef _WIN32
void sleep(int milliseconds) {
	Sleep(milliseconds);
}
#else
void sleep(int milliseconds) {
	usleep(milliseconds * 1000);
}
#endif
#endif