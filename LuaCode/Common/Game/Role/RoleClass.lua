---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:11
--- Desc:
---


--lua角色类
Role = Role or {}

Role.RoleID = 0
Role.RoleName = ""

function Role:new(id, name)
    o = o or {
        RoleID = id or 0,
        RoleName = name or ""
    }
    setmetatable(o, {__index = self})
    return o
end

function Role:GetRoleID()
    return self.RoleID
end

function Role:GetRoleName()
    return self.RoleName
end

__G__RoleTable = __G__RoleTable or {}

function __G__RoleTable.CreateRole(id, name)
    return Role:new(id, name)
end
return __G__RoleTable