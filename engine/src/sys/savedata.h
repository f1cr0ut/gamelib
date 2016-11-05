#pragma once

#include<string>
#include<tuple>
#include<vector>
#include<core_header.h>

namespace engine {
	class APICALL savedata {
		core::sql handle;
		std::string table_name;
	public:

		// name, type, condition or default value
		typedef std::vector<std::tuple<const char*, const char *, const char *>> schema;
		typedef std::vector<const char *> coldata;
		typedef std::vector<std::vector<const char *>> rowdata;

		savedata() = default;
		~savedata();

		bool open(const char * db, const char * table);

		bool create_table(const schema & columns);
		bool delete_table(); // delete this table

		bool insert_default();

		bool insert_value(const coldata & col, const rowdata & row);

		bool update_value(const coldata & col, const coldata & replace_val, const char * condition = nullptr);

		bool start_select(const coldata & col, const char * condition);
		bool next_row();
		void find_value(u32 column_index, char ** data);
		void find_value(u32 column_index, void ** data);
		void find_value(u32 column_index, i32 & data);
		void find_value(u32 column_index, i64 & data);
		void find_value(u32 column_index, f64 & data);
		void end_select();

		void close();
	};
}