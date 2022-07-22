---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 11:23
--- Desc:
---

local s = require("Common/String")
local p = require("Common/Print")
local gs_event = require("Server/GSEvent")
__G__PersistentDataTable = __G__PersistentDataTable or {}


__G__PersistentDataTable.DataTable = {}


function __G__PersistentDataTable.test()
    --load from mysql
    --local rm = require("Server/PersistentData")
    --rm.load_data()

    --write to mysql
    --local rm = require("Server/PersistentBase")
    --local role = rm:new("DebugTable")
    --role:set_key_value(3, 4)
    --role:set_key_value(5, 6)
    --role:set_key_value(7, 8)
    --local sdb = require("ServerDB/ServerData")
    --sdb.save_data()
end

local function after_load_all_script(reg_param, p1, p2, p3)
    print(reg_param, p1, p2, p3)
end

function __G__PersistentDataTable.init()
    gs_event.reg_event(gs_event.AfterLoadAllScripts, after_load_all_script, "__G__PersistentDataTable")
end


function __G__PersistentDataTable.load_data()
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect("world", db_define.User, db_define.Password, db_define.Host, db_define.Port)
    local cur = con:execute("select name, data from persistentdata;")
    local row = cur:fetch({}, "a")
    while row do
        __G__PersistentDataTable.DataTable[row.name] = s.unSerialize(row.data)
        row = cur:fetch(row,"a")
    end
end


function __G__PersistentDataTable.save_data()
    --    保存数据
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect("world", db_define.User, db_define.Password, db_define.Host, db_define.Port)
    for k, v in pairs(__G__PersistentDataTable.DataTable) do
        local serialize_str = s.Serialize(v)
        cur = con:execute(string.format("INSERT INTO persistentdata (name, data) values ('%s', '%s') ON DUPLICATE KEY UPDATE name = '%s', data='%s';", k, serialize_str, k, serialize_str))
    end
end

return __G__PersistentDataTable