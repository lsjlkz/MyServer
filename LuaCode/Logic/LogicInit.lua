---
--- Created by lsjlkz.
--- DateTime: 2022/6/8 17:31
--- Desc:
---

__G__LogicTable = __G__LogicTable or {}
local cDefineTable = require("Common/CDefine")

function __G__LogicTable.test(param)
    print(param)
end


function __G__LogicTable.Init()
    print("Logic Start...")
    local cGameServer = require("cGameServer")
    cGameServer.CreateNetwork(100, cDefineTable.LogicID, 10010)

    local f = require("Common/Module")
    f.load_all_module("Common")
    f.load_all_module("Logic")

    local rm = require("Logic/Game/Role/RoleMgr")
    local role = rm.CreateRole(6542, "123")

end

return __G__LogicTable
