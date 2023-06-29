---
--- Created by lsjlkz.
--- DateTime: 2022/6/1 17:15
--- Desc:
---



__G__GSInitTable = __G__GSInitTable or {}

package.path = package.path .. ";../LuaCode/?.lua;"
require("Server/ServerPrint")

function __G__GSInitTable.world()
    --初始化游戏世界
    print("__G__GSInitTable.world")
    local world_init = require("World/WorldInit")
    world_init.Init()
end

function __G__GSInitTable.logic()
    print("__G__GSInitTable.logic")
    local logic_init = require("Logic/LogicInit")
    logic_init.Init()
end

function __G__GSInitTable.gateway()
    print("__G__GSInitTable.gateway")
    local gateway_init = require("GateWay/GateWayInit")
    gateway_init.Init()
end

local initFuncTable = { __G__GSInitTable.world, __G__GSInitTable.gateway, __G__GSInitTable.logic}

function __G__GSInitTable.main(p1, p2)
    initFuncTable[cGameServer.GetGameServerID()]()
    print("GSInit start...")
end

function __G__GSInitTable.close()
    local gs_event = require("Server/GSEvent")
    print('server close...')
    gs_event.trigger_event(gs_event.BeforeServerClose)
    cGameServer.SetServerStop()
end

return __G__GSInitTable