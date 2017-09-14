#pragma once
#include "../msg_base.h"

using namespace std;
/***************************start struct define***********************************/
class struct_dog
{
	public:
		void serialize(uint8* & data)
		{
			do 
			{ 
				uint32 s_size = dog_name.size(); 
				memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
				memcpy(data, dog_name.c_str(), s_size); data += s_size;
			} while (0);
			memcpy(data, &dog_age, sizeof(dog_age)); data += sizeof(dog_age);

		};
		void parse(uint8* & data)
		{
			do 
			{ 
				dog_name.clear();
				uint32 s_size = *((uint32*)data); data += sizeof(uint32);
				dog_name.assign((char*)data,s_size);data += s_size;
			} while (0);
			memcpy(&dog_age, data, sizeof(dog_age)); data += sizeof(dog_age);

		};
		void reset()
		{
			dog_name.clear();
			memset(&dog_age, 0, sizeof(dog_age)); 

		}
		uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += dog_name.size();
			data_size += sizeof(dog_age);

			return data_size;
		}
	public:
		string	dog_name;
		uint32	dog_age;

};
/***************************end struct define*************************************/
//serialize packet
class msg_TEST_LUA_TO_CPP_PACKET : public msg_base
{
	public:
		virtual void serialize()
		{
			uint32 s_size = get_data_size();
			if(s_size > 0) re_malloc(s_size);
			*((uint32*)(m_buff + DATA_HEAD_LEN)) = m_op;
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
			do 
			{ 
				uint32 s_size = from_name.size(); 
				memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
				memcpy(data, from_name.c_str(), s_size); data += s_size;
			} while (0);
			memcpy(data, &from_id, sizeof(from_id)); data += sizeof(from_id);
			do 
			{ 
				uint32 v_size = dogs.size(); 
				memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_dog& sub = dogs[i];
					sub.serialize(data);

				}
			} while (0);
		
			m_data_len = data - m_buff;
			*((uint32*)(m_buff)) = m_data_len;
		};
		virtual void parse()
		{
			assert(*((uint32*)(m_buff + DATA_HEAD_LEN)) == m_op);
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
			do 
			{ 
				from_name.clear();
				uint32 s_size = *((uint32*)data); data += sizeof(uint32);
				from_name.assign((char*)data,s_size);data += s_size;
			} while (0);
			memcpy(&from_id, data, sizeof(from_id)); data += sizeof(from_id);
			do 
			{ 
				dogs.clear();
				uint32 v_size = *((uint32*)data); data += sizeof(uint32);
				dogs.resize(v_size);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_dog& sub = dogs[i];
					sub.parse(data);

				}
			} while (0);
		
		};
		virtual void reset()
		{
			msg_base::reset();
			from_name.clear();
			memset(&from_id, 0, sizeof(from_id)); 
			dogs.clear();

		}
		virtual uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += from_name.size();
			data_size += sizeof(from_id);
			do 
			{ 
				uint32 v_size = dogs.size();
				for(uint32 i=0; i<v_size; i++)
				{
					struct_dog& sub = dogs[i];
					data_size += sub.get_data_size();

				}
			} while (0);

			return data_size;
		}
	public:
		string	from_name;
		uint32	from_id;
		vector<struct_dog>	dogs;

};
extern msg_base* new_msg_TEST_LUA_TO_CPP_PACKET();
//serialize stream_mode
class msg_TEST_LUA_TO_CPP_STREAM : public msg_base
{
	public:
		virtual void serialize()
		{
			uint32 s_size = get_data_size();
			if(s_size > 0) re_malloc(s_size);
			*((uint32*)(m_buff + DATA_HEAD_LEN)) = m_op;
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
		
			m_data_len = data - m_buff;
			*((uint32*)(m_buff)) = m_data_len;
		};
		virtual void parse()
		{
			assert(*((uint32*)(m_buff + DATA_HEAD_LEN)) == m_op);
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
		
		};
		virtual void reset()
		{
			msg_base::reset();

		}
		virtual uint32 get_data_size()
		{
			uint32 data_size = 0;

			return data_size;
		}
	public:

};
extern msg_base* new_msg_TEST_LUA_TO_CPP_STREAM();
