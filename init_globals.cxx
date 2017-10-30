using init_func = void(*)();

extern init_func _init_array_begin[];
extern init_func _init_array_end;

extern "C" void init_globals()
{
	for (auto idx = 0; _init_array_begin[idx] != _init_array_end; idx ++)
		_init_array_begin[idx]();
}
