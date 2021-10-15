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

#ifndef _tara_mainwin_h_
#define _tara_mainwin_h_

#include <iostream>

#include "invoice.h"

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>
#include <PostgreSQL/PostgreSQL.h>
#include <Report/Report.h>

using namespace Upp;

#define IMAGECLASS Images
#define IMAGEFILE <tara/tara.iml>
#include <Draw/iml_header.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#define MODEL <tara/taradb.sch>
#include <Sql/sch_header.h>

#define LAYOUTFILE <tara/tarawin.lay>
#include <CtrlCore/lay.h>

static const String VERSION = "0.4.3";

class MainWin : public WithMainWinLayout<TopWindow> {
public:
	typedef MainWin CLASSNAME;
	MainWin(String dbname);
	
	void WindowSetup(); 	// setup controls of window
	void StoreSetup();		// store setup to member variables/controls

	void Serialize(Stream &s);
	
	int lang;
	
private:
	int splitter_position;
	
	// internal callbacks
	void when_main_menu(Bar& menu);
	void when_toolbar(Bar& bar);
	void when_file_menu(Bar& menu);
	void when_tools_menu(Bar& menu);
	void when_stats_menu(Bar& menu);
	void when_search_menu(Bar& menu);
	void when_help_menu(Bar& menu);
	void when_about();
	void find_client(SqlSelect &sel);
	void when_find_client();
	void when_insert_client();
	void when_update_client();
	void when_client_change_row();
	void when_insert_patient();
	void when_update_patient();
	void when_patiens_menu(Bar &menu);
	void when_patients_records();
	void when_edit_lysset();
	void when_lov_dialog();
	void when_inv_data_dlg();
	void when_reset_accounting();
	void when_change_city();
	void when_change_street();
	void when_new_species();
	void when_new_breed();
	void when_new_title();
	void when_complex_statistics();
	void when_invoice_list();
	void when_change_first_name();
	void when_change_last_name();
	void when_change_patient_name();
	void when_find_invoice();
	void when_find_by_lysset();
	void when_find_by_patient();
	void when_options();
	void when_invoice_mass_print();
	void when_dummy() {PromptOK("Not implemented yet");}
	Event<Bar&> patients_default_menu;
	
	void fill_controls();
	void fill_breed_all();
	void fill_breed();
	void fill_street_all();
	void fill_street();
	
	int get_next_lysset();
	
	void prepareInvoiceFormatter();
	
	// internal controls (not declared in Layout file)
	// toolbar
	//ToolButton new_client_btn;
	//ToolButton new_patient_btn;
	EditString find;
	
	// client
	EditString first_name, last_name, phone1, phone2, email, house_no, note;
	EditString ic, dic, icdph;
	DropGrid title, city, street;
	
	// patient
	EditString name, pnote;
	DropDate birthdate;
	DropGrid species, breed, sex;
	Option is_dead;
	DataPusher lysset;
	
	InvoiceFormatter formatter;
};

enum RecordItemType
{
	RIT_LABOR = 0,
	RIT_MATERIAL
};

class RecordsWin : public WithRecordsWinLayout<TopWindow> {
public:
	typedef RecordsWin CLASSNAME;
	RecordsWin(int lang);
	
	bool is_new;
	int client, patient;
	double	sum_dbl;
	int lang;
	
	void checksave_record_data(int recordsCursorId);
	void generate_cash_register_commands();

private:

	// internal callbacks
	void when_records_change_row();
	void when_records_new_inserted();
	void when_labor_item_product_changes();
	void when_recompute_labor_price();
	void when_recompute_material_price();
	void when_labor_item_inserted();
	void when_labor_item_updated();
	void when_labor_item_deleted();
	void when_material_item_inserted();
	void when_material_item_updated();
	void when_material_item_deleted();
	void when_material_item_product_changes();
	void when_print_record();
	void when_text_menu(Bar& menu);
	void when_phrases_menu(Bar& menu);
	void apply_phrase(int id);
	void when_new_phrase();
	void sum_items(RecordItemType item_type = RIT_LABOR, int removed_idx = -1);
	// for rounding double values
	double custom_round_double(double d, int n);
	
	// internal controls - labor_items
	DropGrid 			labor_item_product, labor_item_unit;
	EditDoubleNotNull	labor_item_amount, labor_item_price, labor_item_final_price;
	EditInt				labor_item_plu;
	
	// internal controls - material_items
	DropGrid 			material_item_product, material_item_unit;
	EditDoubleNotNull	material_item_amount, material_item_price, material_item_final_price;
	
	InvoiceFormatter formatter;
};

