---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 15:20
--- Desc:
---事件的触发，因为事件在不同进程中独立，只在同一进程中关联，所以可以直接分配

local cur_event_allot_id = 0
local p = require("Common/Print")

--为什么这里用负数，因为玩家身上存储了正数的索引，当玩家的数据改变后，也会触发事件
local function allot_event_id()
    cur_event_allot_id = cur_event_allot_id - 1
    return cur_event_allot_id
end

local function make_event()
    return allot_event_id()
end


__G__GSEventTable = __G__GSEventTable or {}

__G__GSEventTable._Event_Function = __G__GSEventTable._Event_Function or {}
__G__GSEventTable._DelayEvent_Function = __G__GSEventTable._DelayEvent_Function or {}
__G__GSEventTable._DelayEvent_Trigger = __G__GSEventTable._DelayEvent_Trigger or {}


--事件分配

__G__GSEventTable.BeforeServerClose = make_event()
__G__GSEventTable.AfterLoadAllScripts = make_event()
__G__GSEventTable.AfterInitLogic = make_event()
__G__GSEventTable.AfterLoadPersistentTable = make_event()
__G__GSEventTable.AfterInitWorldMap = make_event()
__G__GSEventTable.AfterCallPerSecond = make_event()
__G__GSEventTable.AfterCallPerMinute = make_event()
__G__GSEventTable.AfterCallPerHour = make_event()
__G__GSEventTable.AfterCallPerDay = make_event()
__G__GSEventTable.TestDelayEvent = make_event()
__G__GSEventTable.AfterLoadAllRole = make_event()



function __G__GSEventTable.reg_event(event_id, func, reg_param)
    if __G__GSEventTable._Event_Function[event_id] == nil then
        __G__GSEventTable._Event_Function[event_id] = {}
    end
    table.insert(__G__GSEventTable._Event_Function[event_id], {func, reg_param})
end

function __G__GSEventTable.reg_delay_event(event_id, func, reg_param)
    if __G__GSEventTable._DelayEvent_Function[event_id] == nil then
        __G__GSEventTable._DelayEvent_Function[event_id] = {}
    end
    table.insert(__G__GSEventTable._DelayEvent_Function[event_id], {func, reg_param})
end

function __G__GSEventTable.trigger_event(event_id, param1, param2, param3, param4)
    if __G__GSEventTable._Event_Function[event_id] == nil then
        return
    end
    for _, v in ipairs(__G__GSEventTable._Event_Function[event_id]) do
        func = v[1]
        reg_param = v[2]
        func(reg_param, param1, param2, param3, param4)
    end
end

function __G__GSEventTable.trigger_delay_event(event_id, param1, param2, param3, param4)
    if __G__GSEventTable._DelayEvent_Function[event_id] == nil then
        return
    end
    __G__GSEventTable._DelayEvent_Trigger[event_id] = {event_id, param1, param2, param3, param4}
end

local function call_per_sec_delay_event()
    if __G__GSEventTable._DelayEvent_Trigger == {} then
        return
    end
    for event_id, param in pairs(__G__GSEventTable._DelayEvent_Trigger) do
        param1 = param[1]
        param2 = param[2]
        param3 = param[3]
        param4 = param[4]
        if __G__GSEventTable._DelayEvent_Function[event_id] ~= nil then
            for _, v in ipairs(__G__GSEventTable._DelayEvent_Function[event_id]) do
                func = v[1]
                reg_param = v[2]
                func(reg_param, param1, param2, param3, param4)
            end
        end
    end
    __G__GSEventTable._DelayEvent_Trigger = {}
end

function __G__GSEventTable.init()
    --注册每秒的事件
    __G__GSEventTable.reg_event(__G__GSEventTable.AfterCallPerSecond, call_per_sec_delay_event)
end

function __G__GSEventTable.test()
    __G__GSEventTable.reg_delay_event(__G__GSEventTable.TestDelayEvent, __G__GSEventTable.test1)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
    __G__GSEventTable.trigger_delay_event(__G__GSEventTable.TestDelayEvent, 3, 4, 5, 6)
end

function __G__GSEventTable.test1(param1, param2, param3, param4)
    print('test1')
    print(param1, param2, param3, param4)
end


return __G__GSEventTable