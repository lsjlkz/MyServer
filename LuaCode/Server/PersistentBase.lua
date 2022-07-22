---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 9:35
--- Desc:
---

local d = require("Server/PersistentData")

__G__PersistentBaseTable = __G__PersistentBaseTable or {}


function __G__PersistentBaseTable:new(name)
    o = o or {
        TableName = name or "",
        DataTable = {},
        Version = 0
    }
    setmetatable(o, {__index = self})
    d.DataTable[name] = o
    return o
end

function __G__PersistentBaseTable:set_key_value(key, value)
    self.DataTable[key] = value
end

function __G__PersistentBaseTable:get_key_value(key)
    return self.DataTable[key]
end

function __G__PersistentBaseTable:get_version()
    return self.Version
end

function __G__PersistentBaseTable:has_change()
    self.Version  = self.Version + 1
end

return __G__PersistentBaseTable