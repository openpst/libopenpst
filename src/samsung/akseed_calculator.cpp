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
* @file akseed.cpp
* @package openpst/libopenpst
* @brief Samsung AKSEED Calculations
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "samsung/akseed_calculator.h"

using namespace OpenPST::Samsung;


std::tuple<int, int, int> AkseedCalculator::calculate(int number1, int number2, int number3, int addOffset)
{
	int i = 1;
	int calc1, calc2, calc3;

	calc1 = number2;
	while (i <= (number3 / 2)){
		calc1 = (((number2 * number2) % number1) * calc1) % number1;
		i++;
	}

	calc1 += addOffset;

	calc2 = round(fmod((number1 * 0x181E5), log(number1))) + addOffset;
	calc3 = round(((number2 * number1) / number1) / log(number2)) + addOffset;

	return std::make_tuple(calc1, calc2, calc3);
}

std::string AkseedCalculator::calculateAsString(int number1, int number2, int number3, int addOffset)
{
	std::stringstream ss; 
	std::tuple<int, int, int> res = calculate(number1, number2, number3, addOffset);

	ss << std::get<0>(res) << "-" << std::get<1>(res) << "-" << std::get<2>(res);

	return ss.str();
}
