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

