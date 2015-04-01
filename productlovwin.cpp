#include "mainwin.h"
#include "convert.h"

ProductLovWin::ProductLovWin()
{
	products.AddIndex(ID);
	products.AddColumn(NAME, t_("Name")).Edit(name.MaxChars(100));
	products.AddColumn(UNIT_ID, t_("Unit")).Edit(unit).SetConvert(unit);
	products.AddColumn(PRICE, t_("Price")).Edit(price).SetConvert(Single<ConvertMoney>());
	products.AddColumn(PLU, t_("PLU")).Edit(plu);
	products.AddColumn(VALID_TO, t_("Valid to")).Edit(valid_to).SetConvert(valid_to);
	products.SetToolBar(true, BarCtrl::BAR_TOP);
	products.WhenInsertRow = THISBACK(when_insert);
	products.WhenUpdateRow = THISBACK(when_update);
	
	unit.AddPlus(THISBACK(when_new_unit));
	unit.ColorRows(true);
	valid_to.WhenAction = THISBACK(when_valid_to);

	::LoadLov(unit, LOV_UNIT);
	
	SQL & Select(ID, NAME, UNIT_ID, PRICE, PLU, VALID_TO).From(LOV_PRODUCT).OrderBy(NAME);
	while(SQL.Fetch())
	      products.Add(SQL);

	if (!products.IsEmpty())
		products.SetCursor(0);
}

void ProductLovWin::when_insert()
{
	SQL & Insert(LOV_PRODUCT)
	    (NAME, products(NAME))
	    (UNIT_ID, products(UNIT_ID))
	    (PRICE, products(PRICE))
	    (VALID_TO, products(VALID_TO))
	    (PLU, products(PLU))
	;
}

void ProductLovWin::when_update()
{
	SQL & ::Update(LOV_PRODUCT)
		(NAME, products(NAME))
	    (UNIT_ID, products(UNIT_ID))
	    (PRICE, products(PRICE))
	    (VALID_TO, products(VALID_TO))
	    (PLU, products(PLU))
	.Where(ID == products(ID));
}

void ProductLovWin::when_new_unit()
{
	WithInputLovLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("New unit"));
	dlg.label.SetText(t_("Unit name:"));
	if(dlg.Run(true) == IDOK)
	{
		// check whether unit allready exists
		SQL & Select(SqlCountRows()).From(LOV_UNIT).Where(NAME == ~dlg.name);
		if(SQL.Fetch() && SQL[0] == 0)
		{
			// if not, insert and refresh unit cache
			SQL & Insert(LOV_UNIT)
				(NAME, ~dlg.name)
			;
			
			::LoadLov(unit, LOV_UNIT);
			
			// select new unit in dropgrid
			products.Set(UNIT_ID, SQL.GetInsertedId());
		}
		else
			PromptOK(Format(t_("Unit %s already exists."), ~dlg.name));
			
	}
}

void ProductLovWin::when_valid_to() {
	Date dt = valid_to.GetData();
	if (dt.Compare(GetSysDate()) < 0) {
		ErrorOK(t_("Valid to can't be in past"));
		valid_to.Clear();
	}
}
