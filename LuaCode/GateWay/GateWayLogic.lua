---
--- Created by lsjlkz.
--- DateTime: 2023/6/15 16:53
--- Desc:
---

-- GateWay To Logic

__G__GateWayLogicTable = __G__GateWayLogicTable or {
    GatewayLogicSessionID = 0,
}


local cDefine = require("Common/CDefine")
local gsevent = require("Server/GSEvent")
local msg = require("Server/GSMessage")

function __G__GateWayLogicTable.connect_logic(owner, callargv, regparam)
    --TODO localhost
    local uSessionId = cGameServer.Connect("localhost", cDefine.Port_Logic, cDefine.EndPoint_GatewayLogic, cDefine.MAX_UINT16, 1024, cDefine.MAX_UINT16, 1024, 0, 0)

    print("connect" .. uSessionId)
    if(uSessionId == cDefine.MAX_UINT32) then
        --继续等待连接
        print("connect_wait")
        --TODO 需要在C++端触发after_load_script，因为可能会因为load_script导致时间延迟，需要更新
        cGameServer.RegTick(10, nil, __G__GateWayLogicTable.connect_logic, nil)
        return false
    end
    print("connected" .. uSessionId)

    __G__GateWayLogicTable.GatewayLogicSessionID = uSessionId

    cGameServer.RegMsgDistribute(msg.TestGatewayLogic, __G__GateWayLogicTable.test_recv_logic)

    __G__GateWayLogicTable.test_send_to_logic()

    -- 设置重定向到逻辑进程的session
    cGameServer.SetClientRedirect(cDefine.RedirectToLogic, uSessionId)
    return true
end

function __G__GateWayLogicTable.test_recv_logic(p1, p2)
    print("test_recv_logic" .. p1 .. p2)

end

function __G__GateWayLogicTable.test_send_to_logic()
    print('test_send_to_logic')
    cGameServer.SendMsg(__G__GateWayLogicTable.GatewayLogicSessionID, msg.TestGatewayLogic, {3, 4, 5})
end

function __G__GateWayLogicTable.after_load_script()
    __G__GateWayLogicTable.connect_logic()
end

function __G__GateWayLogicTable.init()
    GSEvent.RegEvent(gsevent.AfterLoadAllScripts, __G__GateWayLogicTable.after_load_script)
end


return __G__GateWayLogicTable