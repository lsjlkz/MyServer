---
--- Created by lsjlkz.
--- DateTime: 2022/7/20 17:41
--- Desc:
---

local RoleClass = require("Common/Game/Role/RoleClass")
local p = require("Common/Print")
local s = require("Common/String")
local logic_role_data = require("Logic/Game/Role/RoleData")
local gs_event = require("Server/GSEvent")

__G__RoleMgrTable = __G__RoleMgrTable or {}

function __G__RoleMgrTable.DBCreateRole(role_name)
    --DB创建角色，返回id
    local roledb = require("ServerDB/RoleData")
    local role_id = roledb.CreateRole(role_name)
    local role = RoleClass.CreateRole(role_id, role_name)
    logic_role_data.RoleDataIDTable[role_id] = role
    logic_role_data.RoleDataNameTable[role_name] = role
    return role
end

function __G__RoleMgrTable.CreateRole(role_name)
    if logic_role_data.RoleDataNameTable[role_name] ~= nil then
        print("role name repeat")
        return
    end
    return __G__RoleMgrTable.DBCreateRole(role_name)
end

function __G__RoleMgrTable.LoadRole(role_id, role_name, int_table, obj_table)
    local role = RoleClass.LoadRole(role_id, role_name, int_table, obj_table)
    logic_role_data.RoleDataIDTable[role_id] = role
    logic_role_data.RoleDataNameTable[role_name] = role
    return role
end

function __G__RoleMgrTable.init()
    GSEvent.RegEvent(gs_event.AfterLoadAllScripts, __G__RoleMgrTable.after_load_script)
    GSEvent.RegEvent(gs_event.BeforeServerClose, __G__RoleMgrTable.before_server_close)
    GSEvent.RegEvent(gs_event.AfterLoadAllRole, __G__RoleMgrTable.after_load_all_role)
end

function __G__RoleMgrTable.before_server_close()
    local roledb = require("ServerDB/RoleData")
    roledb.save_all_data(logic_role_data.RoleDataIDTable)
end

function __G__RoleMgrTable.load_role()

    local roledb = require("ServerDB/RoleData")
    local t = roledb.load_data()
    local cnt = 0
    for i, v in ipairs(t) do
        cnt = cnt + 1
        local role_id = v[1]
        local role_name = v[2]
        local int_table = s.unSerialize(v[3])
        local obj_table = s.unSerialize(v[4])
        __G__RoleMgrTable.LoadRole(role_id, role_name, int_table, obj_table)
    end
    print("load role end... cnt:", cnt)
end

function __G__RoleMgrTable.after_load_script()
    __G__RoleMgrTable.load_role()
    GSEvent.TriggerEvent(gs_event.AfterLoadAllRole)
end

function __G__RoleMgrTable.after_load_all_role()
    __G__RoleMgrTable.CreateRole("debug_role")
    __G__RoleMgrTable.CreateRole("debug_role_2")
end


return __G__RoleMgrTable