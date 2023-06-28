---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:32
--- Desc:
---

__G__GateWayTable = __G__GateWayTable or {}
local cDefineTable = require("Common/CDefine")
local f = require("Common/Module")
local cGameServer = require("cGameServer")
local gs_tick = require("Server/GSTick")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_gateway_module()
    f.load_all_module("Common")
    f.load_all_module("Server")
    f.load_all_module("GateWay")
    gs_event.trigger_event(gs_event.AfterLoadAllScripts)
    gs_tick.reg_tick(nil, 300, test_close)
end

local function test_send()
    print("test_send")
    local a = {}
    local b = {}
    a[3] = b
    b[5] = -6
    a[4] = -5
    cGameServer.DebugSendMsg(0, 88, a)
    cGameServer.DebugSendMsg(1, 88, a)
    gs_tick.reg_tick(nil, 5, test_send)
end

function __G__GateWayTable.Init()
    cGameServer.SetProcessName("GateWay")
    cGameServer.CreateNetwork(2, cDefineTable.GateWayID, cDefineTable.Port_Gateway)
    print(cGameServer.TestSeconds())
    local v2 = Vector2(3,4)
    v2:print()
    load_gateway_module()
end

return __G__GateWayTable