class SpeciesLovWin : public WithSpeciesLovLayout<ParentCtrl> {
public:
	typedef SpeciesLovWin CLASSNAME;
	SpeciesLovWin();

private:
	void when_species_change_row();
	void when_insert_species();
	void when_update_species();
	void when_insert_breed();
	void when_update_breed();
	
	EditStringNotNull species_name, breed_name;
};

class TitleLovWin : public WithTitleLovLayout<ParentCtrl> {
public:
	typedef TitleLovWin CLASSNAME;
	TitleLovWin();

private:
	void when_insert();
	void when_update();
	
	EditStringNotNull name;
};

class ProductLovWin : public WithProductLovLayout<ParentCtrl> {
public:
	typedef ProductLovWin CLASSNAME;
	ProductLovWin();

private:
	void when_insert();
	void when_update();
	void when_new_unit();
	void when_valid_to();
	
	EditStringNotNull 	name;
	EditDoubleNotNull 	price;
	DropGrid			unit;
	DropDate			valid_to;
	EditInt				plu;
};

class AddressLovWin : public WithAddressLovLayout<ParentCtrl> {
public:
	typedef AddressLovWin CLASSNAME;
	AddressLovWin();

private:
	void when_city_change_row();
	void when_insert_city();
	void when_update_city();
	void when_insert_street();
	void when_update_street();
	
	EditStringNotNull	city, district, province;
	EditString			zip, postoffice;
	EditStringNotNull	street, street_zip, street_postoffice;
};

class PhraseLovWin : public WithPhraseLovLayout<ParentCtrl> {
public:
	typedef PhraseLovWin CLASSNAME;
	PhraseLovWin();
	void check_save_record(int prevId);
		
private:
	void when_names_change_row();
	void when_insert_row();
	void when_delete_row();
	void when_update_row();
	
	EditStringNotNull 	name;
};

class ComplexStatsWin : public WithComplexStatsLayout<TopWindow>
{
public:
	typedef ComplexStatsWin CLASSNAME;
	ComplexStatsWin();
	
	void refresh();
	void when_interval_changes();

private:
	GridCtrl products, clients;
};

class InvoiceChoiceWin : public WithInvoiceChoiceWinLayout<TopWindow> {
public:
	typedef InvoiceChoiceWin CLASSNAME;
	InvoiceChoiceWin(int lang);
private:
	int lang;
};

class LoginWin : public WithLoginWinLayout<TopWindow> {
public:
	typedef LoginWin CLASSNAME;
	LoginWin();
	
	void Serialize(Stream &s);
};

class InvoiceDataWin : public WithInvoiceDataWinLayout<TopWindow> {
public:
	typedef InvoiceDataWin CLASSNAME;
	InvoiceDataWin();
	
	void Serialize(Stream &s);
};

enum En_payment_type
{
	ptCurrency = 0,
	ptBankTransfer
};

enum En_stats_interval
{
	siDay = 0,
	siMonth,
	siDateRange
};


/*
 * CONVERTERS
 */
class ConvertLysset : public ConvertString
{
	public: ConvertLysset() : ConvertString(INT_MAX, false) {}
};

struct DispIsDead : GridDisplay
{
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		if(!val.IsNull())
		{
			bool b = (val.ToString() == "0") ? false : true;;
			SetCenterImage(b ? Images::Dead : Images::Alive);
		}
		else
			SetCenterImage(Images::Alive); //default (not set) is alive
		GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
	}
};

void LoadProducts (DropGrid& list, Date valid_to);
void LoadLov (DropGrid& list, const SqlId table, const SqlId name_column);
void LoadLov (DropGrid& list, const SqlId table);
void LoadLov (DropGrid& list, const SqlId table, const SqlId name_column, const SqlId fk, const Value &fk_value);
void LoadLov (DropGrid& list, const SqlId table, const SqlId fk, const Value &fk_value);

#ifdef _DEBUG
void initDebugDB(PostgreSQLSession &session);
#endif

/*
 * INVOICE
 */
enum En_inv_list_item
{
	iliInvNum = 0,
	iliDate,
	iliTotal
};

class InvoiceList
{
public:
	typedef InvoiceList CLASSNAME;
	InvoiceList(int lang, Date date_from, Date date_to);
	
	bool prepare();
	Report& report();
private:
	Report invoiceList;
	Date from, to;
	int lang;
	InvoiceDataWin inv_data; //dialog with invoice data
	//items
	Vector< VectorMap<int, String> > inv_list_items;
	double summary_price;
	
	void formatHeader(StringBuffer &buf);
	void formatCompanyData(StringBuffer &buf);
	void formatItems(StringBuffer &buf);
};

String convertEurToSkk(double eur);

#endif
