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
* @file hexdump.h
* @package openpst/libopenpst
* @brief hexdump helper functions
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author https://github.com/posixninja/DLOADTool
*/
#pragma once

#include "definitions.h"
#include <iostream>
#include <stdio.h>

#ifdef QT_CORE_LIB
#include <QVariant>
#endif


#ifdef HEXDUMP_PORT_TX
#define hexdump_tx(data, amount) \
			do { if (amount > 0) printf("Dumping %lu bytes written\n", amount); hexdump(data, amount); } while (0)
#else
#define hexdump_tx(data, amount) \
			do {} while (0)
#endif


#ifdef HEXDUMP_PORT_RX
#define hexdump_rx(data, amount) \
			do { if (amount > 0) printf("Dumping %lu bytes read\n", amount); hexdump(data, amount); } while (0)
#else
#define hexdump_rx(data, amount) \
			do {} while (0)
#endif

const char hex_trans_dump[] =
	"................................ !\"#$%&'()*+,-./0123456789"
	":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
	"nopqrstuvwxyz{|}~...................................."
	"....................................................."
	"........................................";

const char hex_trans_display[] =
	"                                 !\"#$%&'()*+,-./0123456789"
	":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
	"nopqrstuvwxyz{|}~                                    "
	"                                                     "
	"                                        ";

void hexdump(unsigned char *data, unsigned int amount);
void hexdump(unsigned char *data, unsigned int amount, FILE* file);

#ifdef QT_CORE_LIB
void hexdump(unsigned char *data, unsigned int amount, QString& out, bool toHtml);
#endif

