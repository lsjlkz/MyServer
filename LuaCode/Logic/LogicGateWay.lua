---
--- Created by lsjlkz.
--- DateTime: 2023/7/19 17:17
--- Desc:
---


__G__LogicGateWayTable = __G__LogicGateWayTable or {

}

local msg = require("Server/GSMessage")


local function test_recv_gateway(p1, p2)
    session_id = p1
    print('test_recv_gateway', p1, p2)
    for i, v in ipairs(p2) do
        print(i .."\t" .. v)
    end
    cGameServer.SendMsg(session_id, msg.TestGatewayLogic, "successfully and back")

end


function __G__LogicGateWayTable.init()
    cGameServer.RegMsgDistribute(msg.TestGatewayLogic, test_recv_gateway)
end

return __G__LogicGateWayTable