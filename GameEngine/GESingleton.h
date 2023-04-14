//
// Created by lsjlkz on 2021/12/23.
//

#pragma once
#ifndef MYSERVER_GESINGLETON_H
#define MYSERVER_GESINGLETON_H


template<typename T>
class GESingleton {
public:
    static T* m_Instance;

protected:
    GESingleton(){}
    virtual ~GESingleton(){}

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
T* GESingleton<T>::m_Instance = nullptr;


#endif //MYSERVER_GESINGLETON_H
