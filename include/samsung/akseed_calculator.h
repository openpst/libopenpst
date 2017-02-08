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
* @file akseed.h
* @package openpst/libopenpst
* @brief Samsung AKSEED Calculations
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include <string>
#include <math.h>
#include <sstream>
#include <tuple>

namespace OpenPST {
	namespace Samsung {

		enum AkseedModelOffset {
			kAkseedModelMarvell		= 0,
			kAkseedModelSMN900S		= 51,
			kAkseedModelKorea		= 51,
			kAkseedModelSMN900L		= 71,
			kAkseedModelSMG900L		= 47,
			kAkseedModelSMG900K		= 54,
			kAkseedModelSMG900S		= 73,
			kAkseedModelSHVE470S	= 73,
			kAkseedModelSMN900K		= 61,
			kAkseedModelSHVE250K	= 88,
			kAkseedModelDefault		= 89,
		};

		class AkseedCalculator {

			public:

				static std::tuple<int, int, int> calculate(int number1, int number2, int number3, int addOffset = kAkseedModelDefault);
				static std::string calculateAsString(int number1, int number2, int number3, int addOffset = kAkseedModelDefault);

		};
	}
}
