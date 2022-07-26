---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:41
--- Desc:
---

local RoleClass = require("Common/Game/Role/RoleClass")
local p = require("Common/Print")

__G__RoleMgrTable = __G__RoleMgrTable or {}


function __G__RoleMgrTable.CreateRole(id, name)
    return RoleClass.CreateRole(id, name)
end

function __G__RoleMgrTable.LoadRole(id, name, int_table, obj_table)
    --TODO
    return RoleClass.LoadRole(id, name, int_table, obj_table)
end


function __G__RoleMgrTable.init()
    role_1 = __G__RoleMgrTable.CreateRole(1, "1")
    role_2 = __G__RoleMgrTable.CreateRole(2, "2")
    role_3 = __G__RoleMgrTable.CreateRole(3, "3")
end


return __G__RoleMgrTable