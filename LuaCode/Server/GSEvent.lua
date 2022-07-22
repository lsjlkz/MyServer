---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 15:20
--- Desc:
---事件的触发，因为事件在不同进程中独立，只在同一进程中关联，所以可以直接分配

local cur_event_allot_id = 0

local function allot_event_id()
    cur_event_allot_id = cur_event_allot_id - 1
    return cur_event_allot_id
end

local function make_event()
    return allot_event_id()
end


__G__GSEventTable = __G__GSEventTable or {}

__G__GSEventTable._Event_Function = {}


__G__GSEventTable.AfterLoadAllScripts = make_event()
__G__GSEventTable.AfterInitLogic = make_event()
__G__GSEventTable.AfterLoadPersistentTable = make_event()
__G__GSEventTable.AfterInitWorldMap = make_event()
__G__GSEventTable.AfterCallPerSecond = make_event()
__G__GSEventTable.AfterCallPerMinute = make_event()
__G__GSEventTable.AfterCallPerHour = make_event()
__G__GSEventTable.AfterCallPerDay = make_event()


function __G__GSEventTable.reg_event(event_id, func, reg_param)
    if __G__GSEventTable._Event_Function[event_id] == nil then
        __G__GSEventTable._Event_Function[event_id] = {}
    end
    table.insert(__G__GSEventTable._Event_Function[event_id], {func, reg_param})
end


function __G__GSEventTable.trigger_event(event_id, param1, param2, param3, param4)
    if __G__GSEventTable._Event_Function[event_id] ~= nil then
        for _, v in ipairs(__G__GSEventTable._Event_Function[event_id]) do
            func = v[1]
            reg_param = v[2]
            func(reg_param, param1, param2, param3, param4)
        end
    end
end


return __G__GSEventTable