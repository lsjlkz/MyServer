---
--- Created by lsjlkz.
--- DateTime: 2022/7/19 17:00
--- Desc:
---

__G__MessageTable = __G__MessageTable or {}

__G__MessageTable.RegServerDistributeTable = {}

function __G__MessageTable.RegServerDistribute(msg_type, func)
    __G__MessageTable.RegServerDistributeTable[msg_type] = func
end

function __G__MessageTable.TriggerServerDistribute(msg_type, param)
    if __G__MessageTable.RegServerDistributeTable[msg_type] == nil then
        return
    end
    __G__MessageTable.RegServerDistributeTable[msg_type](param)
end

function __G__MessageTable.MessageDebug(param)
    for i, v in pairs(param) do
        print(v)
    end
end

--消息注册和收发debug
function __G__MessageTable.Debug()
    __G__MessageTable.RegServerDistribute(3, __G__MessageTable.MessageDebug)
    cGameServer.DebugReceiveMsg(3, {3,4,5, 6, 7, false})
end

return __G__MessageTable