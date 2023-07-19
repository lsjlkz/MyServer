---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:32
--- Desc:
---

__G__GateWayTable = __G__GateWayTable or {}
local cDefineTable = require("Common/CDefine")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_gateway_module()
    cGameServer.LoadModule("Common")
    cGameServer.LoadModule("Server")
    cGameServer.LoadModule("GateWay")
    GSEvent.TriggerEvent(gs_event.AfterLoadAllScripts)
    cGameServer.RegTick(150, nil, cGameServer.SetServerStop, nil)
    cGameServer.RegTick(200, nil, cGameServer.SetServerStop, nil)
    cGameServer.RegTick(300, nil, cGameServer.SetServerStop, nil)
end

local function test_reg(a, b, c, d, e)
    print("test_reg" .. a .. b .. c .. d .. e)
end

local function test_tick(owner, callargv, param)
    print('test_tick')
end

function __G__GateWayTable.Init()
    cGameServer.SetProcessName("GateWay")
    cGameServer.CreateNetwork(2, cDefineTable.GateWayID, cDefineTable.Port_Gateway)

    --GSEvent.RegEvent(gs_event.AfterLoadAllScripts, test_reg)
    --GSEvent.TriggerEvent(gs_event.AfterLoadAllScripts, 4, 5, 1, 2)
    --cGameServer.RegTick(3, nil, test_tick, 5)
    load_gateway_module()
end


return __G__GateWayTable