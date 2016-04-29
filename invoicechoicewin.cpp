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

#include "constants.h"
#include "mainwin.h"

InvoiceChoiceWin::InvoiceChoiceWin(int lang)
{
	CtrlLayoutOK(*this, t_("Invoice choice"));
	
	this->lang = lang;
	
	payment_type.Add(ptCurrency, t_("Currency"));
	payment_type.Add(ptBankTransfer,t_("Bank transfer"));
	payment_type.SetData(ptCurrency);
	
	payed_now.SetData(true);
	delivery_date.SetData(GetSysDate());
	creation_date.SetData(GetSysDate());
	payment_date.SetData(GetSysDate());
	
	if (lang == LANG_CZ) {
		payment_type.Disable();
		payment_type_lbl.Disable();
		payed_now.Disable();
		delivery_date.Disable();
		delivery_date_lbl.Disable();
		payment_date.Disable();
		payment_date_lbl.Disable();
	}
}

