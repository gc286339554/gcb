#pragma once
//S  game server
//G  gate server
//C	 game client
enum op_enum {
	TEST_CPP_SERIALIZE_PACKET				= 1,//serialize packet
	TEST_CPP_SERIALIZE_STREAM				= 2,//serialize stream_mode
	TEST_CPP_TO_LUA_PACKET				= 3,//serialize packet
	TEST_CPP_TO_LUA_STREAM				= 4,//serialize stream_mode
	TEST_END				= 5,//end
	TEST_01_BEGIN				= 6,//begin
	TEST_LUA_TO_CPP_PACKET				= 7,//serialize packet
	TEST_LUA_TO_CPP_STREAM				= 8,//serialize stream_mode
	TEST_01_END				= 9,//end
	TEST_BEGIN				= 0,//begin
	OP_COUNT				= 10
};
