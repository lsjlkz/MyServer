---
--- Created by lsjlkz.
--- DateTime: 2022/6/1 17:15
--- Desc:
---


package.path = package.path .. ";../LuaCode/?.lua;"
local cDefineTable = require("Common/CDefine")

local function get_this_lua_path()
    module = require("Common/Module")
    path = module.get_root_folder()
    return path
end


local function world()
    --初始化游戏世界
    local world_init = require("World/WorldInit")
    world_init.init()
end

local function logic()
    local logic_init = require("Logic/LogicInit")
    logic_init.init()
end

local function gateway()
    local gateway_init = require("GateWay/GateWayInit")
    gateway_init.init()
end


local initFuncTable = { world, gateway, logic}

local function main()
    print("GSInit start...")
    local cGameServer = require("cGameServer")
    initFuncTable[cGameServer.GetGameServerID()]()
end


main()
