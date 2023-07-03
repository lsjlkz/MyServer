---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:31
--- Desc:
---

__G__LogicTable = __G__LogicTable or {}
local cDefineTable = require("Common/CDefine")
local f = require("Common/Module")
local gs_event = require("Server/GSEvent")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_logic_module()
    f.load_all_module("Common")
    f.load_all_module("Server")
    f.load_all_module("Logic")
    GSEvent.TriggerEvent(gs_event.AfterLoadAllScripts)
    local gsinit = require("Server/GSInit")
    cGameServer.RegTick(300, nil, gsinit.close, nil)

end

function __G__LogicTable.Init()
    load_logic_module()
    cGameServer.SetConnectParam(100, 100, 100, 100, 100, 100)
    cGameServer.CreateNetwork(100, cDefineTable.LogicID, cDefineTable.Port_Logic)
    cGameServer.SetProcessName("Logic")
end

return __G__LogicTable
