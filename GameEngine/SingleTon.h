//
// Created by lsjlkz on 2021/12/23.
//

#pragma once
#ifndef MYSERVER_SINGLETON_H
#define MYSERVER_SINGLETON_H

#include <vector>
#include <unordered_map>
#include <map>
#include "iostream"


template<typename T>
class SingleTon {
public:
    static T* m_Instance;

protected:
    SingleTon(){}
    virtual ~SingleTon(){}

public:

    static void Delete(){
        if(m_Instance){
            // 调用子类的析构
            delete m_Instance;
            m_Instance = nullptr;
        }
    }
    static T* Instance(void){
		if(!m_Instance){
			New();
		}
        return m_Instance;
    }

    static void New(){
        m_Instance = new T();
    }
};

template<typename T>
T* SingleTon<T>::m_Instance = nullptr;


#endif //MYSERVER_SINGLETON_H
