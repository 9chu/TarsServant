/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#ifndef __TARS_SERVANT_HELPER_H
#define __TARS_SERVANT_HELPER_H

#include <iostream>
#include <map>
#include <vector>
#include "../util/tc_autoptr.h"
#include "../util/tc_singleton.h"
#include "Servant.h"

namespace tars
{
//////////////////////////////////////////////////////////////////////////////
/**
 * Servant
 */
class ServantHelperCreation : public TC_HandleBase
{
public:
    virtual ServantPtr create(const std::string &s) = 0;
};
typedef TC_AutoPtr<ServantHelperCreation> ServantHelperCreationPtr;

//////////////////////////////////////////////////////////////////////////////
/**
 * Servant
 */
template<class T>
struct ServantCreation : public ServantHelperCreation
{
    ServantPtr create(const std::string &s) { T *p = new T; p->setName(s); return p; }
};

//////////////////////////////////////////////////////////////////////////////
//
/**
 * Servant管理
 */
class ServantHelperManager : public TC_Singleton<ServantHelperManager>
{
public:
    /**
     * 构造函数
     */
    ServantHelperManager()
    : _isDyeing(false)
    {
    }

    /**
     * 添加Servant
     * @param T
     * @param id
     */
    template<typename T>
    void addServant(const std::string &id,bool check = false)
    {
        if(check && _servant_adapter.end() == _servant_adapter.find(id))
        {
            std::cerr<<"[TARS]ServantHelperManager::addServant "<< id <<" not find adapter.(maybe not conf in the web)"<<std::endl;
            throw std::runtime_error("[TARS]ServantHelperManager::addServant " + id + " not find adapter.(maybe not conf in the web)");
        }
        _servant_creator[id] = new ServantCreation<T>();
    }

    /**
     * 生成Servant
     * @param id
     *
     * @return ServantPtr
     */
    ServantPtr create(const std::string &sAdapter);

    /**
     * 添加Adapter的Servant
     * @param sAdapter
     * @param sServant
     */
    void setAdapterServant(const std::string &sAdapter, const std::string &sServant);

    /**
     * 根据adapter名称获取Servant名称
     * @param sAdapter
     * @return string
     */
    std::string getAdapterServant(const std::string &sAdapter)
    {
        std::map<std::string, std::string>::iterator it = _adapter_servant.find(sAdapter);
        if(it != _adapter_servant.end())
        {
            return it->second;
        }
        return "(NO TARS PROTOCOL)";
    }

    /**
     * 根据servant名称获取adapter名称
     * @param sServant
     * @return string
     */
    std::string getServantAdapter(const std::string& sServant)
    {
        std::map<std::string, std::string>::iterator it = _servant_adapter.find(sServant);
        if(it != _servant_adapter.end())
        {
            return it->second;
        }
        return "";
    }

    /**
     * 获取Adapter/Servant对应表
     * @return map<string, string>
     */
    std::map<std::string, std::string> getAdapterServant() {return _adapter_servant;}

    /**
     * 设置染色信息
     * @param params: notify的输入参数
     * @param sDyeingKey:      用户号码
     * @param sDyeingServant:  对象名称
     * @param sDyeingInterface:接口名称
     * @return string: 设置结果
     */
    bool setDyeing(const std::string & sDyeingKey, const std::string & sDyeingServant, const std::string & sDyeingInterface);

    /**
     * 是否是染色的请求
     * @param sKey:      用户号码
     * @param sServant:  对象名称
     * @param sInterface:接口名称
     * @return string: 设置结果
     */
    bool isDyeingReq(const std::string & sKey, const std::string & sServant, const std::string & sInterface);

    /**
     * 是否是已经被染色
     */
    bool isDyeing() {return _isDyeing;}


protected:

    /**
     * Servant生成类
     */
    std::map<std::string, ServantHelperCreationPtr>   _servant_creator;

    /**
     * Adapter包含的Servant(Adapter名称:servant名称)
     */
    std::map<std::string, std::string>                     _adapter_servant;

    /**
     * Adapter包含的Servant(Servant名称:Adapter名称)
     */
    std::map<std::string, std::string>                     _servant_adapter;

protected:

    /**
     * 锁
     */
    TC_ThreadMutex _mutex;

    /**
     * 是否染色
     */
    bool           _isDyeing;

    /**
     * 染色用户号码
     */
    std::string         _dyeingKey;

    /**
     * 染色的servant对象
     */
    std::string         _dyeingServant;

    /**
     * 染色的接口
     */
    std::string         _dyeingInterface;

};

}

#endif

