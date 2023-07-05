---
--- Created by lsjlkz.
--- DateTime: 2022/6/2 11:12
--- Desc:
---

__G__WorldTable = __G__WorldTable or {}
local cDefineTable = require("Common/CDefine")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_world_module()
    cGameServer.LoadModule("Common")
    cGameServer.LoadModule("Server")
    cGameServer.LoadModule("ServerDB")
    cGameServer.LoadModule("World")
    GSEvent.TriggerEvent(gs_event.AfterLoadAllScripts)
    cGameServer.RegTick(3, nil, test_close, 3)
end


function __G__WorldTable.Init()
    cGameServer.CreateNetwork(100, cDefineTable.WorldID, cDefineTable.Port_World)
    cGameServer.SetProcessName("World")
    load_world_module()
end

return __G__WorldTable