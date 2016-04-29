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
#include "convert.h"
#include "services.h"

RecordsWin::RecordsWin(int lang)
{
	CtrlLayoutOKCancel(*this, t_("Patient's records"));
	
	this->lang = lang;
	
	recordbox.SetLabel(t_("Record"));
	
	records.AddIndex(ID);
	records.AddColumn(REC_DATE, t_("Date"));
	records.SetToolBar(true, BarCtrl::BAR_TOP);
	records.WhenChangeRow = THISBACK(when_records_change_row);
	records.WhenInsertRow = THISBACK(when_records_new_inserted);
	
	// labor items
	labor_items.AddIndex(ID);
	labor_items.AddColumn(PROD_ID, t_("Product")).Edit(labor_item_product).SetConvert(labor_item_product).Width(260);
	labor_items.AddColumn(AMOUNT, t_("Amount")).Edit(labor_item_amount).SetConvert(Single<ConvertAmount>()).Width(60);
	labor_items.AddColumn(PROD_PRICE, t_("Ut. price")).Edit(labor_item_price).SetConvert(Single<ConvertMoney>()).Width(65);
	labor_items.AddColumn(UNIT_ID, t_("Ut.")).Edit(labor_item_unit.SetEditable(false)).SetConvert(labor_item_unit).Width(55);
	labor_items.AddColumn(ITEM_PRICE, t_("Price")).Edit(labor_item_final_price.SetEditable(false)).SetConvert(Single<ConvertMoney>()).Width(45);
	labor_items.AddColumn(PLU, t_("PLU")).Edit(labor_item_plu.SetEditable(false)).Width(30);
	
	labor_items.SetToolBar(false);
	labor_items.WhenInsertRow = THISBACK(when_labor_item_inserted);
	labor_items.WhenUpdateRow = THISBACK(when_labor_item_updated);
	labor_items.WhenRemoveRow = THISBACK(when_labor_item_deleted);
	labor_item_product.WhenAction = THISBACK(when_labor_item_product_changes);
	labor_item_amount.WhenAction = THISBACK(when_recompute_labor_price);
	labor_item_price.WhenAction = THISBACK(when_recompute_labor_price);
	labor_item_product.ColorRows(true);
	
	// material_items
	material_items.AddIndex(ID);
	material_items.AddColumn(PROD_ID, t_("Product")).Edit(material_item_product).SetConvert(material_item_product).Width(290);
	material_items.AddColumn(AMOUNT, t_("Amount")).Edit(material_item_amount).SetConvert(Single<ConvertAmount>()).Width(60);
	material_items.AddColumn(PROD_PRICE, t_("Ut. price")).Edit(material_item_price).SetConvert(Single<ConvertMoney>()).Width(65);
	material_items.AddColumn(UNIT_ID, t_("Ut.")).Edit(material_item_unit.SetEditable(false)).SetConvert(material_item_unit).Width(55);
	material_items.AddColumn(ITEM_PRICE, t_("Price")).Edit(material_item_final_price.SetEditable(false)).SetConvert(Single<ConvertMoney>()).Width(45);
	
	material_items.SetToolBar(false);
	material_items.WhenInsertRow = THISBACK(when_material_item_inserted);
	material_items.WhenUpdateRow = THISBACK(when_material_item_updated);
	material_items.WhenRemoveRow = THISBACK(when_material_item_deleted);
	material_item_product.WhenAction = THISBACK(when_material_item_product_changes);
	material_item_amount.WhenAction = THISBACK(when_recompute_material_price);
	material_item_price.WhenAction = THISBACK(when_recompute_material_price);
	material_item_product.ColorRows(true);
	
	
	// load LOVs
	::LoadLov(labor_item_unit, LOV_UNIT);
	::LoadLov(material_item_unit, LOV_UNIT);
	
	is_new = false; // no new record inserted
	
	text.WhenBar = THISBACK(when_text_menu);
	
	print_rec.WhenAction = THISBACK(when_print_record);
	
	sum_dbl = 0.0;
	
	cash_register_commands.SetFont(Font().Face(Font::MONOSPACE).Height(12));
	
	// prepare invoice formatter
	InvoiceDataWin inv_data;
	LoadFromFile(inv_data, "tara_invoice.cfg");
	formatter.vet_name = ~inv_data.name;
	formatter.vet_address = ~inv_data.address;
	formatter.vet_phone1 = ~inv_data.phone1;
	formatter.vet_phone2 = ~inv_data.phone2;
	formatter.vet_ic = ~inv_data.ic;
	formatter.vet_dic = ~inv_data.dic;
	formatter.vet_icdph = ~inv_data.icdph;
	formatter.vet_bank_acc = ~inv_data.acc_num;
	formatter.lang = lang;
}

