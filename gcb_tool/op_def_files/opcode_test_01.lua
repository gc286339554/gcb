print "load opcode_test_01 ..."
local declare_op,declare_struct = register_op_file("opcode_test_01")

----------------------------------------------------------------------
declare_struct( 
{
	struct_name = "struct_dog",
	struct_param = {
		{type = "string", name = "dog_name"},
		{type = "uint32", name = "dog_age"},		
	}
})

----------------------------------------------------------------------

declare_op("TEST_01_BEGIN",{
	describe = "begin",
	just_opcode = true,
});

declare_op("TEST_LUA_TO_CPP_PACKET",{
	describe = "serialize packet",
	param = {
		{type = "string", name = "from_name"},
		{type = "uint32", name = "from_id"},	
		{type = "vector", name = "dogs", content = "struct_dog"},
	}
});

declare_op("TEST_LUA_TO_CPP_STREAM",{
	describe = "serialize stream_mode",
	stream_mode = true,
});


declare_op("TEST_01_END",{
	describe = "end",
	just_opcode = true,
});



	


