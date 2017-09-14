package.path = package.path ..';op_def_files\\?.lua';
g_op_list = {}
g_struct_list = {}
g_op_enum = {}
g_op_count = 0;

function register_op_file(file_name)
	if g_op_list[file_name] == nil then
		g_op_list[file_name] = {}
	end
	if g_struct_list[file_name] == nil then
		g_struct_list[file_name] = {}
	end

	local declare_op_func = 
	function (op_name,details)
		g_op_enum[g_op_count] = {op_name = op_name,op_value = g_op_count,describe = details.describe};
		g_op_list[file_name][op_name] = details
		g_op_count = g_op_count + 1;
	end

	local declare_struct_func = 
	function (details)
		table.insert(g_struct_list[file_name], details) 
	end

	return declare_op_func,declare_struct_func
end

require "op_loader"

require "generate_cpp"
require "generate_lua"
