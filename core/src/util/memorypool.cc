#include"memorypool.h"

using namespace core;

void fixed_memory_pool::init(u32 area_size, u32 num) {
	if(active || wait || ! memory.empty()) {
		return;
	}
	auto struct_size = sizeof(elem);
	auto whole_size = struct_size + area_size;
	memory.resize(whole_size * (num + 2)); 
	wait = (elem*)&memory[0];
	active = (elem*)&memory[whole_size]; 
	wait->next = wait->prev = wait;
	wait->id = -1;
	active->next = active->prev = active;
	active->id = -2; 
	for(u64 i = 0, e = whole_size * 2; i < num; ++i, e += whole_size) {
		elem * p = (elem*)&memory[e];
		p->id = (i32)i;
		p->work = &memory[e + struct_size];
		p->next = wait->next;
		p->prev = wait;
		wait->next = wait->next->prev = p;
	}
	num_wait = num;
}

fixed_memory_pool::elem * fixed_memory_pool::pop() {
	if( ! active || ! wait || memory.empty()) {
		return nullptr;
	}
	auto p = wait->next;
	if(p == wait) {
		return nullptr;
	}
	p->next->prev = wait;
	wait->next = p->next;
	p->next = active->next;
	p->prev = active;
	active->next = active->next->prev = p;
	--num_wait;
	return p;
}

void fixed_memory_pool::push(elem * p) {
	if( ! active || ! wait || memory.empty()) {
		return;
	}
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = wait->next;
	p->prev = wait;
	wait->next = wait->next->prev = p;
	++num_wait;
}

void fixed_memory_pool::clear() {
	memory.clear();
	wait = active = nullptr;
}

u32 fixed_memory_pool::availables() const {
	return num_wait;
}
