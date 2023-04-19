---
--- Created by lsjlkz.
--- DateTime: 2022/6/2 11:12
--- Desc:
---

__G__WorldTable = __G__WorldTable or {}
local cDefineTable = require("Common/CDefine")
local f = require("Common/Module")
local cGameServer = require("cGameServer")
local gs_tick = require("Server/GSTick")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_world_module()
    f.load_all_module("Common")
    f.load_all_module("Server")
    f.load_all_module("ServerDB")
    f.load_all_module("World")
    gs_event.trigger_event(gs_event.AfterLoadAllScripts)
    gs_tick.reg_tick(nil, 3, test_close)
end


function __G__WorldTable.Init()
    cGameServer.CreateNetwork(100, cDefineTable.WorldID, 10000)
    cGameServer.SetProcessName("World")
    load_world_module()
end

return __G__WorldTable