---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 17:43
--- Desc:
---
local cGameServer = require("cGameServer")


__G__AllotIDTable = __G__AllotIDTable or {}

function __G__AllotIDTable.AllotID()
    math.randomseed(cGameServer.Seconds())
    return cGameServer.Seconds() * 10000 + math.random(0, 9999)
end



return __G__AllotIDTable