#include <stdarg.h>
#include"savedata.h"

using namespace engine;

savedata::~savedata() {
	handle.close();
}

bool savedata::open(const char * db, const char * table) {
	if( ! db || ! table) {
		return false;
	}
	if( ! handle.open(db)) {
		return false;
	}
	table_name = table;
	return true;
}

bool savedata::create_table(const schema & columns) {
	if(columns.empty()) {
		return false;
	}
	std::string s = "create table " + table_name + " (";
	for(size_t i = 0, num = columns.size(); i < num; ++i) {
		s += std::get<0>(columns[i]);
		s += " ";
		s += std::get<1>(columns[i]);
		std::string tmp = std::get<2>(columns[i]);
		if( ! tmp.empty()) {
			s += " ";
			s += std::get<2>(columns[i]);
		}
		if(i < num - 1) {
			s += ", ";
		}
	}
	s += ");";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	handle.finish();
	return true;
}

bool savedata::delete_table() {
	std::string s = "drop table " + table_name + ";";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	handle.finish();
	return true;
}

bool savedata::insert_default() {
	std::string s = "insert into " + table_name + " default values;";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	handle.finish();
	return true;
}

bool savedata::insert_value(const coldata & col, const rowdata & row) {
	if(col.empty() || row.empty()) {
		return false;
	}
	std::string s = "insert into " + table_name + "(";
	for(size_t i = 0, size = col.size(); i < size; ++i) {
		s += col[i];
		if(i < size - 1) {
			s += ", ";
		}
	}
	s += ") values";
	for(size_t i = 0, rsize = row.size(), csize = col.size(); i < rsize; ++i) {
		s += "(";
		for(u32 j = 0; j < csize; ++j) {
			s += row[i][j];
			if(j < csize - 1) {
				s += ",";
			}
		}
		s += ")";
		if(i < rsize - 1) {
			s += ",";
		}
	}

	s += ";";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	handle.finish();
	return true;
}

bool savedata::update_value(const coldata & col, const coldata & replace_val, const char * condition) {
	if(col.empty() || replace_val.empty()) {
		return false;
	}
	std::string s = "update " + table_name + " set ";
	for(size_t i = 0, size = col.size(); i < size; ++i) {
		s += col[i];
		s += " = ";
		s += replace_val[i];
		if(i < size - 1) {
			s += ", ";
		}
	}
	if(condition) {
		s += " where ";
		s += condition;
	}
	s += ";";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	handle.finish();
	return true;
}
bool savedata::start_select(const coldata & col, const char * condition) {
	std::string s = "select ";
	for(size_t i = 0, size = col.size(); i < size; ++i) {
		s += col[i];
		if (i < size - 1) {
			s += ", ";
		}
	}
	s += " from " + table_name;

	if(condition) {
		s += " where ";
		s += condition;
	}
	s += ";";
	if( ! handle.execute(s.c_str())) {
		return false;
	}
	return next_row();
}

bool savedata::next_row() {
	return handle.next();
}

void savedata::find_value(u32 column_index, char ** data) {
	*data = (char*)handle.get_str(column_index);
}

void savedata::find_value(u32 column_index, void ** data) {
	*data = (void*)handle.get_data(column_index);
}

void savedata::find_value(u32 column_index, i32 & data) {
	data = handle.get_int(column_index);
}

void savedata::find_value(u32 column_index, i64 & data) {
	data = handle.get_i64(column_index);
}

void savedata::find_value(u32 column_index, f64 & data) {
	data = handle.get_f64(column_index);
}

void savedata::end_select() {
	while(handle.next()) {}
	handle.finish();
}

void savedata::close() {
	handle.close();
}