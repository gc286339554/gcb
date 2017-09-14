#pragma once
#include "../msg_base.h"

using namespace std;
/***************************start struct define***********************************/
class struct_tel
{
	public:
		void serialize(uint8* & data)
		{
			do 
			{ 
				uint32 s_size = tel_num.size(); 
				memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
				memcpy(data, tel_num.c_str(), s_size); data += s_size;
			} while (0);
			memcpy(data, &price, sizeof(price)); data += sizeof(price);

		};
		void parse(uint8* & data)
		{
			do 
			{ 
				tel_num.clear();
				uint32 s_size = *((uint32*)data); data += sizeof(uint32);
				tel_num.assign((char*)data,s_size);data += s_size;
			} while (0);
			memcpy(&price, data, sizeof(price)); data += sizeof(price);

		};
		void reset()
		{
			tel_num.clear();
			memset(&price, 0, sizeof(price)); 

		}
		uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += tel_num.size();
			data_size += sizeof(price);

			return data_size;
		}
	public:
		string	tel_num;
		uint32	price;

};
class struct_player
{
	public:
		void serialize(uint8* & data)
		{
			do 
			{ 
				uint32 s_size = player_name.size(); 
				memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
				memcpy(data, player_name.c_str(), s_size); data += s_size;
			} while (0);
			memcpy(data, &player_age, sizeof(player_age)); data += sizeof(player_age);
			do 
			{ 
				uint32 v_size = tels.size(); 
				memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_tel& sub = tels[i];
					sub.serialize(data);

				}
			} while (0);

		};
		void parse(uint8* & data)
		{
			do 
			{ 
				player_name.clear();
				uint32 s_size = *((uint32*)data); data += sizeof(uint32);
				player_name.assign((char*)data,s_size);data += s_size;
			} while (0);
			memcpy(&player_age, data, sizeof(player_age)); data += sizeof(player_age);
			do 
			{ 
				tels.clear();
				uint32 v_size = *((uint32*)data); data += sizeof(uint32);
				tels.resize(v_size);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_tel& sub = tels[i];
					sub.parse(data);

				}
			} while (0);

		};
		void reset()
		{
			player_name.clear();
			memset(&player_age, 0, sizeof(player_age)); 
			tels.clear();

		}
		uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += player_name.size();
			data_size += sizeof(player_age);
			do 
			{ 
				uint32 v_size = tels.size();
				for(uint32 i=0; i<v_size; i++)
				{
					struct_tel& sub = tels[i];
					data_size += sub.get_data_size();

				}
			} while (0);

			return data_size;
		}
	public:
		string	player_name;
		uint32	player_age;
		vector<struct_tel>	tels;

};
class struct_country
{
	public:
		void serialize(uint8* & data)
		{
			do 
			{ 
				uint32 s_size = country_num.size(); 
				memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
				memcpy(data, country_num.c_str(), s_size); data += s_size;
			} while (0);
			do 
			{ 
				uint32 v_size = province.size(); 
				memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
				for(uint32 i=0; i<v_size; i++)
				{
					string& sub = province[i];
					do 
					{ 
						uint32 s_size = sub.size(); 
						memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
						memcpy(data, sub.c_str(), s_size); data += s_size;
					} while (0);

				}
			} while (0);

		};
		void parse(uint8* & data)
		{
			do 
			{ 
				country_num.clear();
				uint32 s_size = *((uint32*)data); data += sizeof(uint32);
				country_num.assign((char*)data,s_size);data += s_size;
			} while (0);
			do 
			{ 
				province.clear();
				uint32 v_size = *((uint32*)data); data += sizeof(uint32);
				province.resize(v_size);
				for(uint32 i=0; i<v_size; i++)
				{
					string& sub = province[i];
					do 
					{ 
						sub.clear();
						uint32 s_size = *((uint32*)data); data += sizeof(uint32);
						sub.assign((char*)data,s_size);data += s_size;
					} while (0);

				}
			} while (0);

		};
		void reset()
		{
			country_num.clear();
			province.clear();

		}
		uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += country_num.size();
			do 
			{ 
				uint32 v_size = province.size();
				for(uint32 i=0; i<v_size; i++)
				{
					string& sub = province[i];
					data_size += sizeof(uint32); data_size += sub.size();

				}
			} while (0);

			return data_size;
		}
	public:
		string	country_num;
		vector<string>	province;

};
/***************************end struct define*************************************/
//serialize stream_mode
class msg_TEST_CPP_TO_LUA_STREAM : public msg_base
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
extern msg_base* new_msg_TEST_CPP_TO_LUA_STREAM();
//serialize packet
class msg_TEST_CPP_SERIALIZE_PACKET : public msg_base
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
				uint32 v_size = players.size(); 
				memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					sub.serialize(data);

				}
			} while (0);
			country_info.serialize(data);
		
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
				players.clear();
				uint32 v_size = *((uint32*)data); data += sizeof(uint32);
				players.resize(v_size);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					sub.parse(data);

				}
			} while (0);
			country_info.parse(data);
		
		};
		virtual void reset()
		{
			msg_base::reset();
			from_name.clear();
			memset(&from_id, 0, sizeof(from_id)); 
			players.clear();
			country_info.reset();

		}
		virtual uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += from_name.size();
			data_size += sizeof(from_id);
			do 
			{ 
				uint32 v_size = players.size();
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					data_size += sub.get_data_size();

				}
			} while (0);
			data_size += country_info.get_data_size();

			return data_size;
		}
	public:
		string	from_name;
		uint32	from_id;
		vector<struct_player>	players;
		struct_country	country_info;

};
extern msg_base* new_msg_TEST_CPP_SERIALIZE_PACKET();
//serialize packet
class msg_TEST_CPP_TO_LUA_PACKET : public msg_base
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
				uint32 v_size = players.size(); 
				memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					sub.serialize(data);

				}
			} while (0);
			country_info.serialize(data);
		
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
				players.clear();
				uint32 v_size = *((uint32*)data); data += sizeof(uint32);
				players.resize(v_size);
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					sub.parse(data);

				}
			} while (0);
			country_info.parse(data);
		
		};
		virtual void reset()
		{
			msg_base::reset();
			from_name.clear();
			memset(&from_id, 0, sizeof(from_id)); 
			players.clear();
			country_info.reset();

		}
		virtual uint32 get_data_size()
		{
			uint32 data_size = 0;
			data_size += sizeof(uint32); data_size += from_name.size();
			data_size += sizeof(from_id);
			do 
			{ 
				uint32 v_size = players.size();
				for(uint32 i=0; i<v_size; i++)
				{
					struct_player& sub = players[i];
					data_size += sub.get_data_size();

				}
			} while (0);
			data_size += country_info.get_data_size();

			return data_size;
		}
	public:
		string	from_name;
		uint32	from_id;
		vector<struct_player>	players;
		struct_country	country_info;

};
extern msg_base* new_msg_TEST_CPP_TO_LUA_PACKET();
//serialize stream_mode
class msg_TEST_CPP_SERIALIZE_STREAM : public msg_base
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
extern msg_base* new_msg_TEST_CPP_SERIALIZE_STREAM();
