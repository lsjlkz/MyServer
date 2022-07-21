---
--- Created by lsjlkz.
--- DateTime: 2022/7/21 15:27
--- Desc:
---


__G__SERVERTABLE = __G__SERVERTABLE or {}

function __G__SERVERTABLE.init()
    local mysql = require("luasql.mysql").mysql()
    local con = mysql:connect("world", "root", "root", "localhost", 3306)
    print(con)
    local cur = con:execute("select * from city")
    local row = cur:fetch({}, "a")

    while row do
        var = string.format("%s %s", row.ID, row.Name)
        print(var)
    end
end


return __G__SERVERTABLE