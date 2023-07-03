---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 11:23
--- Desc:
---

local s = require("Common/String")
local p = require("Common/Print")
local gs_event = require("Server/GSEvent")
__G__PersistentDataTable = __G__PersistentDataTable or {}


__G__PersistentDataTable.DataTable = __G__PersistentDataTable.DataTable or {}


function __G__PersistentDataTable.test()
    --write to mysql
    local rm = require("ServerDB/PersistentBase")
    local debug_table = rm:new("DebugTable")
    debug_table:set_key_value(3, 4)
    debug_table:set_key_value(5, 6)
    debug_table:set_key_value(7, 8)
end



local function load_data()
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect(db_define.PersistentDBName, db_define.User, db_define.Password, db_define.Host, db_define.Port)
    local cur = con:execute(string.format("select table_name, table_data from %s;", db_define.PersistentTableName))
    local row = cur:fetch({}, "a")
    local cnt = 0
    while row do
        cnt = cnt + 1
        __G__PersistentDataTable.DataTable[row.table_name] = s.unSerialize(row.table_data)
        row = cur:fetch(row,"a")
    end
    con:close()
    print("load persistent data end...cnt:", cnt)
    GSEvent.TriggerEvent(gs_event.AfterLoadAllPersistentData)
end


local function save_data()
    --    保存数据
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect(db_define.PersistentDBName, db_define.User, db_define.Password, db_define.Host, db_define.Port)
    for k, v in pairs(__G__PersistentDataTable.DataTable) do
        local serialize_str = s.Serialize(v)
        cur = con:execute(string.format("insert into %s (table_name, table_data) values ('%s', '%s') on duplicate key update table_name = '%s', table_data='%s';", db_define.PersistentTableName, k, serialize_str, k, serialize_str))
    end
    con:close()
    print("save persistent data end...")
end

function __G__PersistentDataTable.init()
    GSEvent.RegEvent(gs_event.AfterLoadAllScripts, load_data)
    GSEvent.RegEvent(gs_event.BeforeServerClose, save_data)
    GSEvent.RegEvent(gs_event.AfterLoadAllPersistentData, __G__PersistentDataTable.after_load_all_persistent_data)
end

function __G__PersistentDataTable.after_load_all_persistent_data()
    __G__PersistentDataTable.test()
end


return __G__PersistentDataTable