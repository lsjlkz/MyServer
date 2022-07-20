---
--- Created by lsjlkz.
--- DateTime: 2022/6/13 9:56
--- Desc:
---

__G__FunctionTable = __G__FunctionTable or {}

--函数id
__G__FunctionTable.FuncID = 0

--函数分发保存的表
__G__FunctionTable.FuncTable = {}

local function GetFuncID()
    --获得自增ID
    __G__FunctionTable.FuncID = __G__FunctionTable.FuncID + 1
    return __G__FunctionTable.FuncID
end

function __G__FunctionTable.RegMesFunction(func)
    --注册消息函数
    func_id = GetFuncID()
    __G__FunctionTable.FuncTable[func_id] = func
    return func_id
end

function __G__FunctionTable.TriMesFunction(func_id, mes_param)
    if __G__FunctionTable.FuncTable[func_id] == nil then
        return
    end
    __G__FunctionTable.FuncTable[func_id](mes_param)
end

return __G__FunctionTable