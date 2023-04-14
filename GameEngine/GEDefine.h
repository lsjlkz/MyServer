//
// Created by lsjlkz on 2022/6/16.
//

#ifndef MYSERVER_GEDEFINE_H
#define MYSERVER_GEDEFINE_H

#include "boost/static_assert.hpp"
#include <cstdlib>

#define GE_STATIC_ASSERT BOOST_STATIC_ASSERT

#ifdef WIN
#define WIN_ASSERT
#endif

#define GE_ASSERT(exp) if (!(exp)){ std::abort(); }

#ifdef WIN_ASSERT
#define GE_WIN_ASSERT(exp) GE_ASSERT(exp)
#else
#define GE_WIN_ASSERT(exp)
#endif

#define GE_DISALLOW_COPY_AND_ASSIGN(className) \
className(const className&) = delete;          \
void operator=(const className&) = delete;\

#define GE_SAFE_DELETE_POINT(head) if((head)){delete (head); (head) = nullptr;}

#define GE_IS_POINT_NULL(head) ((head)==nullptr)

#define GE_IS_POINT_NOT_NULL(head) ((head)!=nullptr)

namespace GE{


#ifdef WIN
	typedef signed char 		Int8;
	typedef signed short		Int16;
	typedef signed int 			Int32;
	typedef signed __int64		Int64;
	typedef unsigned char 		Uint8;
	typedef unsigned short		Uint16;
	typedef unsigned int 		Uint32;
	typedef unsigned __int64	Uint64;
#elif LINUX
	typedef signed char 		Int8;
	typedef signed short		Int16;
	typedef signed int 			Int32;
	typedef signed long long	Int64;
	typedef unsigned char 		Uint8;
	typedef unsigned short		Uint16;
	typedef unsigned int 		Uint32;
	typedef unsigned long long	Uint64;
#endif

	union B2{
	public:
		B2():_i16(0){};
		B2(Uint16 uint16):_ui16(uint16){}
		B2(Uint8 uint8_0, Uint8 uint8_1){
			_ui8[0] = uint8_0;
			_ui8[1] = uint8_1;
		}
		Int8&	I8_0(){return _i8[0];}
		Int8&	I8_1(){return _i8[1];}
		Uint8&	U8_0(){return _ui8[0];}
		Uint8&	U8_1(){return _ui8[1];}
		Int16&	I16(){return _i16;}
		Uint16&	UI16(){return _ui16;}

	private:
		Uint16	_ui16;
		Int16 	_i16;
		Uint8 	_ui8[2];
		Int8 	_i8[2];
	};

	union B4{
	public:
		B4():_i32(0){};
		B4(Uint32 uint32):_ui32(uint32){}
		B4(Uint16 uint16_0, Uint16 uint16_1){
			_i16[0] = uint16_0;
			_i16[1] = uint16_1;
		}
		B4(Uint8 uint8_0, Uint8 uint8_1, Uint8 uint8_2, Uint8 uint8_3){
			_i8[0] = uint8_0;
			_i8[1] = uint8_1;
			_i8[2] = uint8_2;
			_i8[3] = uint8_3;
		}
		Int8&	I8_0(){return _i8[0];}
		Int8&	I8_1(){return _i8[1];}
		Int8&	I8_2(){return _i8[2];}
		Int8&	I8_3(){return _i8[3];}
		Uint8&	U8_0(){return _ui8[0];}
		Uint8&	U8_1(){return _ui8[1];}
		Uint8&	U8_2(){return _ui8[2];}
		Uint8&	U8_3(){return _ui8[3];}
		Int16&	I16_0(){return _i16[0];}
		Int16&	I16_1(){return _i16[1];}
		Uint16&	U16_0(){return _ui16[0];}
		Uint16&	U16_1(){return _ui16[1];}
		Int32&	I32(){return _i32;}
		Uint32&	UI32(){return _ui32;}

	private:
		Uint32	_ui32;
		Int32 	_i32;
		Uint16 	_ui16[2];
		Int16 	_i16[2];
		Uint8 	_ui8[4];
		Int8 	_i8[4];
	};

