---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:31
--- Desc:
---

__G__LogicTable = __G__LogicTable or {}
local cDefineTable = require("Common/CDefine")
local gs_event = require("Server/GSEvent")
local f = require("Common/Module")

local function load_logic_module()
    f.load_all_module("Common")
    f.load_all_module("Logic")
    f.load_all_module("Server")
    gs_event.trigger_event(gs_event.AfterLoadAllScripts)
end

function __G__LogicTable.Init()
    local cGameServer = require("cGameServer")
    cGameServer.CreateNetwork(100, cDefineTable.LogicID, 10010)
    load_logic_module()

end

return __G__LogicTable
