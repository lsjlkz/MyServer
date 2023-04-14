---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:31
--- Desc:
---

__G__LogicTable = __G__LogicTable or {}
local cDefineTable = require("Common/CDefine")
local f = require("Common/Module")
local cGameServer = require("cGameServer")
local gs_tick = require("Server/GSTick")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_logic_module()
    f.load_all_module("Common")
    f.load_all_module("Server")
    f.load_all_module("ServerDB")
    f.load_all_module("Logic")
    gs_event.trigger_event(gs_event.AfterLoadAllScripts)
    local gsinit = require("Server/GSInit")
    gs_tick.reg_tick(nil, 300, gsinit.close)

end

function __G__LogicTable.Init()
    load_logic_module()
    cGameServer.SetConnectParam(100, 100, 100, 100, 100, 100)
    cGameServer.CreateNetwork(100, cDefineTable.LogicID, 10010)
    cGameServer.SetFSCoutName("Logic")
end

return __G__LogicTable