void RecordsWin::when_records_change_row()
{
	checksave_record_data(records.GetPrevCursorId());
	text.Clear();
	
	// select other record data
	SQL & Select(DATA.Of(RECORD), INVOICE_ID.Of(RECORD))
		.From(RECORD)
		.Where(ID.Of(RECORD) == records(ID));
	
	while(SQL.Fetch()) {
		text.SetData(SQL[DATA]);
		
		if (SQL[INVOICE_ID].IsNull()) {
			inv_id.SetText("");
		} else {
			inv_id.SetText(AsString(SQL[INVOICE_ID]));
		}
	}
	
	::LoadProducts(labor_item_product, records.Get());
	::LoadProducts(material_item_product, records.Get());
	
	//select record labor_items
	labor_items.Clear();
	SQL & Select(ID.Of(RECORD_ITEM), PROD_ID.Of(RECORD_ITEM), AMOUNT.Of(RECORD_ITEM), PROD_PRICE.Of(RECORD_ITEM),
		UNIT_ID.Of(LOV_PRODUCT), ITEM_PRICE.Of(RECORD_ITEM), PLU.Of(LOV_PRODUCT))
		.From(RECORD_ITEM)
		.LeftJoin(LOV_PRODUCT).On(PROD_ID.Of(RECORD_ITEM) == ID.Of(LOV_PRODUCT))
		.Where(RECORD_ID.Of(RECORD_ITEM) == records(ID) && ITEM_TYPE.Of(RECORD_ITEM) == RIT_LABOR)
		.OrderBy(ID.Of(RECORD_ITEM));
	
	sum_dbl = 0.0;
	while(SQL.Fetch()) {
		labor_items.Add(SQL);
		sum_dbl += (double)SQL[ITEM_PRICE];
	}
	
	//select record material_items
	material_items.Clear();
	SQL & Select(ID.Of(RECORD_ITEM), PROD_ID.Of(RECORD_ITEM), AMOUNT.Of(RECORD_ITEM), PROD_PRICE.Of(RECORD_ITEM),
		UNIT_ID.Of(LOV_PRODUCT), ITEM_PRICE.Of(RECORD_ITEM))
		.From(RECORD_ITEM)
		.LeftJoin(LOV_PRODUCT).On(PROD_ID.Of(RECORD_ITEM) == ID.Of(LOV_PRODUCT))
		.Where(RECORD_ID.Of(RECORD_ITEM) == records(ID) && ITEM_TYPE.Of(RECORD_ITEM) == RIT_MATERIAL)
		.OrderBy(ID.Of(RECORD_ITEM));
	
	while(SQL.Fetch()) {
		material_items.Add(SQL);
		sum_dbl += (double)SQL[ITEM_PRICE];
	}
	
	//compute sum
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}

void RecordsWin::when_records_new_inserted()
{
	checksave_record_data(records.GetPrevCursorId());
	
	if (!is_new) {
		text.Clear();
		labor_items.Clear();
		material_items.Clear();
		is_new = true;
		records(REC_DATE) = ::GetSysTime();
		
		SQL & Insert(RECORD)
			(PATIENT_ID, patient)
	    	(REC_DATE, records(REC_DATE))
	    	(BILLED, false)
		;
			
		records.Set(ID, SQL.GetInsertedId());
		
		sum.SetText("0,00");
		sum_dbl = 0.0;
		
		inv_id.SetText("");
		
		cash_register_commands.Clear();
		
		::LoadProducts(labor_item_product, records.Get());
		::LoadProducts(material_item_product, records.Get());
		
		// mark new record using bold font
		records.GetRow(records.GetCursor()).SetFont(Font::GetStdFont().Bold(true));
	} else {
		records.RemoveLast();
		PromptOK(t_("Cannot insert more than one record."));
	}
}

void RecordsWin::when_labor_item_product_changes() {
	if (!labor_item_product.GetKey().IsNull()) {
		// select product data from LOV table
		SQL & Select(UNIT_ID, PRICE, PLU)
			.From(LOV_PRODUCT)
			.Where(ID.Of(LOV_PRODUCT) == labor_item_product.GetKey());
	
		/// fill gridctrl and controls
		while(SQL.Fetch())
		{
			labor_item_amount.SetData(1.0);
			labor_item_price.SetData(SQL[PRICE]);
			labor_item_final_price.SetData(SQL[PRICE]);
			labor_item_plu.SetData(SQL[PLU]);
			// change unit on product change
			//unit.Set(ID, SQL[UNIT_ID]);
			
			labor_items(AMOUNT) = 1.0;
			labor_items(PROD_PRICE) = SQL[PRICE];
			labor_items(ITEM_PRICE) = SQL[PRICE];
			labor_items(UNIT_ID) = SQL[UNIT_ID];
			labor_items(PLU) = SQL[PLU];
		}
		
		sum_items();
		sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	}
}

