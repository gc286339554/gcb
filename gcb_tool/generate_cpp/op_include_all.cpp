#include "op_include_all.h"


void register_new_msg_handler(uint64* p_g_new_msg_handler)
{
	p_g_new_msg_handler[TEST_LUA_TO_CPP_PACKET] = reinterpret_cast<uint64>(new_msg_TEST_LUA_TO_CPP_PACKET);
	p_g_new_msg_handler[TEST_LUA_TO_CPP_STREAM] = reinterpret_cast<uint64>(new_msg_TEST_LUA_TO_CPP_STREAM);
	p_g_new_msg_handler[TEST_CPP_TO_LUA_STREAM] = reinterpret_cast<uint64>(new_msg_TEST_CPP_TO_LUA_STREAM);
	p_g_new_msg_handler[TEST_CPP_SERIALIZE_PACKET] = reinterpret_cast<uint64>(new_msg_TEST_CPP_SERIALIZE_PACKET);
	p_g_new_msg_handler[TEST_CPP_TO_LUA_PACKET] = reinterpret_cast<uint64>(new_msg_TEST_CPP_TO_LUA_PACKET);
	p_g_new_msg_handler[TEST_CPP_SERIALIZE_STREAM] = reinterpret_cast<uint64>(new_msg_TEST_CPP_SERIALIZE_STREAM);
}
