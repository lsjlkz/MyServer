---
--- Created by lsjlkz.
--- DateTime: 2022/7/21 15:27
--- Desc:
---


__G__SERVERTABLE = __G__SERVERTABLE or {}


function __G__SERVERTABLE.init()
    local mysql = require("luasql.mysql").mysql()
    local db_define = require("ServerDB/ServerDBDefine")
    local con = mysql:connect("world", db_define.User, db_define.Password, db_define.Host, db_define.Port)
    local cur = con:execute("select * from city")
    local row = cur:fetch({}, "a")

    while row do

        var = string.format("%s %s\n", row.ID, row.Name)

        print(var)


        row = cur:fetch(row,"a")
    end
end


return __G__SERVERTABLE