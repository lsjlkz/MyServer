//
// Created by lsjlkz on 2022/6/2.
//

#include "GENetWork.h"

GENetWork::GENetWork(int MaxConnect, int Thread) {
	// TODO 网络层研究一下
	std::cout << "GENetWork" << MaxConnect << ";" << Thread << std::endl;
}

int GENetWork::ListenPort(int Port) {
	// 监听端口
	std::cout << "ListenPort" << Port << std::endl;
//    if(this->accpetor == NULL){
//        try{
////            TODO
////            this->accpetor = new tdBoostAcceptor
//        }
//		catch{
//		}
//    }
	return 0;
}

bool GENetWork::isRun() {
	return true;
}
