---
--- Created by lsjlkz.
--- DateTime: 2022/7/22 10:47
--- Desc:
---


local p = require("Common/Print")

__G__StringTable = __G__StringTable or {}

__G__StringTable.CanSerialzeType = {
    number = "number",
    boolean = "boolean",
    string = "string",
    table = "table"}

function __G__StringTable.Serialize(obj)
    local lua = ""
    local t = type(obj)
    if __G__StringTable.CanSerialzeType[t] == nil then
        return nil
    end
    if t == "number" then
        lua = lua .. obj
    elseif t == "boolean" then
        lua = lua .. tostring(obj)
    elseif t == "string" then
        lua = lua .. string.format("%q", obj)
    elseif t == "table" then
        lua = lua .. "{\n"
        for k, v in pairs(obj) do
            if __G__StringTable.CanSerialzeType[type(v)] ~= nil then
                lua = lua .. "[" .. __G__StringTable.Serialize(k) .. "]=" .. __G__StringTable.Serialize(v) .. ",\n"
            end
        end
        local metatable = getmetatable(obj)
        if metatable ~= nil and type(metatable.__index) ~= "table" then
            for k, v in pairs(metatable.__index) do
                if __G__StringTable.CanSerialzeType[type(v)] ~= nil then
                    lua = lua .. "[" .. __G__StringTable.Serialize(k) .. "]=" .. __G__StringTable.Serialize(v) .. ",\n"
                end
            end
        end
        lua = lua .. "}"
    elseif t == "nil" then
        return nil
    else
        return ""
    end
    return lua
end

function __G__StringTable.unSerialize(lua)
    local t = type(lua)
    if t == "nil" or lua == "" then
        return nil
    elseif t == "number" or t == "string" or t == "boolean" then
        lua = tostring(lua)
    end
    lua = "return " .. lua
    local func = load(lua)
    if func == nil then
        return nil
    end
    return func()
end


return __G__StringTable