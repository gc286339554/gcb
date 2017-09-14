#pragma once
#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>

#ifdef __GNUC__
typedef long long			int64;
typedef int					int32;
typedef short				int16;
typedef signed char			int8;
typedef unsigned long long	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
#else
typedef signed __int64		int64;
typedef signed __int32		int32;
typedef signed __int16		int16;
typedef signed __int8		int8;
typedef unsigned __int64	uint64;
typedef unsigned __int32	uint32;
typedef unsigned __int16	uint16;
typedef unsigned __int8		uint8;
#endif

typedef float					f32;
typedef double					f64;

#define SAFE_DEL(x) do { delete x; x = nullptr;} while (0);
#define DATA_HEAD_LEN (sizeof(uint32))
#define DATA_OP_LEN (sizeof(uint32))	
#define DATA_MAX_LEN (5*1024*1024)

class data_packet
{
	protected:
		uint8* m_buff;
		uint32 m_pos = 0;
		uint32 m_buff_len;
		uint32 m_data_len;
		uint32 m_op = 0;
	public:
		data_packet(uint32 size = 1024)
		{
			m_buff_len = size;
			m_buff = new uint8[size];
			memset(m_buff, 0, size);
		}
		virtual ~data_packet(){	SAFE_DEL(m_buff);}
		virtual void reset()
		{
			m_pos = 0;
		}
	public:
		const uint8* get_buff() { return m_buff; }
		uint32 get_buff_len() { return m_buff_len; }
		uint32 get_data_len() { return m_data_len; }
		uint32 get_data_pos() {	return m_pos; }
		uint32 get_op() { return m_op; }
		data_packet& set_op(uint32 op) { m_op = op; return *this;}
		
		data_packet& calculate_data_len_when_read()
		{
			m_data_len = *((uint32*)m_buff);
			m_op = *((uint32*)(m_buff + DATA_HEAD_LEN));
			re_malloc(m_data_len);
			return *this;
		}
		data_packet& start_write()
		{
			assert(m_pos == 0);
			put_uint32(m_data_len);
			put_uint32(m_op);
			return *this;
		}
		data_packet& end_write()
		{
			assert(m_pos != 0);
			m_data_len = m_pos;
			m_pos = 0;
			memcpy(m_buff, &m_data_len, DATA_HEAD_LEN);
			memcpy(m_buff + DATA_HEAD_LEN, &m_op, DATA_OP_LEN);
			return *this;
		}
		data_packet& start_read()
		{
			m_pos = 0;
			m_data_len = get_uint32();
			m_op = get_uint32();
			return *this;
		}
		data_packet& put_int_reserved(int val, int pos_reserved)
		{
			memcpy(m_buff + pos_reserved, &val, sizeof(int));
			return *this;
		}
		void copy_from(data_packet* t_packet)
		{
			re_malloc(t_packet->get_data_len());
			memcpy(m_buff, t_packet->get_buff(), t_packet->get_data_len());

			m_pos = t_packet->m_pos;
			m_data_len = t_packet->m_data_len;
			m_op = t_packet->m_op;
		}
		data_packet& append(const uint8* data, uint32 count)
		{
			while ((m_buff_len - m_pos) < count)
			{
				re_malloc();
			}
			memcpy(m_buff + m_pos, data, count);
			m_pos += count;
			return *this;
		}
	protected:
		void re_malloc(uint32 new_size = 0)
		{
			new_size = new_size == 0 ? m_buff_len * 2 : new_size;
			if (new_size <= m_buff_len){ return;}
			uint8* temp = m_buff;
			m_buff = new uint8[new_size];
			if (m_buff == NULL){ throw("memory Less Exception");}
			m_buff_len = new_size;
			memset(m_buff, 0, m_buff_len);
			memcpy(m_buff, temp, m_pos);
			delete[] temp;
		}
	public:
		template <typename T> data_packet& put(T value)
		{
			while ((m_buff_len - m_pos) < sizeof(T))
			{
				re_malloc();
			}
			memcpy(m_buff + m_pos, &value, sizeof(T));
			m_pos += sizeof(T);
			return *this;
		}

		template <> data_packet& put(std::string& value)
		{
			uint32 str_len = value.size();
			while ((m_buff_len - m_pos) < (str_len + sizeof(uint32)))
			{
				re_malloc();
			}
			memcpy(m_buff + m_pos, &str_len, sizeof(uint32));
			m_pos += sizeof(uint32);

			memcpy(m_buff + m_pos, value.c_str(), str_len);
			m_pos += str_len;
			return *this;
		}
		template <> data_packet& put(const char* value)
		{
			uint32 str_len = strlen(value);
			while ((m_buff_len - m_pos) < (str_len + sizeof(uint32)))
			{
				re_malloc();
			}
			memcpy(m_buff + m_pos, &str_len, sizeof(uint32));
			m_pos += sizeof(uint32);

			memcpy(m_buff + m_pos, value, str_len);
			m_pos += str_len;
			return *this;
		}
		/*-------------------------------------------------------------------*/
		template <typename T> T get()
		{
			if ((m_buff_len - m_pos) < sizeof(T)) throw("buf Less Exception");
			T ret;
			memcpy(&ret, m_buff + m_pos, sizeof(T));
			m_pos += sizeof(T);
			return ret;
		}
		template <> std::string get()
		{
			if ((m_buff_len - m_pos) < sizeof(uint32)) throw("buf Less Exception");
			uint32 str_len;
			std::string str;
			memcpy(&str_len, m_buff + m_pos, sizeof(uint32));
			m_pos += sizeof(int);

			str.resize(str_len, 0);
			memcpy((void*)str.c_str(), m_buff + m_pos, str_len);
			m_pos += str_len;

			return str;
		}
		/*-------------------------------------------------------------------*/
		data_packet& put_uint8(uint8 val) { return put(val); }
		data_packet& put_int8(int8 val) { return put(val); }
		data_packet& put_uint16(uint16 val) { return put(val); }
		data_packet& put_int16(uint16 val) { return put(val); }
		data_packet& put_uint32(uint32 val){ return put(val); }
		data_packet& put_int32(int32 val) { return put(val); }
		data_packet& put_uint64(uint64 val) { return put(val); }
		data_packet& put_int64(int64 val) { return put(val); }
		data_packet& put_f32(f32 val) { return put(val); }
		data_packet& put_f64(f64 val) { return put(val); }
		data_packet& put_bool(bool val) { return put(val); }
		data_packet& put_string(std::string& val) { return put(val); }
		data_packet& put_string(const char* val) { return put(val); }
			
		uint8 get_uint8() { return get<uint8>(); }
		int8 get_int8() { return get<int8>(); }
		uint16 get_uint16() { return get<uint16>(); }
		int16 get_int16() { return get<int16>(); }
		uint32 get_uint32(){ return get<uint32>(); }
		int32 get_int32() { return get<int32>(); }
		int64 get_uint64() { return get<uint64>(); }
		int64 get_int64() { return get<int64>(); }
		f32 get_f32() { return get<f32>(); }
		f64 get_f64() { return get<f64>(); }
		bool get_bool() { return get<bool>(); }
		std::string get_string() { return get<std::string>(); }
};


