using init_func = void(*)();

extern init_func _init_array_begin;
extern init_func _init_array_end;

extern char _bss_begin;
extern char _bss_end;

extern "C" void init_globals()
{
	for (auto func = &_init_array_begin; func != &_init_array_end; func ++)
		(*func)();
}

extern "C" void clear_bss()
{
	for (auto ptr = &_bss_begin; ptr != &_bss_end; ptr ++) {
		*ptr = 0;
	}
}
