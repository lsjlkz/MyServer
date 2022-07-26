---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:11
--- Desc:
---


--lua角色类
Role = Role or {}

local role_enum = require("Common/Game/Enum/EnumRoleInt")

function Role:new(id, name)
    local o = {
        RoleID = id or 0,
        RoleName = name or "",
        IntTable = {},
        ObjTable = {}
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

function Role:SetInt(role_index, value)
    self.IntTable[role_index] = value
end

function Role:SetObj(role_index, tvalue)
    self.ObjTable[role_index] = tvalue
end

__G__RoleTable = __G__RoleTable or {}

function __G__RoleTable.CreateRole(id, name)
    local role = Role:new(id, name)
    role:SetInt(role_enum.RoleUpdateVersion, 1)
    role:SetInt(role_enum.RoleLevel, 1)
    return role
end

function __G__RoleTable.LoadRole(id, name, int_table, obj_table)
    local role = Role:new(id, name)
    role.IntTable = int_table
    role.ObjTable = obj_table
    return role
end

return __G__RoleTable