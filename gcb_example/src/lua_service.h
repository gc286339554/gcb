#pragma once
#include "../src/data_packet.h"
#include "../src/msg_pool.h"
#include "../src/lua_tinker/lua_tinker.h"
extern "C"
{
#include "../src/lua/lua.h"
#include "../src/lua/lualib.h"
#include "../src/lua/lauxlib.h"
};

class lua_service
{
public:
	lua_service(const char* lua_enter_file);
	~lua_service();

	virtual void init();
	virtual void update(uint32 ms_delay);

	void send_to_lua(msg_base* msg);
	void notify_send_to_cpp(std::function<void(msg_base*)> handler);

public:
	std::string m_lua_bridge_str;//通信中转桥梁作用的string,临时方案  不太优雅
	std::function<void(msg_base*)> m_cpp_msg_handler;
protected:
	lua_State* L = nullptr;
	std::string m_lua_enter_file;	
};