void RecordsWin::when_recompute_labor_price()
{
	labor_item_final_price = custom_round_double(labor_item_amount * labor_item_price, -2);
	labor_items(ITEM_PRICE) = labor_item_final_price.GetData();
		
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
}

void RecordsWin::when_print_record()
{
	checksave_record_data(records.GetCursorId());
	Perform(formatter.formatRecord(findRecord(records(ID))));
}

void RecordsWin::when_labor_item_inserted()
{
	SQL & Insert(RECORD_ITEM)
		(RECORD_ID, records(ID))
		(PROD_ID, labor_items(PROD_ID))
		(AMOUNT, labor_items(AMOUNT))
		(ITEM_PRICE, labor_items(ITEM_PRICE))
		(PROD_PRICE, labor_items(PROD_PRICE))
		(ITEM_TYPE, RIT_LABOR)
	;
	
	labor_items.Set(ID, SQL.GetInsertedId());
	
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}

void RecordsWin::when_labor_item_updated()
{
	SQL & ::Update(RECORD_ITEM)
		(PROD_ID, labor_items(PROD_ID))
		(AMOUNT, labor_items(AMOUNT))
		(ITEM_PRICE, labor_items(ITEM_PRICE))
		(PROD_PRICE, labor_items(PROD_PRICE))
	.Where(ID == labor_items(ID));
	
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}

void RecordsWin::when_labor_item_deleted()
{
	SQL & Delete(RECORD_ITEM).Where(ID == labor_items(ID));
	
	sum_items(RIT_LABOR, labor_items.GetRowId());
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}

void RecordsWin::sum_items(RecordItemType item_type, int removed_idx)
{
	sum_dbl = 0.0;
	for (int i=0; i<labor_items.GetCount(); i++)
		sum_dbl += (removed_idx == i && item_type == RIT_LABOR) ? 0.0 : (double) labor_items.Get(i, ITEM_PRICE);
	
	for (int i=0; i<material_items.GetCount(); i++)
		sum_dbl += (removed_idx == i && item_type == RIT_MATERIAL) ? 0.0 : (double) material_items.Get(i, ITEM_PRICE);
}

void RecordsWin::when_text_menu(Bar& menu)
{
	menu.Add(t_("Phrases"), THISBACK(when_phrases_menu));
}

void RecordsWin::when_phrases_menu(Bar& menu)
{
	SQL & Select(ID, NAME).From(LOV_PHRASE).OrderBy(NAME);
	while(SQL.Fetch()) {
		menu.Add(AsString(SQL[NAME]), THISBACK1(apply_phrase ,SQL[ID]));
	}
	
	menu.Separator();
	menu.Add(t_("Add new phrase.."), THISBACK(when_new_phrase));
}

void RecordsWin::apply_phrase(int id)
{
	SQL & Select(PHRASE_TEXT).From(LOV_PHRASE).Where(ID == id);
	if (SQL.Fetch())
		text.Insert(text.GetCursor(),  AsString(SQL[0]));
}

void RecordsWin::when_new_phrase()
{
	WithNewPhraseLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("New phrase"));
	if (dlg.Run(true) == IDOK)
	{
		// check if exists
		SQL & Select(SqlCountRows())
			.From(LOV_PHRASE)
			.Where(NAME == ~dlg.name);
			
		if (SQL.Fetch() && SQL[0] == 0)
		{
			SQL & Insert(LOV_PHRASE)
				(NAME, ~dlg.name)
				(PHRASE_TEXT, ~dlg.phrase_text)
			;
			
			text.Insert(text.GetCursor(),  AsString(~dlg.phrase_text));
		}
		else
			PromptOK(Format(t_("Phrase %s already exists."), ~dlg.name));

	}
}

void RecordsWin::checksave_record_data(int recordsCursorId)
{
	// save data to record if it has been changed
	if (text.IsDirty()&& recordsCursorId >= 0) {

		SQL & ::Update(RECORD)
		(DATA, text.GetData())
		.Where(ID == records.Get(recordsCursorId, ID));
		
		text.ClearDirty();
	}
}

/*
 * to round 123.125 to 123.13 use custom_round_double(123.125, -2)
 */
double RecordsWin::custom_round_double(double d, int n)
{
	return floor(d / ipow10(n) + 0.5) * ipow10(n);
}




