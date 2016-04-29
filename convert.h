/*
    Tara, simple veterinary practice support software
    Copyright (C) 2009 - 2015  Jan Kusniar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef _tara_convert_h_
#define _tara_convert_h_

#include <Core/Core.h>
using namespace Upp;

class NumberToTextConverter
{
public: virtual String convert(unsigned long cislo) = 0;
};

class SlovakNumberToTextConverter : public NumberToTextConverter
{
public: String convert(unsigned long cislo);
	SlovakNumberToTextConverter();

private:
	// Number to text conversion...
	VectorMap<unsigned long, String> map_jednotky;
	VectorMap<unsigned long, String> map_desiatky;
	VectorMap<unsigned long, String> map_rady;
	
	String convertJednotky(unsigned long num, unsigned long multipl);
	String convertDeasiatky(unsigned long jednotky, unsigned long desiatky);
	String convertStovky(unsigned long cislo, unsigned long rad);
	String convertRady(unsigned long cislo, unsigned long rad);
};

class ConvertMoney : public ConvertDouble
{
public: ConvertMoney() : ConvertDouble() {Pattern("%2!nl");}
//public: ConvertMoney() : ConvertDouble() {Pattern("%2,!nl");} // if sk_SK doesn't work
};

class ConvertAmount : public ConvertDouble
{
public: ConvertAmount() : ConvertDouble() {Pattern("%4!nl");}
};

class ConvertLocalized : public ConvertDouble
{
public: ConvertLocalized() : ConvertDouble() {Pattern("%10vl");}
//public: ConvertLocalized() : ConvertDouble() {Pattern("%10,vl");} // if sk_SK doesn't work
};

#endif
