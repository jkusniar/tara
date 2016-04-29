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

SpeciesLovWin::SpeciesLovWin()
{
	species.AddIndex(ID);
	species.AddColumn(NAME, t_("Name")).Edit(species_name.MaxChars(100));
	species.SetToolBar(true, BarCtrl::BAR_TOP);
	species.WhenChangeRow = THISBACK(when_species_change_row);
	species.WhenInsertRow = THISBACK(when_insert_species);
	species.WhenUpdateRow = THISBACK(when_update_species);
	
	breed.AddIndex(ID);
	breed.AddColumn(NAME, t_("Name")).Edit(breed_name.MaxChars(100));
	breed.SetToolBar(true, BarCtrl::BAR_TOP);
	breed.WhenInsertRow = THISBACK(when_insert_breed);
	breed.WhenUpdateRow = THISBACK(when_update_breed);
	
	SQL & Select(ID, NAME).From(LOV_SPECIES).OrderBy(NAME);
	while(SQL.Fetch())
	      species.Add(SQL);

	if (!species.IsEmpty())
		species.SetCursor(0);
	else
		breed.Appending(false);
}

void SpeciesLovWin::when_species_change_row()
{
	breed.Clear();
	SQL & Select(ID, NAME).From(LOV_BREED).Where(LOV_SPECIES_ID == species(ID)).OrderBy(NAME);
	while(SQL.Fetch())
	      breed.Add(SQL);
}

void SpeciesLovWin::when_insert_species()
{
	SQL & Insert(LOV_SPECIES)
	    (NAME, species(NAME))
	;
	
	SQL & Select(SqlMax(ID)).From(LOV_SPECIES);
	if (SQL.Fetch())
		species.Set(ID, SQL[0]);
	
	breed.Appending(true);
}

void SpeciesLovWin::when_update_species()
{
	SQL & ::Update(LOV_SPECIES)
		(NAME, species(NAME))
	.Where(ID == species(ID));
}

void SpeciesLovWin::when_insert_breed()
{
	SQL & Insert(LOV_BREED)
		(NAME, breed(NAME))
	    (LOV_SPECIES_ID, species(ID))
	;
}

void SpeciesLovWin::when_update_breed()
{
	SQL & ::Update(LOV_BREED)
		(NAME, breed(NAME))
	.Where(ID == breed(ID));
}
