print "load opcode_test ..."
local declare_op,declare_struct = register_op_file("opcode_test")

----------------------------------------------------------------------
declare_struct( 
{
	struct_name = "struct_tel",
	struct_param = {
		{type = "string", name = "tel_num"},
		{type = "uint32", name = "price"},		
	}
})

declare_struct(
{
	struct_name = "struct_player",
	struct_param = {
		{type = "string", name = "player_name"},
		{type = "uint32", name = "player_age"},	
		{type = "vector", name = "tels", content = "struct_tel"},		
	}	
})

declare_struct(
{
	struct_name = "struct_country",
	struct_param = {
		{type = "string", name = "country_num"},
		{type = "vector", name = "province", content = "string"},			
	}		
})
----------------------------------------------------------------------

declare_op("TEST_BEGIN",{
	describe = "begin",
	just_opcode = true,
});

declare_op("TEST_CPP_SERIALIZE_PACKET",{
	describe = "serialize packet",
	param = {
		{type = "string", name = "from_name"},
		{type = "uint32", name = "from_id"},	
		{type = "vector", name = "players", content = "struct_player"},
		{type = "struct_country", name = "country_info"},
	}
});


declare_op("TEST_CPP_SERIALIZE_STREAM",{
	describe = "serialize stream_mode",
	stream_mode = true,
});


declare_op("TEST_CPP_TO_LUA_PACKET",{
	describe = "serialize packet",
	param = {
		{type = "string", name = "from_name"},
		{type = "uint32", name = "from_id"},	
		{type = "vector", name = "players", content = "struct_player"},
		{type = "struct_country", name = "country_info"},
	}
});

declare_op("TEST_CPP_TO_LUA_STREAM",{
	describe = "serialize stream_mode",
	stream_mode = true,
});



	
declare_op("TEST_END",{
	describe = "end",
	just_opcode = true,
});



	


