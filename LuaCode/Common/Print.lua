---
--- Created by lsjlkz.
--- DateTime: 2022/6/13 14:42
--- Desc:
---

__G__PrintTable = __G__PrintTable or {}


-- 用于友好的打印lua对象
function __G__PrintTable.pprint ( t )
    local print_r_cache={}
    local print_result = {}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            table.insert(print_result, indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos, val in pairs(t) do
                    if type(pos) == "boolean" then
                        pos = tostring(pos)
                    end
                    if (type(val)=="table") then
                        table.insert(print_result, indent.."["..pos.."] => "..tostring(t).." {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        table.insert(print_result, indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        table.insert(print_result, indent.."["..pos..'] => "'..val..'"')
                    else
                        table.insert(print_result, indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                table.insert(print_result, indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        table.insert(print_result, tostring(t).." {")
        sub_print_r(t,"  ")
        table.insert(print_result, "}")
    else
        sub_print_r(t,"  ")
    end
    table.insert(print_result, "")
    print(table.concat(print_result, "\n"))
end

return __G__PrintTable