	union B8{
	public:
		B8 () : _ui64(0) {_ui64 = 0;}
		B8(Int64 i64) : _i64(i64) {}
		B8 (Uint64 ui64) : _ui64(ui64) {}
		B8 (Uint32 b4_0, Uint32 b4_1) {_ui32[0] = b4_0; _ui32[1] = b4_1;}
		B8(Int32 b4_0, Int32 b4_1) { _i32[0] = b4_0; _i32[1] = b4_1; }
		Int8&		I8_0() {return _i8[0];}
		Int8&		I8_1() {return _i8[1];}
		Int8&		I8_2() {return _i8[2];}
		Int8&		I8_3() {return _i8[3];}
		Int8&		I8_4() {return _i8[4];}
		Int8&		I8_5() {return _i8[5];}
		Int8&		I8_6() {return _i8[6];}
		Int8&		I8_7() {return _i8[7];}
		Uint8&		UI8_0() {return _ui8[0];}
		Uint8&		UI8_1() {return _ui8[1];}
		Uint8&		UI8_2() {return _ui8[2];}
		Uint8&		UI8_3() {return _ui8[3];}
		Uint8&		UI8_4() {return _ui8[4];}
		Uint8&		UI8_5() {return _ui8[5];}
		Uint8&		UI8_6() {return _ui8[6];}
		Uint8&		UI8_7() {return _ui8[7];}
		Int16&		I16_0() {return _i16[0];}
		Int16&		I16_1() {return _i16[1];}
		Int16&		I16_2() {return _i16[2];}
		Int16&		I16_3() {return _i16[3];}
		Uint16&		UI16_0() {return _ui16[0];}
		Uint16&		UI16_1() {return _ui16[1];}
		Uint16&		UI16_2() {return _ui16[2];}
		Uint16&		UI16_3() {return _ui16[3];}
		Int32&		I32_0() {return _i32[0];}
		Int32&		I32_1() {return _i32[1];}
		Uint32&		UI32_0() {return _ui32[0];}
		Uint32&		UI32_1() {return _ui32[1];}
		Int64&		I64() {return _i64;}
		Uint64&		UI64() {return _ui64;}
	private:
		Uint64		_ui64;
		Int64		_i64;
		Int8		_i8[8];
		Uint8		_ui8[8];
		Int16		_i16[4];
		Uint16		_ui16[4];
		Int32		_i32[2];
		Uint32		_ui32[2];
	};
}

namespace GEDefine {

// 整数取值范围
#define MIN_INT8				(-127 - 1)
#define	MAX_INT8				127
#define MIN_INT16				(-32767 - 1)
#define MAX_INT16				32767
#define MIN_INT32				(-2147483647 - 1)
#define MAX_INT32				2147483647
#define MIN_INT64				(-9223372036854775807 - 1)
#define MAX_INT64				9223372036854775807
#define MAX_UINT8				255U
#define MAX_UINT16				65535U
#define MAX_UINT32				4294967295U
#define MAX_UINT64				18446744073709551615U


// 连接的默认参数
#define DEFUAL_SEND_BLOCK_SIZE		MAX_UINT16			//发送缓冲区消息块大小
#define DEFUAL_SEND_BLOCK_NUM		100					//发送缓冲区消息块个数
#define DEFUAL_RECV_BLOCK_SIZE		MAX_UINT16			//接收缓冲区消息块大小
#define DEFUAL_RECV_BLOCK_NUM		10					//接收缓冲区消息块个数
#define DEFAUL_WAIT_WHO_TIME		10					//等待身份验证时间
#define DEFAUL_WAIT_RECV_TIME		3600				//等待接收消息时间
// 客户端消息重定向枚举
#define CLIENT_REDIRECT_NONE		0					//无重定向
#define CLIENT_REDIRECT_LOGIC		1					//重定向到逻辑进程
#define CLIENT_REDIRECT_2			2					//占位
#define CLIENT_REDIRECT_3			3					//占位
#define CLIENT_REDIRECT_MAX			4					//最大重定向数量

struct ConnectParam{
	ConnectParam(){
		uSendBlockSize = DEFUAL_SEND_BLOCK_SIZE;
		uSendBlockNum = DEFUAL_SEND_BLOCK_NUM;
		uRecvBlockSize = DEFUAL_RECV_BLOCK_SIZE;
		uRecvBlockNum = DEFUAL_RECV_BLOCK_NUM;
		uWaitWhoTime = DEFAUL_WAIT_WHO_TIME;
		uWaitRecvTime = DEFAUL_WAIT_RECV_TIME;
	}
	GE::Int32		uSendBlockSize;
	GE::Int32		uSendBlockNum;
	GE::Int32		uRecvBlockSize;
	GE::Int32		uRecvBlockNum;
	GE::Uint16		uWaitWhoTime;
	GE::Uint16		uWaitRecvTime;
};
};


#endif //MYSERVER_GEDEFINE_H
