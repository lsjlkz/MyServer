---
--- Created by lsjlkz.
--- DateTime: 2022/7/26 10:08
--- Desc:
---

local gs_event = require("Server/GSEvent")

__G__CreateTable = __G__CreateTable or {}




__G__CreateTable._CreateTable = {}

CreateTable = {}

__G__CreateTable.Int = "int"
__G__CreateTable.ShortVChar = "varchar(20)"
__G__CreateTable.LongVChar = "varchar(60)"
__G__CreateTable.BLOB = "blob"

function CreateTable:add_field(field_name, field_type, is_key)
    self.FieldTable[field_name] = field_type
    if is_key ~= nil then
        table.insert(self.PrimaryKeyTable, field_name)
        self.PrimaryKeyTable[field_name] = true
    end
end


function CreateTable:get_sql()
    sql = string.format("create table if not exists %s.%s (\n", self.DBName, self.TableName)
    for field_name, field_type in pairs(self.FieldTable) do
        sql = sql .. "`" .. field_name .. "` " .. field_type
        if self.PrimaryKeyTable[field_name] ~= nil then
            sql = sql .. " not null"
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
        PrimaryKeyTable = {}
    }
    setmetatable(o, {__index=self})
    table.insert(__G__CreateTable._CreateTable, o)
    return o
end

function __G__CreateTable.init()
    gs_event.reg_event(gs_event.AfterLoadAllScripts, __G__CreateTable.CreateAllTable)
end

function __G__CreateTable.CreateAllTable()
    print("create mysql table start...")
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    for i, table_obj in ipairs(__G__CreateTable._CreateTable) do
        local con = mysql:connect(table_obj.DBName, db_define.User, db_define.Password, db_define.Host, db_define.Port)
        con:execute(table_obj:get_sql())
        con:close()
    end
    print("create mysql table end...")
end

function __G__CreateTable.CreateTable(table_name, dbname)
    t = CreateTable:new(table_name, dbname)
    return t
end


return __G__CreateTable