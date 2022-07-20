---
--- Created by lsjlkz.
--- DateTime: 2022/6/1 17:37
--- Desc:
---

__G__Module_Table = __G__Module_Table or {}

function __G__Module_Table.get_root_folder()
    path = debug.getinfo(1, 'S').source:sub(2)
    return path
end

function __G__Module_Table.findindir (path, wefind, r_table, intofolder)
    for file in lfs.dir(path) do
        print(file)
        if file ~= "." and file ~= ".." then
            local f = path..'\\'..file
            --print ("/t "..f)
            if string.find(f, wefind) ~= nil then
                --print("/t "..f)
                table.insert(r_table, f)
            end
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" and intofolder then
                findindir (f, wefind, r_table, intofolder)
            else
                --for name, value in pairs(attr) do
                --    print (name, value)
                --end
            end
        end
    end
end


function __G__Module_Table.load_all_module()
    __G__Module_Table.findindir(currentFolder, "%.lua", input_table, false)--查找lua文件，这里可以改的
end

return __G__Module_Table