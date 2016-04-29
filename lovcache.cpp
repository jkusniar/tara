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

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif
#include <Sql/sch_source.h>

#ifdef _DEBUG
void initDebugDB(PostgreSQLSession &session)
{
	session.SetTrace();
	//schema
	Progress p;
	p.SetText(t_("Creating _DEBUG database"));
	SqlSchema sch(PGSQL);
	StdStatementExecutor se(session);
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		PostgreSQLPerformScript(sch.Upgrade(),se, p);
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
		PostgreSQLPerformScript(sch.Attributes(),se, p);
	}
	if(sch.ScriptChanged(SqlSchema::CONFIG)) {
		PostgreSQLPerformScript(sch.ConfigDrop(),se, p);
		PostgreSQLPerformScript(sch.Config(),se, p);
	}
	sch.SaveNormal();
}
#endif

void LoadProducts (DropGrid& list, Date valid_to) {
	LOG("Loading Products with valid to >= " + AsString(valid_to));
	
	list.Clear();
	SQL & Select(ID, NAME, PLU).From(LOV_PRODUCT)
		.Where(SqlIsNull(VALID_TO) || VALID_TO >= valid_to)
		.OrderBy(NAME);
	while(SQL.Fetch()) {
		list.Add(SQL[ID], SQL[NAME]);
		if (!SQL[PLU].IsNull()) {
			list.GetRow(list.GetCount()-1).SetFont(Font::GetStdFont().Bold(true));
		}
	}
}

void LoadLov (DropGrid& list, const SqlId table, const SqlId name_column) {
	list.Clear();
	SQL & Select(ID, name_column).From(table).OrderBy(name_column);
	while(SQL.Fetch())
		list.Add(SQL[ID], SQL[name_column]);
}

void LoadLov (DropGrid& list, const SqlId table) {
	LoadLov(list, table, NAME);
}

void LoadLov (DropGrid& list, const SqlId table, const SqlId name_column, const SqlId fk, const Value &fk_value) {
	list.Clear();
	SQL & Select(ID, name_column).From(table).Where(fk.Of(table) == fk_value).OrderBy(name_column);
	while(SQL.Fetch())
		list.Add(SQL[ID], SQL[name_column]);
}

void LoadLov (DropGrid& list, const SqlId table, const SqlId fk, const Value &fk_value) {
	LoadLov(list, table, NAME, fk, fk_value);
}

