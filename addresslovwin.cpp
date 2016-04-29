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

AddressLovWin::AddressLovWin()
{
	cities.AddIndex(ID);
	cities.AddColumn(CITY, t_("City")).Edit(city.MaxChars(100));
	cities.AddColumn(DISTRICT, t_("District")).Edit(district.MaxChars(100));
	cities.AddColumn(PROVINCE, t_("Province")).Edit(province.MaxChars(2));
	cities.AddColumn(PSC, t_("ZIP")).Edit(zip.MaxChars(6));
	cities.AddColumn(POST_OFFICE, t_("Post office")).Edit(postoffice.MaxChars(100));
	cities.SetToolBar(true, BarCtrl::BAR_TOP);
	cities.WhenChangeRow = THISBACK(when_city_change_row);
	cities.WhenInsertRow = THISBACK(when_insert_city);
	cities.WhenUpdateRow = THISBACK(when_update_city);
	
	streets.AddIndex(ID);
	streets.AddColumn(STREET, t_("Street")).Edit(street.MaxChars(100));
	streets.AddColumn(PSC, t_("ZIP")).Edit(street_zip.MaxChars(6));
	streets.AddColumn(POST_OFFICE, t_("Post office")).Edit(street_postoffice.MaxChars(100));
	streets.SetToolBar(true, BarCtrl::BAR_TOP);
	streets.WhenInsertRow = THISBACK(when_insert_street);
	streets.WhenUpdateRow = THISBACK(when_update_street);
	
	SQL & Select(ID, CITY, DISTRICT, PROVINCE, PSC, POST_OFFICE).From(LOV_CITY).OrderBy(CITY);
	while(SQL.Fetch())
	      cities.Add(SQL);

	if (!cities.IsEmpty())
		cities.SetCursor(0);
	else
		streets.Appending(false);
}

void AddressLovWin::when_city_change_row()
{
	streets.Clear();
	SQL & Select(ID, STREET, PSC, POST_OFFICE).From(LOV_STREET).Where(CITY_ID == cities(ID)).OrderBy(STREET);
	while(SQL.Fetch())
	      streets.Add(SQL);
}

void AddressLovWin::when_insert_city()
{
	SQL & Insert(LOV_CITY)
	    (CITY, cities(CITY))
	    (DISTRICT, cities(DISTRICT))
	    (PROVINCE, cities(PROVINCE))
	    (PSC, cities(PSC))
	    (POST_OFFICE, cities(POST_OFFICE))
	;
	
	SQL & Select(SqlMax(ID)).From(LOV_CITY);
	if (SQL.Fetch())
		cities.Set(ID, SQL[0]);
		
	streets.Appending(true);
}

void AddressLovWin::when_update_city()
{
	SQL & ::Update(LOV_CITY)
		(CITY, cities(CITY))
	    (DISTRICT, cities(DISTRICT))
	    (PROVINCE, cities(PROVINCE))
	    (PSC, cities(PSC))
	    (POST_OFFICE, cities(POST_OFFICE))
	.Where(ID == cities(ID));
}

void AddressLovWin::when_insert_street()
{
	SQL & Insert(LOV_STREET)
	    (STREET, streets(STREET))
	    (PSC, streets(PSC))
	    (POST_OFFICE, streets(POST_OFFICE))
	    (CITY_ID, cities(ID))
	;
}

void AddressLovWin::when_update_street()
{
	SQL & ::Update(LOV_STREET)
		(STREET, streets(STREET))
	    (PSC, streets(PSC))
	    (POST_OFFICE, streets(POST_OFFICE))
	.Where(ID == streets(ID));
}
