---
--- Created by lsjlkz.
--- DateTime: 2022/7/26 10:08
--- Desc:
---

local gs_event = require("Server/GSEvent")
local db_define = require("ServerDB/ServerDBDefine")

__G__CreateTable = __G__CreateTable or {}




__G__CreateTable._CreateTable = __G__CreateTable._CreateTable or {}

CreateTable = {}

Int = "int"
ShortVChar = "varchar(20)"
LongVChar = "varchar(60)"
BLOB = "blob"

function CreateTable:add_field(field_name, field_type, is_key, auto_inc)
    self.FieldTable[field_name] = field_type
    if is_key ~= nil then
        table.insert(self.PrimaryKeyTable, field_name)
        self.PrimaryKeyTable[field_name] = true
    end
    if auto_inc ~= nil then
        table.insert(self.AutoIncKeyTable, field_name)
        self.AutoIncKeyTable[field_name] = true
    end
end


function CreateTable:get_sql()
    sql = string.format("create table if not exists %s.%s (\n", self.DBName, self.TableName)
    for field_name, field_type in pairs(self.FieldTable) do
        sql = sql .. "`" .. field_name .. "` " .. field_type
        if self.PrimaryKeyTable[field_name] ~= nil then
            sql = sql .. " not null"
        end
        if self.AutoIncKeyTable[field_name] ~= nil then
            sql = sql .. " auto_increment"
        end
        sql = sql .. ",\n"
    end
    k = table.concat(self.PrimaryKeyTable, "`,`")
    sql = sql .. "primary key (`" .. k .. "`));"
    return sql
end

function CreateTable:new(table_name, dbname)
    local o = o or {
        TableName = table_name,
        DBName = dbname,
        FieldTable = {},
        PrimaryKeyTable = {},
        AutoIncKeyTable = {}
    }
    setmetatable(o, {__index=self})
    table.insert(__G__CreateTable._CreateTable, o)
    return o
end

function __G__CreateTable.init()
    __G__CreateTable.TableInit()
end


function __G__CreateTable.TableInit()
    --角色表
    local tb = CreateTable:new(db_define.RoleTableName, db_define.RoleDBName)
    tb:add_field("role_id", Int, true, true)
    tb:add_field("role_name",ShortVChar)
    tb:add_field("int_table", BLOB)
    tb:add_field("obj_table", BLOB)

    tb = CreateTable:new(db_define.PersistentTableName, db_define.PersistentDBName)
    tb:add_field("table_name", ShortVChar, true)
    tb:add_field("table_data", BLOB)

    __G__CreateTable.CreateAllTable()
end


function __G__CreateTable.CreateAllTable()
    local mysql = require("luasql.mysql").mysql()
    for i, table_obj in ipairs(__G__CreateTable._CreateTable) do
        local con = mysql:connect(table_obj.DBName, db_define.User, db_define.Password, db_define.Host, db_define.Port)
        con:execute(table_obj:get_sql())
        con:close()
    end
    print("create mysql table end...")
end

return __G__CreateTable