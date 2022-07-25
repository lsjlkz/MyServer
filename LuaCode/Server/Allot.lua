---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 17:43
--- Desc:
---
local cGameServer = require("cGameServer")

__G__AllotIDTable = __G__AllotIDTable or {}

__G__AllotIDTable.curID = 0

function __G__AllotIDTable.AllotID()
    __G__AllotIDTable.curID = (__G__AllotIDTable.curID + 1) % 10000
    return cGameServer.Seconds() * 10000 + __G__AllotIDTable.curID
end



return __G__AllotIDTable