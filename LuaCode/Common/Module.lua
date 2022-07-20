---
--- Created by lsjlkz.
--- DateTime: 2022/6/1 17:37
--- Desc:
---
lfs = require("lfs")

__G__Module_Table = __G__Module_Table or {}

function __G__Module_Table.get_root_folder()
    local currentDir = lfs.currentdir()
    return currentDir
end

function __G__Module_Table.get_lua_root_folder()
    server_root_folder = lfs.currentdir()
    idx = string.find(server_root_folder, "MyServer")
    return string.sub(server_root_folder, 1, idx + 8) .. "LuaCode\\"
end

function __G__Module_Table.findindir (path, wefind, r_table, intofolder)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'\\'..file
            if string.find(f, wefind) ~= nil then
                table.insert(r_table, f)
            end
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" and intofolder then
                __G__Module_Table.findindir (f, wefind, r_table, intofolder)
            else
                local idx = string.find(f, "LuaCode")
                local s = string.sub(f, idx + 8, -5)
                local l = require(s)
                if type(l) == "table" then
                    if l['init'] ~= nil then
                        l['init']()
                        end
                end
                --for name, value in pairs(attr) do
                --
                --end
                end
        end
    end
end


function __G__Module_Table.load_all_module(package_name)
    currentFolder = __G__Module_Table.get_lua_root_folder()
    currentFolder = currentFolder .. package_name
    input_table = {}
    __G__Module_Table.findindir(currentFolder, "%.lua", input_table, true)--查找lua文件，这里可以改的
end

return __G__Module_Table