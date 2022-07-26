---
--- Created by lsjlkz.
--- DateTime: 2022/7/21 15:27
--- Desc:
---


__G__ServerTable = __G__ServerTable or {}


function __G__ServerTable.init()
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect("world", db_define.User, db_define.Password, db_define.Host, db_define.Port)
    con:close()
end

function __G__ServerTable.save_data()
    local persistent = require("Server/PersistentData")
    persistent.save_data()
end


return __G__ServerTable