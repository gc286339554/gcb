#pragma once

#include "../src/data_packet.h"
#include "../src/generate/op_enum.h"

class msg_base;
typedef msg_base* (*new_msg_handler)();
class msg_base : public data_packet
{
	protected:
		//使用手工 get和put方式，不使用serialize 和 parse
		bool m_stream_mode = false;
	public:
		msg_base() : data_packet(1024){};
		virtual ~msg_base() {};
		virtual void serialize() = 0;
		virtual void parse() = 0;
		virtual void reset(){ data_packet::reset(); };
		bool is_stream_mode() { return m_stream_mode; };
		void set_stream_mode(bool mode) { m_stream_mode = mode; };
		static msg_base* new_msg(uint32 opcode)
		{
			if (opcode > OP_COUNT || g_new_msg_handler[opcode] == 0)
			{
				return nullptr;
			}
			return (reinterpret_cast<new_msg_handler>(g_new_msg_handler[opcode]))();
		}
		static uint64 g_new_msg_handler[OP_COUNT];
};


