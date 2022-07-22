---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 16:22
--- Desc:
---延迟n秒触发的方法

local cGameServer = require("cGameServer")
local gs_event = require("Server/GSEvent")
local allot = require("Server/Allot")

__G__GSTickTable = __G__GSTickTable or {}
__G__GSTickTable._TickIDTable = {}
__G__GSTickTable._TickSecTable = {}

function __G__GSTickTable.test1()
    __G__GSTickTable.reg_tick(nil, 3, __G__GSTickTable.test2, {1,3,5})
end

function __G__GSTickTable.test2(owner, reg_param)
    print(owner, reg_param)
end

function __G__GSTickTable.init()
    gs_event.reg_event(gs_event.AfterCallPerSecond, __G__GSTickTable.call_per_second)
    gs_event.reg_event(gs_event.AfterLoadAllScripts, __G__GSTickTable.test1)
end


function __G__GSTickTable.reg_tick(owner, tick_sec, func, reg_param)
    if tick_sec <= 0 then
        func(owner, reg_param)
        return
    end
    local cur_sec = cGameServer.Seconds()
    local target_sec = cur_sec + tick_sec
    local tick_id = allot.AllotID()
    if __G__GSTickTable._TickSecTable[target_sec] == nil then
        __G__GSTickTable._TickSecTable[target_sec] = {}
    end
    tick_table = {tick_id, target_sec, owner, func, reg_param}
    table.insert(__G__GSTickTable._TickSecTable[target_sec], tick_table)
    __G__GSTickTable._TickIDTable[tick_id] = tick_table
    return tick_id
end

function __G__GSTickTable.unreg_tick(owner, tick_id)
    if __G__GSTickTable._TickIDTable[tick_id] == nil then
        return
    end
    tick_table = __G__GSTickTable._TickIDTable[tick_id]
    reg_owner = tick_table[3]
    if owner ~= nil and owner ~= reg_owner then
        return
    end
    target_sec = tick_table[2]
    if __G__GSTickTable._TickSecTable[target_sec] == nil then
        return
    end
    for i, v in ipairs(__G__GSTickTable._TickSecTable[target_sec]) do
        reg_tick = v[1]
        if reg_tick == tick then
            print("remove tick", reg_func, reg_param)
        end
    end
end


function __G__GSTickTable.call_per_second()
    cur_sec = cGameServer.Seconds()
    if __G__GSTickTable._TickSecTable[cur_sec] == nil then
        return
    end
    for i, v in ipairs(__G__GSTickTable._TickSecTable[cur_sec]) do
        tick_id = v[1]
        owner = v[3]
        func = v[4]
        reg_param = v[5]
        func(owner, reg_owner)
        --这里应该需要优化
        table.remove(__G__GSTickTable._TickIDTable, tick_id)
    end
    table.remove(__G__GSTickTable._TickSecTable, cur_sec)
end

return __G__GSTickTable