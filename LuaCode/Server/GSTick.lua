---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 16:22
--- Desc:
---延迟n秒触发的方法

local gs_event = require("Server/GSEvent")
local allot = require("Server/Allot")
local p = require("Common/Print")

__G__GSTickTable = __G__GSTickTable or {}
__G__GSTickTable._TickIDTable = __G__GSTickTable._TickIDTable or {}
__G__GSTickTable._TickSecTable = __G__GSTickTable._TickSecTable or {}

function __G__GSTickTable.test_reg_tick()
    print('test_reg_tick')
    tick_id = __G__GSTickTable.reg_tick(nil, 5, __G__GSTickTable.print_tick, {1,3,5})
    _t = __G__GSTickTable.reg_tick(nil, 3, __G__GSTickTable.test_unreg_tick, tick_id)
end

function __G__GSTickTable.test_unreg_tick(owner, reg_param)
    print('test_unreg_tick')
    tick_id = reg_param
    __G__GSTickTable.unreg_tick(nil, tick_id)

end

function __G__GSTickTable.print_tick(owner, reg_param)
    print('print_tick')
    print(owner, reg_param)
end

function __G__GSTickTable.init()
    gs_event.reg_event(gs_event.AfterCallPerSecond, __G__GSTickTable.call_per_second)
end


function __G__GSTickTable.reg_tick(owner, tick_sec, func, reg_param)
    --注册一个Tick，tick_sec秒后触发对应的事件，带参数
    if tick_sec <= 0 then
        func(owner, reg_param)
        return 0
    end
    local cur_sec = cGameServer.Seconds()
    local target_sec = cur_sec + tick_sec
    local tick_id = allot.AllotID()
    if __G__GSTickTable._TickSecTable[target_sec] == nil then
        __G__GSTickTable._TickSecTable[target_sec] = {}
    end
    tick_table = {tick_id, target_sec, owner, func, reg_param}
    __G__GSTickTable._TickSecTable[target_sec][tick_id] = tick_table
    __G__GSTickTable._TickIDTable[tick_id] = tick_table
    return tick_id
end

function __G__GSTickTable.unreg_tick(owner, tick_id)
    --反注册Tick
    if __G__GSTickTable._TickIDTable[tick_id] == nil then
        return
    end
    tick_table = __G__GSTickTable._TickIDTable[tick_id]
    reg_owner = tick_table[3]
    if owner ~= nil and owner ~= reg_owner then
        --Tick的持有者必须要对应
        return
    end
    __G__GSTickTable._TickIDTable[tick_id] = nil
    target_sec = tick_table[2]
    if __G__GSTickTable._TickSecTable[target_sec] == nil then
        return
    end
    __G__GSTickTable._TickSecTable[target_sec][tick_id] = nil
end


function __G__GSTickTable.call_per_second()
    --每秒调用
    cur_sec = cGameServer.Seconds()
    if __G__GSTickTable._TickSecTable[cur_sec] == nil then
        return
    end
    for i, v in pairs(__G__GSTickTable._TickSecTable[cur_sec]) do
        local tick_id = v[1]
        local owner = v[3]
        local func = v[4]
        local reg_param = v[5]
        func(owner, reg_param)
        __G__GSTickTable._TickIDTable[tick_id] = nil
    end
    __G__GSTickTable._TickSecTable[cur_sec] = nil
end

return __G__GSTickTable