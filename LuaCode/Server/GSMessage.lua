---
--- Created by lsjlkz.
--- DateTime: 2022/7/19 17:00
--- Desc:
---消息模块，用于给其他进程或客户端收发消息的

__G__GSMessageTable = __G__GSMessageTable or {}

__G__GSMessageTable.RegServerDistributeTable = {}

function __G__GSMessageTable.RegServerDistribute(msg_type, func)
    --注册消息
    __G__GSMessageTable.RegServerDistributeTable[msg_type] = func
end

function __G__GSMessageTable.TriggerServerDistribute(msg_type, param)
    --收到了消息，触发对应的函数
    if __G__GSMessageTable.RegServerDistributeTable[msg_type] == nil then
        return
    end
    __G__GSMessageTable.RegServerDistributeTable[msg_type](param)
end

function __G__GSMessageTable.MessageDebug(param)
    for i, v in pairs(param) do
        print(v)
    end
end

--消息注册和收发debug
function __G__GSMessageTable.Debug()
    __G__GSMessageTable.RegServerDistribute(3, __G__GSMessageTable.MessageDebug)
    cGameServer.DebugReceiveMsg(3, {3,4,5, 6, 7, false})
end

return __G__GSMessageTable