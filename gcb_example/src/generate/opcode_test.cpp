#include "opcode_test.h"

msg_base* new_msg_TEST_CPP_TO_LUA_STREAM()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_CPP_TO_LUA_STREAM());
	p_obj->set_op(TEST_CPP_TO_LUA_STREAM);
	p_obj->set_stream_mode(true);
	return p_obj;
}
msg_base* new_msg_TEST_CPP_SERIALIZE_PACKET()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_CPP_SERIALIZE_PACKET());
	p_obj->set_op(TEST_CPP_SERIALIZE_PACKET);
	p_obj->set_stream_mode(false);
	return p_obj;
}
msg_base* new_msg_TEST_CPP_TO_LUA_PACKET()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_CPP_TO_LUA_PACKET());
	p_obj->set_op(TEST_CPP_TO_LUA_PACKET);
	p_obj->set_stream_mode(false);
	return p_obj;
}
msg_base* new_msg_TEST_CPP_SERIALIZE_STREAM()
{
	msg_base* p_obj = (msg_base*)(new msg_TEST_CPP_SERIALIZE_STREAM());
	p_obj->set_op(TEST_CPP_SERIALIZE_STREAM);
	p_obj->set_stream_mode(true);
	return p_obj;
}
