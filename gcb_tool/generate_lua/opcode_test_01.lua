--***************************start struct define***********************************
--***************struct_dog**************--
function parse_lua_struct__struct_dog(cpp_msg)
	local lua_struct = {}
	lua_struct.dog_name = cpp_msg:get_string()
	lua_struct.dog_age = cpp_msg:get_uint32()
	
	return lua_struct
end
function serialize_lua_struct__struct_dog(cpp_msg, lua_struct)
	lua_struct.dog_name = lua_struct.dog_name or 0
	cpp_msg:put_string(lua_struct.dog_name)
	lua_struct.dog_age = lua_struct.dog_age or 0
	cpp_msg:put_uint32(lua_struct.dog_age)
	
end
--***************************end struct define*************************************
--serialize packet
function parse_lua_op_obj__TEST_LUA_TO_CPP_PACKET(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	lua_op_obj.from_name = cpp_msg:get_string()
	lua_op_obj.from_id = cpp_msg:get_uint32()
	repeat
		local v_size = cpp_msg:get_uint32()
		lua_op_obj.dogs = {}
		for i=1,v_size do
			lua_op_obj.dogs[i] = {}
			lua_op_obj.dogs[i] = parse_lua_struct__struct_dog(cpp_msg)

		end
	until (true)
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_LUA_TO_CPP_PACKET(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_LUA_TO_CPP_PACKET)
	cpp_msg:start_write()
	lua_op_obj.from_name = lua_op_obj.from_name or 0
	cpp_msg:put_string(lua_op_obj.from_name)
	lua_op_obj.from_id = lua_op_obj.from_id or 0
	cpp_msg:put_uint32(lua_op_obj.from_id)
	repeat
		lua_op_obj.dogs = lua_op_obj.dogs or {}
		local v_size = #lua_op_obj.dogs
		cpp_msg:put_uint32(v_size)
		for i=1,v_size do
			lua_op_obj.dogs[i] = lua_op_obj.dogs[i] or {}
			serialize_lua_struct__struct_dog(cpp_msg, lua_op_obj.dogs[i])

		end
	until (true)
	
	cpp_msg:end_write()
end
--serialize stream_mode
function parse_lua_op_obj__TEST_LUA_TO_CPP_STREAM(cpp_msg)
	local lua_op_obj = {}
	lua_op_obj.opcode = cpp_msg:get_op()
	
	return lua_op_obj
end
function serialize_lua_op_obj__TEST_LUA_TO_CPP_STREAM(cpp_msg, lua_op_obj)
	cpp_msg:set_op(TEST_LUA_TO_CPP_STREAM)
	cpp_msg:start_write()
	
	cpp_msg:end_write()
end
