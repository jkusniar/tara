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

#include "mainwin.h"

InvoiceDataWin::InvoiceDataWin()
{
	CtrlLayoutOKCancel(*this, t_("Invoice data"));
	
	//default data
	name.SetData("Doctor Name");
	address.SetData("Street No., ZIP");
	phone1.SetData("Phone 1");
	phone2.SetData("Phone 2");
	ic.SetData("ID");
	dic.SetData("ID TAX");
	icdph.SetData("ID VAT");
	acc_num.SetData("Bank acct.");
}

void InvoiceDataWin::Serialize(Stream &s)
{
	SerializePlacement(s);
	s % name;
	s % address;
	s % phone1;
	s % phone2;
	s % ic;
	s % dic;
	s % icdph;
	s % acc_num;
}
