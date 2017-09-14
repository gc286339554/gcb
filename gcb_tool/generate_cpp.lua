local suo_jin_count = 1 --缩进
local function replace_suo_jin(str)
	local cur_suo_jin_str = ""
	for i=0,suo_jin_count do
		cur_suo_jin_str = cur_suo_jin_str .. "\t"
	end
	return string.gsub(str, "TTAABB ",cur_suo_jin_str)
end

local str_def__enum_pre = 
[[
#pragma once
//S  game server
//G  gate server
//C	 game client
enum op_enum {
]]

local function generate_enum_file()
	local op_enum_file = io.open("generate_cpp\\op_enum.h", "w");
	assert(op_enum_file);
	op_enum_file:write(str_def__enum_pre);
	local g_op_enum_size = 0
	for k,v in pairs(g_op_enum) do
		op_enum_file:write("\t");
		op_enum_file:write(v.op_name);
		op_enum_file:write("\t\t\t\t= ");
		op_enum_file:write(v.op_value);
		op_enum_file:write(",");
		if v.describe ~= nil then
			op_enum_file:write("//");
			op_enum_file:write(v.describe);
		end
		op_enum_file:write("\n");
		g_op_enum_size = g_op_enum_size + 1
	end
	op_enum_file:write("\tOP_COUNT\t\t\t\t= ");
	op_enum_file:write(g_op_enum_size);
	op_enum_file:write("\n");
	op_enum_file:write("};\n");
	op_enum_file:close();
end

local str_def__serialize_string = 
[[TTAABB do 
TTAABB { 
TTAABB 	uint32 s_size = %s.size(); 
TTAABB 	memcpy(data, &s_size, sizeof(uint32)); data += sizeof(uint32);
TTAABB 	memcpy(data, %s.c_str(), s_size); data += s_size;
TTAABB } while (0);
]]
local str_def__serialize_vector = 
[[TTAABB do 
TTAABB { 
TTAABB 	uint32 v_size = %s.size(); 
TTAABB 	memcpy(data, &v_size, sizeof(uint32)); data += sizeof(uint32);
TTAABB 	for(uint32 i=0; i<v_size; i++)
TTAABB 	{
TTAABB 		%s& sub = %s[i];
%s
TTAABB 	}
TTAABB } while (0);
]]

local function get_param_serialize_str(param_name,param_type,param_content, suo_jin_index)
	suo_jin_index = suo_jin_index or 1
	suo_jin_count = suo_jin_count + suo_jin_index
	local res_str = ""
	if param_type == "string" then 
		res_str = string.format(str_def__serialize_string ,param_name,param_name)
	elseif 	param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
			param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
			param_type == "f32" or param_type == "f64" or param_type == "bool" then
		res_str = string.format("TTAABB memcpy(data, &%s, sizeof(%s)); data += sizeof(%s);\n",param_name,param_name,param_name)
	elseif param_type == "vector" then
		local sub_str = get_param_serialize_str("sub",param_content,param_content,2)
		res_str = string.format(str_def__serialize_vector,param_name,param_content,param_name,sub_str)
	else
		res_str = string.format("TTAABB %s.serialize(data);\n",param_name)
	end
	res_str = replace_suo_jin(res_str)
	suo_jin_count = suo_jin_count - suo_jin_index
	return res_str
end

local str_def__parse_string =
[[TTAABB do 
TTAABB { 
TTAABB 	%s.clear();
TTAABB 	uint32 s_size = *((uint32*)data); data += sizeof(uint32);
TTAABB 	%s.assign((char*)data,s_size);data += s_size;
TTAABB } while (0);
]]
local str_def__parse_vector =
[[TTAABB do 
TTAABB { 
TTAABB 	%s.clear();
TTAABB 	uint32 v_size = *((uint32*)data); data += sizeof(uint32);
TTAABB 	%s.resize(v_size);
TTAABB 	for(uint32 i=0; i<v_size; i++)
TTAABB 	{
TTAABB 		%s& sub = %s[i];
%s
TTAABB 	}
TTAABB } while (0);
]]
local function get_param_parse_str(param_name,param_type,param_content,suo_jin_index)
	suo_jin_index = suo_jin_index or 1
	suo_jin_count = suo_jin_count + suo_jin_index
	local res_str = ""
	if param_type == "string" then 
		res_str = string.format(str_def__parse_string, param_name, param_name)
	elseif 	param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
			param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
			param_type == "f32" or param_type == "f64" or param_type == "bool" then
		res_str = string.format("TTAABB memcpy(&%s, data, sizeof(%s)); data += sizeof(%s);\n",param_name,param_name,param_name)
	elseif param_type == "vector" then
		local sub_str = get_param_parse_str("sub",param_content,param_content,2)
		res_str = string.format(str_def__parse_vector, param_name,param_name,param_content,param_name,sub_str)
	else
		res_str = string.format("TTAABB %s.parse(data);\n",param_name)
	end
	res_str = replace_suo_jin(res_str)
	suo_jin_count = suo_jin_count - suo_jin_index
	return res_str
end

local function get_param_reset_str(param_name,param_type,param_content)
	if param_type == "string" or param_type == "vector" then 
		return string.format("\t\t\t%s.clear();\n",param_name)
	elseif 	param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
			param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
			param_type == "f32" or param_type == "f64" or param_type == "bool" then
		return string.format("\t\t\tmemset(&%s, 0, sizeof(%s)); \n",param_name,param_name)
	else
		return string.format("\t\t\t%s.reset();\n",param_name)
	end
end

local str_def__data_vector =
[[TTAABB do 
TTAABB { 
TTAABB 	uint32 v_size = %s.size();
TTAABB 	for(uint32 i=0; i<v_size; i++)
TTAABB 	{
TTAABB 		%s& sub = %s[i];
%s
TTAABB 	}
TTAABB } while (0);
]]
local function get_param_data_size_str(param_name,param_type,param_content,suo_jin_index)
	suo_jin_index = suo_jin_index or 1
	suo_jin_count = suo_jin_count + suo_jin_index
	local res_str = ""
	if param_type == "string" then 
		res_str = string.format("TTAABB data_size += sizeof(uint32); data_size += %s.size();\n",param_name)
	elseif 	param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
			param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
			param_type == "f32" or param_type == "f64" or param_type == "bool" then
		res_str = string.format("TTAABB data_size += sizeof(%s);\n",param_name)
	elseif param_type == "vector" then
		local sub_str = get_param_data_size_str("sub",param_content,param_content,2)
		res_str = string.format(str_def__data_vector, param_name,param_content,param_name,sub_str)
	else
		res_str = string.format("TTAABB data_size += %s.get_data_size();\n",param_name)
	end
	res_str = replace_suo_jin(res_str)
	suo_jin_count = suo_jin_count - suo_jin_index
	return res_str
end

local str_def__generate_struct_class = 
[[
class %s
{
	public:
		void serialize(uint8* & data)
		{
%s
		};
		void parse(uint8* & data)
		{
%s
		};
		void reset()
		{
%s
		}
		uint32 get_data_size()
		{
			uint32 data_size = 0;
%s
			return data_size;
		}
	public:
%s
};
]]
local function generate_instance_file___generate_struct(file_name,instance_file_h)
	local struct_list = g_struct_list[file_name]
	for _,details in pairs(struct_list) do
		local serialize_str = ""
		local parse_str = ""
		local member_str = ""
		local reset_str = ""
		local data_size_str = ""
		for _,param in ipairs(details.struct_param) do
			serialize_str = serialize_str .. get_param_serialize_str(param.name,param.type,param.content)
			parse_str = parse_str .. get_param_parse_str(param.name,param.type,param.content)
			reset_str = reset_str .. get_param_reset_str(param.name,param.type,param.content)
			data_size_str = data_size_str .. get_param_data_size_str(param.name,param.type,param.content)
			if param.type == "vector" then
				member_str = member_str .. string.format("\t\tvector<%s>\t%s;\n",param.content,param.name)
			else
				member_str = member_str .. string.format("\t\t%s\t%s;\n",param.type,param.name)
			end
		end
		local struct_def_h = string.format(str_def__generate_struct_class, 
			details.struct_name,serialize_str,parse_str,reset_str,data_size_str,member_str);
		instance_file_h:write(struct_def_h)
	end
end

local str_def__generate_instance_class = 
[[
//%s
class msg_%s : public msg_base
{
	public:
		virtual void serialize()
		{
			uint32 s_size = get_data_size();
			if(s_size > 0) re_malloc(s_size);
			*((uint32*)(m_buff + DATA_HEAD_LEN)) = m_op;
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
%s		
			m_data_len = data - m_buff;
			*((uint32*)(m_buff)) = m_data_len;
		};
		virtual void parse()
		{
			assert(*((uint32*)(m_buff + DATA_HEAD_LEN)) == m_op);
			uint8* data = m_buff + DATA_HEAD_LEN + DATA_OP_LEN;
%s		
		};
		virtual void reset()
		{
			msg_base::reset();
%s
		}
		virtual uint32 get_data_size()
		{
			uint32 data_size = 0;
%s
			return data_size;
		}
	public:
%s
};
extern msg_base* new_msg_%s();
]]

local str_def__generate_instance_cpp = 
[[
msg_base* new_msg_%s()
{
	msg_base* p_obj = (msg_base*)(new msg_%s());
	p_obj->set_op(%s);
	p_obj->set_stream_mode(%s);
	return p_obj;
}
]]
local function generate_instance_file()
	for file_name,v in pairs(g_op_list) do
		local instance_file_h = io.open("generate_cpp\\" .. file_name .. ".h", "w");
		instance_file_h:write("#pragma once\n#include \"../msg_base.h\"\n\nusing namespace std;\n");
		instance_file_h:write("/***************************start struct define***********************************/\n");
		generate_instance_file___generate_struct(file_name,instance_file_h)
		instance_file_h:write("/***************************end struct define*************************************/\n");
		
		local instance_file_cpp = io.open("generate_cpp\\" .. file_name .. ".cpp", "w");
		instance_file_cpp:write("#include \"" .. file_name .. ".h\"\n\n");

		for op_name,details in pairs(v) do
			if not details.just_opcode then
				local serialize_str = ""
				local parse_str = ""
				local member_str = ""
				local reset_str = ""
				local data_size_str = ""
				
				if not details.stream_mode then 
					for _,param in ipairs(details.param) do
						serialize_str = serialize_str .. get_param_serialize_str(param.name,param.type,param.content)
						parse_str = parse_str .. get_param_parse_str(param.name,param.type,param.content)
						reset_str = reset_str .. get_param_reset_str(param.name,param.type,param.content)
						data_size_str = data_size_str .. get_param_data_size_str(param.name,param.type,param.content)
						if param.type == "vector" then
							member_str = member_str .. string.format("\t\tvector<%s>\t%s;\n",param.content,param.name)
						else
							member_str = member_str .. string.format("\t\t%s\t%s;\n",param.type,param.name)
						end
					end
				end
				local op_def_h = string.format(str_def__generate_instance_class,
					details.describe or "",op_name,serialize_str,parse_str,reset_str,data_size_str,member_str,op_name);
				
				local stream_mode_str = "false" if details.stream_mode then stream_mode_str = "true" end
				local op_def_cpp = string.format(str_def__generate_instance_cpp, op_name,op_name,op_name,stream_mode_str);
				instance_file_h:write(op_def_h)
				instance_file_cpp:write(op_def_cpp)
			end
		end
		instance_file_h:close()
		instance_file_cpp:close()
	end
end

local function generate_include_file()
	local register_func = ""
	local include_file_list = {}
	table.insert(include_file_list,"op_enum.h")
	
	for k,v in pairs(g_op_list) do
		table.insert(include_file_list,k .. ".h")
		
		for op_name,details in pairs(v) do
			if not details.just_opcode then
				register_func = register_func .. "\tp_g_new_msg_handler[" .. op_name .."] = reinterpret_cast<uint64>(new_msg_" .. op_name .. ");\n"
			end
		end
	end

	local all_include_file = io.open("generate_cpp\\op_include_all.h", "w");
	assert(all_include_file);
	all_include_file:write("#pragma once\n\n");
	for k,v in pairs(include_file_list) do
		all_include_file:write("#include \"" .. v .. "\"\n")
	end
	all_include_file:write("\nextern void register_new_msg_handler(uint64* p_g_new_msg_handler);\n")
	all_include_file:close();

	local all_include_file_cpp = io.open("generate_cpp\\op_include_all.cpp", "w");

	all_include_file_cpp:write("#include \"op_include_all.h\"\n\n")
	all_include_file_cpp:write("\nvoid register_new_msg_handler(uint64* p_g_new_msg_handler)\n")
	all_include_file_cpp:write("{\n")
	all_include_file_cpp:write(register_func)
	all_include_file_cpp:write("}\n")
end


generate_enum_file()
generate_instance_file()
generate_include_file()


