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

#ifndef _tara_invoice_h_
#define _tara_invoice_h_

#include <Core/Core.h>
#include <Report/Report.h>
using namespace Upp;

enum InvoicePaymentType
{
	iptCurrency = 0,
	iptBankTransfer
};

enum RecordItemCol
{
	ritProduct = 0,
	ritAmount,
	ritUnit,
	ritUnitPrice,
	ritPrice
};

class InvoiceData : Moveable<InvoiceData> {
public:
	typedef InvoiceData CLASSNAME;
	InvoiceData(){}
	void clear();
	
	// header
	Value 	inv_id;
	Date	create_date, delivery_date, payment_date, payed_date;
	InvoicePaymentType	payment_type;
	
	/// veterinary data
	String 	name, address, phone1, phone2, vet_ic, vet_dic, vet_icdph, bank_acc;
	
	// client data
	String 	client_title, client_name, client_surname;
	String 	client_street, client_city, client_zip, client_house_no;
	String 	client_ic, client_dic, client_icdph;
	
	//patient data
	String patient_name, patient_species, patient_breed;
	
	// record data
	String rec_text;	
	//items
	Vector< VectorMap<int, String> > record_items;
	double summary_price;
};

class InvoiceFormatter {
public:
	typedef InvoiceFormatter CLASSNAME;
	InvoiceFormatter() {}
	
	Report& formatFullInvoice(InvoiceData& invoice);
	Report& formatFullInvoices(Vector<InvoiceData>& invoices);
	Report& formatBill(InvoiceData& invoice);
	Report& formatRecord(InvoiceData& invoice);
	
	int lang;

	// veterinary data
	String vet_name, vet_address, vet_phone1, vet_phone2;
	String vet_ic, vet_dic, vet_icdph, vet_bank_acc;	
private:
	Report output;
	
	void formatHeader(StringBuffer &buf, InvoiceData &invoice);
	void formatCompanyData(StringBuffer &buf);
	void formatClientData(StringBuffer &buf, InvoiceData &invoice);
	void formatPatientData(StringBuffer &buf, InvoiceData &invoice);
	void formatInvoiceItems(StringBuffer &buf, InvoiceData &invoice);
	void formatBillHeader(StringBuffer &buf, InvoiceData &invoice, bool isStandelone = true);
	void formatBillPrice(StringBuffer &buf, InvoiceData &invoice);
	
	void formatRecordHeader(StringBuffer &buf, InvoiceData &invoice);
	void formatVetData(StringBuffer &buf);
	void formatOwnerData(StringBuffer &buf, InvoiceData &invoice);
	void formatRecordItems(StringBuffer &buf, InvoiceData &invoice);
	
	String escapeText(String& s);
};

String fixFuckedLinuxFormating(const String &s);

#endif
