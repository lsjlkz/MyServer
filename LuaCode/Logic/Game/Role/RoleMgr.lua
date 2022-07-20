---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:41
--- Desc:
---

RoleClass = require("Common/Game/Role/RoleClass")

__G__RoleMgrTable = __G__RoleMgrTable or {}


function __G__RoleMgrTable.CreateRole(id, name)
    return RoleClass.CreateRole(id, name)
end




return __G__RoleMgrTable