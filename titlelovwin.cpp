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

TitleLovWin::TitleLovWin()
{
	titles.AddIndex(ID);
	titles.AddColumn(NAME, t_("Name")).Edit(name.MaxChars(30));
	titles.SetToolBar(true, BarCtrl::BAR_TOP);
	titles.WhenInsertRow = THISBACK(when_insert);
	titles.WhenUpdateRow = THISBACK(when_update);
	
	SQL & Select(ID, NAME).From(LOV_TITLE).OrderBy(NAME);
	while(SQL.Fetch())
	      titles.Add(SQL);

	if (!titles.IsEmpty())
		titles.SetCursor(0);
}

void TitleLovWin::when_insert()
{
	SQL & Insert(LOV_TITLE)
	    (NAME, titles(NAME))
	;
}

void TitleLovWin::when_update()
{
	SQL & ::Update(LOV_TITLE)
		(NAME, titles(NAME))
	.Where(ID == titles(ID));
}
