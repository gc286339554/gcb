#include "opcode_test_01.h"

msg_base* new_msg_TEST_LUA_TO_CPP_PACKET()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_LUA_TO_CPP_PACKET());
	p_obj->set_op(TEST_LUA_TO_CPP_PACKET);
	p_obj->set_stream_mode(false);
	return p_obj;
}
msg_base* new_msg_TEST_LUA_TO_CPP_STREAM()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_LUA_TO_CPP_STREAM());
	p_obj->set_op(TEST_LUA_TO_CPP_STREAM);
	p_obj->set_stream_mode(true);
	return p_obj;
}
