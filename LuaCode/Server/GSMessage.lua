---
--- Created by lsjlkz.
--- DateTime: 2022/7/19 17:00
--- Desc:
---消息模块，用于给其他进程或客户端收发消息的

__G__GSMessageTable = __G__GSMessageTable or {}


-- TODO 这个模块将会被干掉，改成字符串hash
-- 从512开始，前面的保留给CMsg
__G__GSMessageTable.TestGatewayLogic = 512
__G__GSMessageTable.TestClientLogic = 513

return __G__GSMessageTable