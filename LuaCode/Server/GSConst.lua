---
--- Created by lsjlkz.
--- DateTime: 2023/7/19 15:32
--- Desc:
---

__G__GSConstTable = __G__GSConstTable or {}

function __G__GSConstTable.connected_callback(uSessionID, sIP, uPort)
    print("connected_callback" .. uSessionID .. sIP .. uPort)
end


function __G__GSConstTable.init()
    -- 设置连接后的回调
    cGameServer.SetConnectedLuaCallback(__G__GSConstTable.connected_callback)
end


return __G__GSConstTable