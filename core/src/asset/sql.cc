#include<cstring>
#include<sqlite3.h>
#include"sql.h"

using namespace core;

struct sql::Impl {
	sqlite3 *db { nullptr };
	sqlite3_stmt *stmt { nullptr };
};

sql::sql() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
}

sql::~sql() {
	finish();
	close();
}

bool sql::open(const char * db) {
	if( ! db) {
		return false;
	}
	if(impl->db) {
		return false;
	}
	if(sqlite3_open(db, &impl->db) != SQLITE_OK) {
		return false;
	}
	return true;
}

bool sql::execute(const char * sql) {
	auto ret = sqlite3_prepare(impl->db, sql, (int)strlen(sql), &impl->stmt, NULL);
	if(ret != SQLITE_OK) {
		return false;
	}
	sqlite3_reset(impl->stmt);
	return true;
}

void sql::finish() {
	if(impl->stmt) {
		while(SQLITE_DONE != sqlite3_step(impl->stmt)) {}
		sqlite3_finalize(impl->stmt);
		impl->stmt = nullptr;
	}
}

void sql::close() {
	if(impl->db) {
		sqlite3_close(impl->db);
		impl->db = nullptr;
	}
}

bool sql::next() {
	return sqlite3_step(impl->stmt) == SQLITE_ROW;
}

i32 sql::get_int(u32 i) {
	return sqlite3_column_int(impl->stmt, i);
}

i64 sql::get_i64(u32 i) {
	return sqlite3_column_int64(impl->stmt, i);
}

f64 sql::get_f64(u32 i) {
	return sqlite3_column_double(impl->stmt, i);
}

const unsigned char * sql::get_str(u32 i) {
	return sqlite3_column_text(impl->stmt, i);
}

const void * sql::get_data(u32 i) {
	return sqlite3_column_blob(impl->stmt, i);
}
