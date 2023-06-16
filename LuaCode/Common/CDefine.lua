---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:37
--- Desc:
---

__G__CDefineTable = __G__CDefineTable or {}


--重定向
__G__CDefineTable.RedirectToLogic = 1

--连接类型
__G__CDefineTable.EndPoint_ClientGateway = 5
__G__CDefineTable.EndPoint_GatewayClient = 6
__G__CDefineTable.EndPoint_LogicGateway = 7
__G__CDefineTable.EndPoint_GatewayLogic = 8


--监听端口
__G__CDefineTable.Port_Logic = 10010
__G__CDefineTable.Port_Gateway = 10011
__G__CDefineTable.Port_Server = 10012
__G__CDefineTable.Port_World = 10013


__G__CDefineTable.WorldID = 1
__G__CDefineTable.GateWayID = 2
__G__CDefineTable.LogicID = 3


--数值范围
__G__CDefineTable.MIN_INT8 = -128
__G__CDefineTable.MAX_INT8 = 127

__G__CDefineTable.MIN_INT16 = -32768
__G__CDefineTable.MAX_INT16 = 32767

__G__CDefineTable.MIN_INT32 = -2147483648
__G__CDefineTable.MAX_INT32 = 2147483647

__G__CDefineTable.MIN_INT64 = -9223372036854775808
__G__CDefineTable.MAX_INT64 = 9223372036854775807

__G__CDefineTable.MIN_UINT64 = 0
__G__CDefineTable.MAX_UINT64 = 18446744073709551615

__G__CDefineTable.MIN_UINT8 = 0
__G__CDefineTable.MAX_UINT8 = 255

__G__CDefineTable.MIN_UINT16 = 0
__G__CDefineTable.MAX_UINT16 = 65535

__G__CDefineTable.MIN_UINT32 = 0
__G__CDefineTable.MAX_UINT32 = 4294967295


return __G__CDefineTable