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

#ifndef _tara_services_h_
#define _tara_services_h_

#include <PostgreSQL/PostgreSQL.h>

using namespace Upp;

#include "invoice.h"

InvoiceData& findInvoice(Value recordId);
Vector<InvoiceData>& findInvoices(Date date_from, Date date_to);
InvoiceData& findBill(Value recordId);
InvoiceData& findRecord(Value recordId);

#endif
