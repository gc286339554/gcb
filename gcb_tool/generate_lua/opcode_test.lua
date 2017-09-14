--***************************start struct define***********************************
--***************struct_tel**************--
function parse_lua_struct__struct_tel(cpp_msg)
	local lua_struct = {}
	lua_struct.tel_num = cpp_msg:get_string()
	lua_struct.price = cpp_msg:get_uint32()
	
	return lua_struct
end
function serialize_lua_struct__struct_tel(cpp_msg, lua_struct)
	lua_struct.tel_num = lua_struct.tel_num or 0
	cpp_msg:put_string(lua_struct.tel_num)
	lua_struct.price = lua_struct.price or 0
	cpp_msg:put_uint32(lua_struct.price)
	
end
--***************struct_player**************--
function parse_lua_struct__struct_player(cpp_msg)
	local lua_struct = {}
	lua_struct.player_name = cpp_msg:get_string()
	lua_struct.player_age = cpp_msg:get_uint32()
	repeat
		local v_size = cpp_msg:get_uint32()
		lua_struct.tels = {}
		for i=1,v_size do
			lua_struct.tels[i] = {}
			lua_struct.tels[i] = parse_lua_struct__struct_tel(cpp_msg)

		end
	until (true)
	
	return lua_struct
end
function serialize_lua_struct__struct_player(cpp_msg, lua_struct)
	lua_struct.player_name = lua_struct.player_name or 0
	cpp_msg:put_string(lua_struct.player_name)
	lua_struct.player_age = lua_struct.player_age or 0
	cpp_msg:put_uint32(lua_struct.player_age)
	repeat
		lua_struct.tels = lua_struct.tels or {}
		local v_size = #lua_struct.tels
		cpp_msg:put_uint32(v_size)
		for i=1,v_size do
			lua_struct.tels[i] = lua_struct.tels[i] or {}
			serialize_lua_struct__struct_tel(cpp_msg, lua_struct.tels[i])

		end
	until (true)
	
end
--***************struct_country**************--
function parse_lua_struct__struct_country(cpp_msg)
	local lua_struct = {}
	lua_struct.country_num = cpp_msg:get_string()
	repeat
		local v_size = cpp_msg:get_uint32()
		lua_struct.province = {}
		for i=1,v_size do
			lua_struct.province[i] = cpp_msg:get_string()

		end
	until (true)
	
	return lua_struct
end
function serialize_lua_struct__struct_country(cpp_msg, lua_struct)
	lua_struct.country_num = lua_struct.country_num or 0
	cpp_msg:put_string(lua_struct.country_num)
	repeat
		lua_struct.province = lua_struct.province or {}
		local v_size = #lua_struct.province
		cpp_msg:put_uint32(v_size)
		for i=1,v_size do
			lua_struct.province[i] = lua_struct.province[i] or 0
			cpp_msg:put_string(lua_struct.province[i])

		end
	until (true)
	
end
--***************************end struct define*************************************
--serialize stream_mode
function parse_lua_op_obj__TEST_CPP_TO_LUA_STREAM(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_CPP_TO_LUA_STREAM(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_CPP_TO_LUA_STREAM)
	cpp_msg:start_write()
	
	cpp_msg:end_write()
end
--serialize packet
function parse_lua_op_obj__TEST_CPP_SERIALIZE_PACKET(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	lua_op_obj.from_name = cpp_msg:get_string()
	lua_op_obj.from_id = cpp_msg:get_uint32()
	repeat
		local v_size = cpp_msg:get_uint32()
		lua_op_obj.players = {}
		for i=1,v_size do
			lua_op_obj.players[i] = {}
			lua_op_obj.players[i] = parse_lua_struct__struct_player(cpp_msg)

		end
	until (true)
	lua_op_obj.country_info = {}
	lua_op_obj.country_info = parse_lua_struct__struct_country(cpp_msg)
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_CPP_SERIALIZE_PACKET(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_CPP_SERIALIZE_PACKET)
	cpp_msg:start_write()
	lua_op_obj.from_name = lua_op_obj.from_name or 0
	cpp_msg:put_string(lua_op_obj.from_name)
	lua_op_obj.from_id = lua_op_obj.from_id or 0
	cpp_msg:put_uint32(lua_op_obj.from_id)
	repeat
		lua_op_obj.players = lua_op_obj.players or {}
		local v_size = #lua_op_obj.players
		cpp_msg:put_uint32(v_size)
		for i=1,v_size do
			lua_op_obj.players[i] = lua_op_obj.players[i] or {}
			serialize_lua_struct__struct_player(cpp_msg, lua_op_obj.players[i])

		end
	until (true)
	lua_op_obj.country_info = lua_op_obj.country_info or {}
	serialize_lua_struct__struct_country(cpp_msg, lua_op_obj.country_info)
	
	cpp_msg:end_write()
end
--serialize packet
function parse_lua_op_obj__TEST_CPP_TO_LUA_PACKET(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	lua_op_obj.from_name = cpp_msg:get_string()
	lua_op_obj.from_id = cpp_msg:get_uint32()
	repeat
		local v_size = cpp_msg:get_uint32()
		lua_op_obj.players = {}
		for i=1,v_size do
			lua_op_obj.players[i] = {}
			lua_op_obj.players[i] = parse_lua_struct__struct_player(cpp_msg)

		end
	until (true)
	lua_op_obj.country_info = {}
	lua_op_obj.country_info = parse_lua_struct__struct_country(cpp_msg)
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_CPP_TO_LUA_PACKET(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_CPP_TO_LUA_PACKET)
	cpp_msg:start_write()
	lua_op_obj.from_name = lua_op_obj.from_name or 0
	cpp_msg:put_string(lua_op_obj.from_name)
	lua_op_obj.from_id = lua_op_obj.from_id or 0
	cpp_msg:put_uint32(lua_op_obj.from_id)
	repeat
		lua_op_obj.players = lua_op_obj.players or {}
		local v_size = #lua_op_obj.players
		cpp_msg:put_uint32(v_size)
		for i=1,v_size do
			lua_op_obj.players[i] = lua_op_obj.players[i] or {}
			serialize_lua_struct__struct_player(cpp_msg, lua_op_obj.players[i])

		end
	until (true)
	lua_op_obj.country_info = lua_op_obj.country_info or {}
	serialize_lua_struct__struct_country(cpp_msg, lua_op_obj.country_info)
	
	cpp_msg:end_write()
end
--serialize stream_mode
function parse_lua_op_obj__TEST_CPP_SERIALIZE_STREAM(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_CPP_SERIALIZE_STREAM(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_CPP_SERIALIZE_STREAM)
	cpp_msg:start_write()
	
	cpp_msg:end_write()
end
