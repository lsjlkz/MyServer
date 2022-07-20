---
--- Created by lsjlkz.
--- DateTime: 2022/6/2 11:12
--- Desc:
---

__G__WorldTable = __G__WorldTable or {}
local cDefineTable = require("Common/CDefine")

function __G__WorldTable.init()
    print("World Start...")
    local cGameServer = require("cGameServer")
    cGameServer.CreateNetwork(100, cDefineTable.WorldID, 10000)
end

return __G__WorldTable