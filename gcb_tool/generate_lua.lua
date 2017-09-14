local suo_jin_count = 0 --缩进
local function replace_suo_jin(str)
	local cur_suo_jin_str = ""
	for i=1,suo_jin_count do
		cur_suo_jin_str = cur_suo_jin_str .. "\t"
	end
	return string.gsub(str, "TTAABB ",cur_suo_jin_str)
end

local str_def__enum_pre = 
[[
--S  game server
--G  gate server
--C	 game client
OP_NAME = {}
]]

local function generate_enum_file()
	local op_enum_file = io.open("generate_lua\\op_enum.lua", "w");
	assert(op_enum_file);
	op_enum_file:write(str_def__enum_pre);
	local g_op_enum_size = 0
	for k,v in pairs(g_op_enum) do
		op_enum_file:write(v.op_name);
		op_enum_file:write("\t\t\t\t= ");
		op_enum_file:write(v.op_value);
		op_enum_file:write("  ");
		if v.describe ~= nil then
			op_enum_file:write("--");
			op_enum_file:write(v.describe);
		end
		op_enum_file:write("\n");
        op_enum_file:write(string.format("OP_NAME[%s] = \"%s\"\n",v.op_name,v.op_name));
		g_op_enum_size = g_op_enum_size + 1
	end
	op_enum_file:write("OP_COUNT\t\t\t\t= ");
	op_enum_file:write(g_op_enum_size);
	op_enum_file:write("\n");
	op_enum_file:close();
end

----------------------------------------------------------------------------
local str_def__parse_vector =
[[TTAABB repeat
TTAABB 	local v_size = cpp_msg:get_uint32()
TTAABB 	%s = {}
TTAABB 	for i=1,v_size do
%s
TTAABB 	end
TTAABB until (true)
]]
local function get_param_parse_str(param_name,param_type,param_content,suo_jin_index)
	suo_jin_index = suo_jin_index or 1
	suo_jin_count = suo_jin_count + suo_jin_index
	local res_str = ""

	if  param_type == "string" or param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
	param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
	param_type == "f32" or param_type == "f64" or param_type == "bool" then
		res_str = string.format("TTAABB %s = cpp_msg:get_%s()\n",param_name,param_type)
	elseif param_type == "vector" then
		local sub_str = get_param_parse_str(string.format("%s[i]",param_name),param_content,nil,2)
		res_str = string.format(str_def__parse_vector, param_name,sub_str)
	else
		res_str = string.format("TTAABB %s = {}\nTTAABB %s = parse_lua_struct__%s(cpp_msg)\n",param_name,param_name,param_type)
	end

	res_str = replace_suo_jin(res_str)
	suo_jin_count = suo_jin_count - suo_jin_index
	return res_str
end

local str_def__serialize_vector =
[[TTAABB repeat
TTAABB 	%s = %s or {}
TTAABB 	local v_size = #%s
TTAABB 	cpp_msg:put_uint32(v_size)
TTAABB 	for i=1,v_size do
%s
TTAABB 	end
TTAABB until (true)
]]
local function get_param_serialize_str(param_name,param_type,param_content,suo_jin_index)
	suo_jin_index = suo_jin_index or 1
	suo_jin_count = suo_jin_count + suo_jin_index
	local res_str = ""

	if  param_type == "string" or param_type == "uint8" or param_type == "int8" or param_type == "uint16" or param_type == "int16" or 
	param_type == "uint32" or param_type == "int32" or param_type == "uint64" or param_type == "int64" or 
	param_type == "f32" or param_type == "f64" or param_type == "bool" then
		res_str = string.format("TTAABB %s = %s or 0\nTTAABB cpp_msg:put_%s(%s)\n",param_name,param_name,param_type,param_name)
	elseif param_type == "vector" then
		local sub_str = get_param_serialize_str(string.format("%s[i]",param_name),param_content,nil,2)
		res_str = string.format(str_def__serialize_vector, param_name, param_name, param_name, sub_str)
	else
		res_str = string.format("TTAABB %s = %s or {}\nTTAABB serialize_lua_struct__%s(cpp_msg, %s)\n", param_name, param_name, param_type, param_name)
	end

	res_str = replace_suo_jin(res_str)
	suo_jin_count = suo_jin_count - suo_jin_index
	return res_str
end

local str_def__generate_struct =
[[
function parse_lua_struct__%s(cpp_msg)
	local lua_struct = {}
%s	
	return lua_struct
end
function serialize_lua_struct__%s(cpp_msg, lua_struct)
%s	
end
]]
local function generate_instance_file___generate_struct(file_name,instance_file)
	local struct_list = g_struct_list[file_name]
	for _,details in pairs(struct_list) do
		local parse_str = ""
		local serialize_str = ""

		for _,param in ipairs(details.struct_param) do
			parse_str = parse_str .. get_param_parse_str(string.format("lua_struct.%s",param.name),param.type,param.content,1)
			serialize_str = serialize_str .. get_param_serialize_str(string.format("lua_struct.%s",param.name),param.type,param.content,1)
		end
		local struct_def_lua = string.format(str_def__generate_struct,details.struct_name,parse_str,details.struct_name,serialize_str);
		instance_file:write(string.format("--***************%s**************--\n",details.struct_name));
		instance_file:write(struct_def_lua)
	end
end

local str_def__generate_op_obj =
[[--%s
function parse_lua_op_obj__%s(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
%s	
	return lua_op_obj
end
function serialize_lua_op_obj__%s(cpp_msg, lua_op_obj)
	cpp_msg:set_op(%s)
	cpp_msg:start_write()
%s	
	cpp_msg:end_write()
end
]]
local function generate_instance_file()
	for file_name,v in pairs(g_op_list) do
		local instance_file = io.open("generate_lua\\" .. file_name .. ".lua", "w");
		instance_file:write("--***************************start struct define***********************************\n");
		generate_instance_file___generate_struct(file_name,instance_file)
		instance_file:write("--***************************end struct define*************************************\n");

		for op_name,details in pairs(v) do
			if not details.just_opcode then
				local serialize_str = ""
				local parse_str = ""
				
				if not details.stream_mode then 
					for _,param in ipairs(details.param) do
						parse_str = parse_str .. get_param_parse_str(string.format("lua_op_obj.%s",param.name),param.type,param.content,1)
						serialize_str = serialize_str .. get_param_serialize_str(string.format("lua_op_obj.%s",param.name),param.type,param.content,1)
					end
				end
				local op_def = string.format(str_def__generate_op_obj,
					details.describe or "",op_name,parse_str,op_name,op_name,serialize_str);
				instance_file:write(op_def)
			end
		end

		instance_file:close()
	end
end

local function generate_include_file()
	local include_file_list = {}
	table.insert(include_file_list,"op_enum")
	
	for k,v in pairs(g_op_list) do
		table.insert(include_file_list,k )
	end

	local all_include_file = io.open("generate_lua\\op_def.lua", "w");
	assert(all_include_file);

	for k,v in pairs(include_file_list) do
		all_include_file:write("require \"" .. v .. "\"\n")
	end
	all_include_file:close()
end


generate_enum_file()
generate_instance_file()
generate_include_file()