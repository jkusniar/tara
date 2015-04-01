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
