#pragma once

#include "../src/generate/op_include_all.h"
#include "../src/singleton.h"
#include "../src/thread_safe_stl.h"
#include "../src/msg_base.h"

//thread safe
class msg_pool : public singleton<msg_pool>
{
	private:
		std::vector<thread_safe_list<msg_base*> > m_free_msg_list;
	public:
		msg_pool()
		{
			extern void register_new_msg_handler(uint64* p_g_new_msg_handler);
			memset(msg_base::g_new_msg_handler, 0, sizeof(msg_base::g_new_msg_handler));
			register_new_msg_handler(msg_base::g_new_msg_handler);

			m_free_msg_list.resize(OP_COUNT);
		}
		~msg_pool()
		{
			uint32 list_size = m_free_msg_list.size();
			for (size_t i = 0; i < list_size; i++)
			{
				thread_safe_list<msg_base*>& t_msg_list = m_free_msg_list[i];
				t_msg_list.for_each([](msg_base* p_msg)
				{
					SAFE_DEL(p_msg);
				});
				t_msg_list.clear();
			}
		}
		msg_base* get_msg(uint32 opcode)
		{
			msg_base* p_msg = nullptr;
			thread_safe_list<msg_base*>& t_msg_list = m_free_msg_list[opcode];
			t_msg_list.lock();
			if (t_msg_list.empty())
			{
				t_msg_list.unlock();
				p_msg = msg_base::new_msg(opcode);
			}
			else
			{
				p_msg = t_msg_list.get_front();
				t_msg_list.unlock();
			}
			return p_msg;
		}
		void put_msg(msg_base* p_msg)
		{
			p_msg->reset();
			thread_safe_list<msg_base*>& t_msg_list = m_free_msg_list[p_msg->get_op()];
			t_msg_list.push_back(p_msg);
		}
};

#define g_msg_pool ((*(msg_pool::instance())))

