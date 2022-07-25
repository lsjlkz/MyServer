---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by lsjlkz.
--- DateTime: 2022/7/25 23:05
---


__G__EnumRoleIntTable = __G__EnumRoleIntTable or {}

__G__EnumRoleIntTable.save_role_index_table = {}
__G__EnumRoleIntTable.log_role_index_table = {}

local un_save = 0
local do_save = 1

local un_log = 0
local do_log = 1


local function enum_role_int(role_index, save, log)
    if save == do_save then
        __G__EnumRoleIntTable.save_role_index_table[role_index] = true
    end
    if log == do_log then
        __G__EnumRoleIntTable.log_role_index_table[role_index] = true
    end
    return role_index
end

__G__EnumRoleIntTable.RoleUpdateVersion = enum_role_int(1, do_save, do_log)
__G__EnumRoleIntTable.RoleLevel = enum_role_int(2, do_save, do_log)


return __G__EnumRoleIntTable