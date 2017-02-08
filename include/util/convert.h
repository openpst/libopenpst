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
* @file convert.h
* @package openpst/libopenpst
* @brief hexdump helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "util/hexdump.h"
#include <iostream>
#include <stdio.h>

std::string bytesToHex(unsigned char* input, int size, bool byteswap);
std::string hexToString(char *input, int length);
std::string min1Decode(unsigned char input[3]);
std::string min2Decode(unsigned char input[1]);
std::string sidDecode(unsigned char input[1]);
long HexToBytes(std::string input);
