#include "../src/lua_service.h"
void test_cpp_serialize()
{
	msg_TEST_CPP_SERIALIZE_PACKET* msg = (msg_TEST_CPP_SERIALIZE_PACKET*)(msg_pool::instance()->get_msg(TEST_CPP_SERIALIZE_PACKET));

	msg->from_name = "cpp";
	msg->from_id = 0;

	struct_player p1;
	p1.player_name = "zhangsan";
	p1.player_age = 18;
	struct_tel tel11;
	tel11.tel_num = "18200390381";
	tel11.price = 120;
	p1.tels.push_back(tel11);
	struct_tel tel12;
	tel12.tel_num = "13542844610";
	tel12.price = 1200;
	p1.tels.push_back(tel12);
	msg->players.push_back(p1);

	struct_player p2;
	p2.player_name = "lisi";
	p2.player_age = 19;
	struct_tel tel21;
	tel21.tel_num = "110";
	tel21.price = 1120;
	p2.tels.push_back(tel21);
	struct_tel tel22;
	tel22.tel_num = "119";
	tel22.price = 1230;
	p2.tels.push_back(tel22);
	msg->players.push_back(p2);

	msg->country_info.country_num = "zhong_guo";
	msg->country_info.province.push_back("SI_CHUAN");
	msg->country_info.province.push_back("CHONG_GING");

	msg->serialize();

	msg_TEST_CPP_SERIALIZE_PACKET* msg01 = (msg_TEST_CPP_SERIALIZE_PACKET*)(msg_pool::instance()->get_msg(TEST_CPP_SERIALIZE_PACKET));
	msg01->copy_from(msg);
	msg01->parse();
	printf("cpp serialize test packet_mode : %s %d %s %d \n", msg01->from_name.c_str(), msg01->players.size(), msg01->country_info.country_num.c_str(), msg->country_info.province.size());

	msg_pool::instance()->put_msg(msg);
	msg_pool::instance()->put_msg(msg01);
}
void test_cpp_serialize_01()
{
	msg_base* msg = msg_pool::instance()->get_msg(TEST_CPP_SERIALIZE_STREAM);

	msg->start_write()
		.put_int16(10)
		.put_string("hehe")
		.put_bool(true)
		.put("默认")
		.end_write();
	
	msg_TEST_CPP_SERIALIZE_STREAM* msg01 = (msg_TEST_CPP_SERIALIZE_STREAM*)(msg_pool::instance()->get_msg(TEST_CPP_SERIALIZE_STREAM));
	msg01->copy_from(msg);
	msg01->start_read();

	/*
	代码块1  不等价于代码块二， 不能保证printf参数列表中的函数 有序执行
	*/

	/* 
	代码块1
	printf("cpp serialize test stream_mode : %d %s %d %s \n", msg01->get_int16(), msg01->get_string().c_str(), msg01->get_bool(), msg01->get_string().c_str());
	*/

	//代码块2
	int16 num01 = msg01->get_int16();
	std::string str01 = msg01->get_string();
	bool b01 = msg01->get_bool();
	std::string str02 = msg01->get_string();
	printf("cpp serialize test stream_mode : %d %s %d %s \n", num01, str01.c_str(),b01, str02.c_str());
	//代码块2

	msg_pool::instance()->put_msg(msg);
	msg_pool::instance()->put_msg(msg01);
}
void test_cpp_to_lua()
{
	lua_service lua_service01("lua_script/test_lua_service.lua");
	lua_service01.init();

	do
	{//cpp 向 lua 传输数据 包格式
		msg_TEST_CPP_TO_LUA_PACKET* msg = (msg_TEST_CPP_TO_LUA_PACKET*)(msg_pool::instance()->get_msg(TEST_CPP_TO_LUA_PACKET));

		msg->from_name = "cpp";
		msg->from_id = 0;

		struct_player p1;
		p1.player_name = "zhangsan";
		p1.player_age = 18;
		struct_tel tel11;
		tel11.tel_num = "18200390381";
		tel11.price = 120;
		p1.tels.push_back(tel11);
		struct_tel tel12;
		tel12.tel_num = "13542844610";
		tel12.price = 1200;
		p1.tels.push_back(tel12);
		msg->players.push_back(p1);

		struct_player p2;
		p2.player_name = "lisi";
		p2.player_age = 19;
		struct_tel tel21;
		tel21.tel_num = "110";
		tel21.price = 1120;
		p2.tels.push_back(tel21);
		struct_tel tel22;
		tel22.tel_num = "119";
		tel22.price = 1230;
		p2.tels.push_back(tel22);
		msg->players.push_back(p2);

		msg->country_info.country_num = "zhong_guo";
		msg->country_info.province.push_back("SI_CHUAN");
		msg->country_info.province.push_back("CHONG_GING");

		msg->serialize();

		lua_service01.send_to_lua(msg);
		msg_pool::instance()->put_msg(msg);
	} while (0);

	do
	{//cpp 向 lua 传输数据 二进制
		msg_base* msg = msg_pool::instance()->get_msg(TEST_CPP_TO_LUA_STREAM);

		msg->start_write()
			.put_f64(10)
			.put_string("hehe")
			.put_bool(true)
			.put("默认")
			.end_write();

		lua_service01.send_to_lua(msg);
		msg_pool::instance()->put_msg(msg);

	} while (0);
}
void test_lua_to_cpp()
{
	lua_service lua_service02("lua_script/test_lua_service_01.lua");
	lua_service02.init();

	lua_service02.notify_send_to_cpp(
		[](msg_base* msg)
	{
		uint32 opcode = msg->get_op();
		if (msg->is_stream_mode())
		{
			if (TEST_LUA_TO_CPP_STREAM == opcode)
			{
				msg_TEST_LUA_TO_CPP_STREAM* msg01 = (msg_TEST_LUA_TO_CPP_STREAM*)(msg_pool::instance()->get_msg(TEST_LUA_TO_CPP_STREAM));
				msg01->copy_from(msg);
				msg01->start_read();
				msg01->get_bool();
				msg01->get_bool();
				printf("int32 is %d \n", msg01->get_int32());
				printf("string is %s \n", msg01->get_string().c_str());
				printf("string is %s \n", msg01->get_string().c_str());
			}
		}
		else
		{
			if (TEST_LUA_TO_CPP_PACKET == opcode)
			{
				msg_TEST_LUA_TO_CPP_PACKET* msg01 = (msg_TEST_LUA_TO_CPP_PACKET*)(msg_pool::instance()->get_msg(TEST_LUA_TO_CPP_PACKET));
				msg01->copy_from(msg);
				msg01->parse();
				printf("TEST_LUA_TO_CPP_STREAM\n");
				printf("from_name: %s \n", msg01->from_name.c_str());
				printf("dog_name01: %s \n", msg01->dogs[1].dog_name.c_str());
			}
		}
	});
		
}

int main()
{
	msg_pool::newInstance();

	//cpp层 序列化 和 反序列化
	test_cpp_serialize();
	//cpp层 序列化 和 反序列化, 使用纯二进制模式
	test_cpp_serialize_01();
	//cpp层 向 lua层 传输数据
	test_cpp_to_lua();
	//lua层 向 cpp层 传输数据
	test_lua_to_cpp();

	while (true)
	{
		std::chrono::milliseconds dura(1000);
		this_thread::sleep_for(dura);
	}
	msg_pool::deleteInstance();
    return 0;
}

