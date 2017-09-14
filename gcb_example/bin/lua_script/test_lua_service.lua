package.path = package.path ..';..\\src\\generate\\?.lua;..\\src\\?.lua';
require "op_def"
local data_packet = require "data_packet"


function update(ms_delay)	
end

function binary_service_msg_handler(p_msg)
	local msg = data_packet.new_from_handler(p_msg)
	msg:start_read()
	if TEST_CPP_TO_LUA_STREAM == msg:get_op() then
		print("---------------TEST_CPP_TO_LUA_STREAM------------------------")
		print(OP_NAME[msg:get_op()])
		print(msg:get_num())
		print(msg:get_string())
		print(msg:get_bool())
		print(msg:get_string())
	end
end

function lua_service_msg_handler(p_msg)
	local msg = data_packet.new_from_handler(p_msg)
	msg:start_read()
	if TEST_CPP_TO_LUA_PACKET == msg:get_op() then
		print("---------------TEST_CPP_TO_LUA_PACKET------------------------")
		local lua_msg = parse_lua_op_obj__TEST_CPP_TO_LUA_PACKET(msg)
		print(lua_msg.from_name)
		print(lua_msg.from_id)
		print("player size = " .. #lua_msg.players)
		print(lua_msg.players[1].player_name)
		print(lua_msg.players[1].player_age)
		print(lua_msg.players[1].tels[1].tel_num)
		print(lua_msg.players[1].tels[1].price)
		print(lua_msg.players[1].tels[2].tel_num)
		print(lua_msg.players[1].tels[2].price)
		
		print(lua_msg.players[2].player_name)
		print(lua_msg.players[2].player_age)
		print(lua_msg.players[2].tels[1].tel_num)
		print(lua_msg.players[2].tels[1].price)
		print(lua_msg.players[2].tels[2].tel_num)
		print(lua_msg.players[2].tels[2].price)
		
		print(lua_msg.country_info.country_num)
		print(lua_msg.country_info.province[1])
		print(lua_msg.country_info.province[2])
	end
end




