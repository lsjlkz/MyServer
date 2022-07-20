---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:11
--- Desc:
---


--lua角色类
Role = Role or {}

Role.RoleID = 0
Role.RoleName = ""

function Role:new(o, id, name)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.RoleID = id
    self.RoleName = name
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
    return Role:new(nil, id, name)
end
return __G__RoleTable