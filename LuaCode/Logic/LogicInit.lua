---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:31
--- Desc:
---

__G__LogicTable = __G__LogicTable or {}
local cDefineTable = require("Common/CDefine")
local gs_event = require("Server/GSEvent")
local f = require("Common/Module")
local cGameServer = require("cGameServer")
local gs_tick = require("Server/GSTick")

local function test_close()
    cGameServer.SetServerStop()
end

local function load_logic_module()
    f.load_all_module("Common")
    f.load_all_module("Logic")
    f.load_all_module("Server")
    f.load_all_module("ServerDB")
    gs_event.trigger_event(gs_event.AfterLoadAllScripts)
    gs_tick.reg_tick(nil, 3, test_close)

end

function __G__LogicTable.Init()
    cGameServer.CreateNetwork(100, cDefineTable.LogicID, 10010)
    load_logic_module()
end

return __G__LogicTable
