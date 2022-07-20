---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:32
--- Desc:
---

__G__GateWayTable = __G__GateWayTable or {}
local cDefineTable = require("Common/CDefine")

function __G__GateWayTable.init()
    print("GateWay Start...")
    local cGameServer = require("cGameServer")
    cGameServer.CreateNetwork(100, cDefineTable.GateWayID, 10086)
end

return __G__GateWayTable