void RecordsWin::when_material_item_inserted() {
		SQL & Insert(RECORD_ITEM)
		(RECORD_ID, records(ID))
		(PROD_ID, material_items(PROD_ID))
		(AMOUNT, material_items(AMOUNT))
		(ITEM_PRICE, material_items(ITEM_PRICE))
		(PROD_PRICE, material_items(PROD_PRICE))
		(ITEM_TYPE, RIT_MATERIAL)
	;
	
	material_items.Set(ID, SQL.GetInsertedId());
	
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}
	
void RecordsWin::when_material_item_updated() {
		SQL & ::Update(RECORD_ITEM)
		(PROD_ID, material_items(PROD_ID))
		(AMOUNT, material_items(AMOUNT))
		(ITEM_PRICE, material_items(ITEM_PRICE))
		(PROD_PRICE, material_items(PROD_PRICE))
	.Where(ID == material_items(ID));
	
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}
	
void RecordsWin::when_material_item_deleted() {
	SQL & Delete(RECORD_ITEM).Where(ID == material_items(ID));
	
	sum_items(RIT_MATERIAL, material_items.GetRowId());
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	generate_cash_register_commands();
}

void RecordsWin::when_material_item_product_changes() {
	if (!material_item_product.GetKey().IsNull()) {
		// select product data from LOV table
		SQL & Select(UNIT_ID, PRICE, PLU)
			.From(LOV_PRODUCT)
			.Where(ID.Of(LOV_PRODUCT) == material_item_product.GetKey());
	
		/// fill gridctrl and controls
		while(SQL.Fetch())
		{
			material_item_amount.SetData(1.0);
			material_item_price.SetData(SQL[PRICE]);
			material_item_final_price.SetData(SQL[PRICE]);
			
			material_items(AMOUNT) = 1.0;
			material_items(PROD_PRICE) = SQL[PRICE];
			material_items(ITEM_PRICE) = SQL[PRICE];
			material_items(UNIT_ID) = SQL[UNIT_ID];
		}
		
		sum_items();
		sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
	}
}

void RecordsWin::when_recompute_material_price() {
	material_item_final_price = custom_round_double(material_item_amount * material_item_price, -2);
	material_items(ITEM_PRICE) = material_item_final_price.GetData();
		
	sum_items();
	sum.SetText(AsString(ConvertMoney().Format(sum_dbl)));
}

void RecordsWin::generate_cash_register_commands() {
	Vector<String> commands;
	cash_register_commands.Clear();
	
	//process labor_items first
	SQL & Select(PLU.Of(LOV_PRODUCT), ITEM_PRICE.Of(RECORD_ITEM))
		.From(RECORD_ITEM)
		.LeftJoin(LOV_PRODUCT).On(PROD_ID.Of(RECORD_ITEM) == ID.Of(LOV_PRODUCT))
		.Where(RECORD_ID.Of(RECORD_ITEM) == records(ID) && ITEM_TYPE.Of(RECORD_ITEM) == RIT_LABOR)
		.OrderBy(ID.Of(RECORD_ITEM));

	// labor items with PLU undefined are added to first PLU found
	double	sum_plu = 0.0;
	String plu;
	while(SQL.Fetch()) {
		if (!SQL[PLU].IsNull()) {
			if (!plu.IsEmpty()) {
				commands.Add(AsString(ConvertMoney().Format(sum_plu)) + " [CENA] " + plu + " [PLU]");
				sum_plu = 0.0;
			}
			plu = AsString(SQL[PLU]);
		}
		sum_plu += (double) SQL[ITEM_PRICE];
	}
	if (!plu.IsEmpty()) {
		commands.Add(AsString(ConvertMoney().Format(sum_plu)) + " [CENA] " + plu + " [PLU]");
	}
	
	//calculate record material_items summary price
	SQL & Select(SqlSum(ITEM_PRICE))
		.From(RECORD_ITEM)
		.LeftJoin(LOV_PRODUCT).On(PROD_ID.Of(RECORD_ITEM) == ID.Of(LOV_PRODUCT))
		.Where(RECORD_ID.Of(RECORD_ITEM) == records(ID) && ITEM_TYPE.Of(RECORD_ITEM) == RIT_MATERIAL);
	
	if(SQL.Fetch() && !SQL[0].IsNull()) {
		commands.Add(AsString(ConvertMoney().Format(SQL[0])) + " [CENA] TERAPIA [PLU]");
	}
	
	int pos = 0;
	for(int i = 0; i < commands.GetCount(); i++) {
		pos += cash_register_commands.Insert(pos, commands[i] + "\n");
	}
}