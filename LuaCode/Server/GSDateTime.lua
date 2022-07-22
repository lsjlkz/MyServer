---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 16:50
--- Desc:
---

local gs_event = require("Server/GSEvent")

__G__GSDateTimeTable = __G__GSDateTimeTable or {}



function __G__GSDateTimeTable.CCallPerSecond()
    local cGameServer = require("cGameServer")
    print("CCallPerSecond", cGameServer.Seconds())
    gs_event.trigger_event(gs_event.AfterCallPerSecond)
end


function __G__GSDateTimeTable.CCallPerMinute()
    local cGameServer = require("cGameServer")
    print("CCallPerMinute", cGameServer.Minute())
    gs_event.trigger_event(gs_event.AfterCallPerMinute)
end

function __G__GSDateTimeTable.CCallPerHour()
    local cGameServer = require("cGameServer")
    print("CCallPerHour", cGameServer.Hour())
    gs_event.trigger_event(gs_event.AfterCallPerHour)
end

function __G__GSDateTimeTable.CCallPerDay()
    local cGameServer = require("cGameServer")
    print("CCallPerDay", cGameServer.Day())
    gs_event.trigger_event(gs_event.AfterCallPerDay)
end

return __G__GSDateTimeTable