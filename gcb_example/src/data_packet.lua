local data_packet = {}
data_packet.__index = data_packet
function data_packet.new(opcode)
	local o = {}  
	o.data = dp_malloc_data_packet(opcode)
    setmetatable(o, data_packet)
    return o 
end
function data_packet.new_from_handler(handler)
	local o = {}  
	o.data = handler
    setmetatable(o, data_packet)
    return o 
end

--free 不能随意调用，只有在lua层分配，不向c++层传递时使用（一般测试接口时会用到）
function data_packet:free()
	dp_free_data_packet(self.data)
end

function data_packet:start_write()
	dp_start_write(self.data)
end
function data_packet:end_write()
	dp_end_write(self.data)
end
function data_packet:start_read()
	dp_start_read(self.data)
end
function data_packet:put_num(val)
	dp_put_num(self.data,val)
end
function data_packet:get_num()
	return dp_get_num(self.data)
end
function data_packet:put_bool(val)
	dp_put_bool(self.data,val)
end
function data_packet:get_bool()
	return dp_get_bool(self.data)
end
-------------------------------------------
function data_packet:put_uint8(val)
	dp_put_uint8(self.data,val)
end
function data_packet:get_uint8()
	return dp_get_uint8(self.data)
end
function data_packet:put_int8(val)
	dp_put_int8(self.data,val)
end
function data_packet:get_int8()
	return dp_get_int8(self.data)
end
-------------------------------------------
function data_packet:put_uint16(val)
	dp_put_uint16(self.data,val)
end
function data_packet:get_uint16()
	return dp_get_uint16(self.data)
end
function data_packet:put_int16(val)
	dp_put_int16(self.data,val)
end
function data_packet:get_int16()
	return dp_get_int16(self.data)
end
-------------------------------------------
function data_packet:put_uint32(val)
	dp_put_uint32(self.data,val)
end
function data_packet:get_uint32()
	return dp_get_uint32(self.data)
end
function data_packet:put_int32(val)
	dp_put_int32(self.data,val)
end
function data_packet:get_int32()
	return dp_get_int32(self.data)
end
-------------------------------------------
function data_packet:put_uint64(val)
	dp_put_uint64(self.data,val)
end
function data_packet:get_uint64()
	return dp_get_uint64(self.data)
end
function data_packet:put_int64(val)
	dp_put_int64(self.data,val)
end
function data_packet:get_int64()
	return dp_get_int64(self.data)
end
-------------------------------------------
function data_packet:put_f64(val)
	dp_put_f64(self.data,val)
end
function data_packet:get_f64()
	return dp_get_f64(self.data)
end
function data_packet:put_f32(val)
	dp_put_f32(self.data,val)
end
function data_packet:get_f32()
	return dp_get_f32(self.data)
end


function data_packet:put_string(str)
	dp_put_string(self.data,str)
end
function data_packet:get_string()
	return dp_get_string(self.data,g_service_this)
end
function data_packet:get_op()
	return dp_get_op(self.data)
end
function data_packet:set_op(val)
	dp_set_op(self.data,val)
end


return data_packet