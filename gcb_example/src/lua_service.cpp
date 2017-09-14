#include "lua_service.h"

static void dp_start_write(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->start_write();
}
static void dp_start_read(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->start_read();
}
static uint32 dp_get_op(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	return msg->get_op();
}
static void dp_set_op(uint64 msg_p, f64 val)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->set_op(val);
}
static void dp_end_write(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->end_write();
}
static uint64 dp_malloc_data_packet(uint32 opcode)
{
	return reinterpret_cast<uint64>(g_msg_pool.get_msg(opcode));
}
static void dp_free_data_packet(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	g_msg_pool.put_msg(reinterpret_cast<msg_base*>(msg));
}
static void dp_put_num(uint64 msg_p, f64 val)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->put_f64(val);
}
static f64 dp_get_num(uint64 msg_p)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	return msg->get_f64();
}

static void dp_put_string(uint64 msg_p, const char* val)
{
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	msg->put_string(val);
}
static const char* dp_get_string(uint64 msg_p, uint64 g_service_this)
{
	lua_service* t_service_this = reinterpret_cast<lua_service*>(g_service_this);
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);
	t_service_this->m_lua_bridge_str = msg->get_string();
	return t_service_this->m_lua_bridge_str.c_str();
}

#define DEF_PUT_GET_FUNC(_type_) static void dp_put_##_type_(uint64 msg_p, _type_ val)\
{\
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);\
	msg->put_##_type_(val);\
}\
static _type_ dp_get_##_type_(uint64 msg_p)\
{\
	data_packet* msg = reinterpret_cast<data_packet*>(msg_p);\
	return msg->get_##_type_();\
}
DEF_PUT_GET_FUNC(bool)
DEF_PUT_GET_FUNC(uint8)
DEF_PUT_GET_FUNC(int8)
DEF_PUT_GET_FUNC(uint16)
DEF_PUT_GET_FUNC(int16)
DEF_PUT_GET_FUNC(uint32)
DEF_PUT_GET_FUNC(int32)
DEF_PUT_GET_FUNC(uint64)
DEF_PUT_GET_FUNC(int64)
DEF_PUT_GET_FUNC(f32)
DEF_PUT_GET_FUNC(f64)

#undef DEF_PUT_GET_FUNC


static void lua_send_to_cpp(uint64 msg_p, uint64 g_service_this)
{
	lua_service* t_service_this = reinterpret_cast<lua_service*>(g_service_this);
	msg_base* msg = reinterpret_cast<msg_base*>(msg_p);
	t_service_this->m_cpp_msg_handler(msg);
}


lua_service::lua_service(const char* lua_enter_file)
:m_lua_enter_file(lua_enter_file)
{
}
lua_service::~lua_service()
{
	if (L)
	{
		lua_close(L);
		L = nullptr;
	}
}
void lua_service::init()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_tinker::def(L, "dp_start_write", dp_start_write);
	lua_tinker::def(L, "dp_start_read", dp_start_read);
	lua_tinker::def(L, "dp_get_op", dp_get_op);
	lua_tinker::def(L, "dp_set_op", dp_set_op);
	lua_tinker::def(L, "dp_end_write", dp_end_write);
	lua_tinker::def(L, "dp_malloc_data_packet", dp_malloc_data_packet);
	lua_tinker::def(L, "dp_free_data_packet", dp_free_data_packet);
	lua_tinker::def(L, "dp_put_num", dp_put_num);
	lua_tinker::def(L, "dp_get_num", dp_get_num);
	lua_tinker::def(L, "dp_put_bool", dp_put_bool);
	lua_tinker::def(L, "dp_get_bool", dp_get_bool);
	lua_tinker::def(L, "dp_put_string", dp_put_string);
	lua_tinker::def(L, "dp_get_string", dp_get_string);

	lua_tinker::def(L, "dp_put_uint8", dp_put_uint8);
	lua_tinker::def(L, "dp_get_uint8", dp_get_uint8);
	lua_tinker::def(L, "dp_put_int8", dp_put_int8);
	lua_tinker::def(L, "dp_get_int8", dp_get_int8);

	lua_tinker::def(L, "dp_put_uint16", dp_put_uint16);
	lua_tinker::def(L, "dp_get_uint16", dp_get_uint16);
	lua_tinker::def(L, "dp_put_int16", dp_put_int16);
	lua_tinker::def(L, "dp_get_int16", dp_get_int16);

	lua_tinker::def(L, "dp_put_uint32", dp_put_uint32);
	lua_tinker::def(L, "dp_get_uint32", dp_get_uint32);
	lua_tinker::def(L, "dp_put_int32", dp_put_int32);
	lua_tinker::def(L, "dp_get_int32", dp_get_int32);

	lua_tinker::def(L, "dp_put_uint64", dp_put_uint64);
	lua_tinker::def(L, "dp_get_uint64", dp_get_uint64);
	lua_tinker::def(L, "dp_put_int64", dp_put_int64);
	lua_tinker::def(L, "dp_get_int64", dp_get_int64);

	lua_tinker::def(L, "dp_put_f32", dp_put_f32);
	lua_tinker::def(L, "dp_get_f32", dp_get_f32);
	lua_tinker::def(L, "dp_put_f64", dp_put_f64);
	lua_tinker::def(L, "dp_get_f64", dp_get_f64);
	lua_tinker::def(L, "lua_send_to_cpp", lua_send_to_cpp);


	
	uint64 t_this = reinterpret_cast<uint64>(this);
	lua_tinker::decl(L, "g_service_this", t_this);
	lua_tinker::dofile(L, m_lua_enter_file.c_str());
	
}
void lua_service::update(uint32 ms_delay)
{
	lua_tinker::call<void, uint32>(L, "update", ms_delay);
}

void lua_service::send_to_lua(msg_base* msg)
{
	if (msg->is_stream_mode())
	{
		lua_tinker::call<void, uint64>(L, "binary_service_msg_handler", reinterpret_cast<uint64>(msg));
	}
	else
	{
		lua_tinker::call<void, uint64>(L, "lua_service_msg_handler", reinterpret_cast<uint64>(msg));
	}
}
void  lua_service::notify_send_to_cpp(std::function<void(msg_base*)> handler)
{
	m_cpp_msg_handler = handler;
	lua_tinker::call<void>(L, "notify_send_to_cpp");
}


