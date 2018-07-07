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

#ifndef __TC_MYSQL_H
#define __TC_MYSQL_H

#include "mysql.h"
#include "tc_ex.h"
#include <map>
#include <vector>
#include <cstdlib>

namespace tars
{

/////////////////////////////////////////////////
/** 
* @file  tc_mysql.h 
* @brief mysql操作类. 
* 
*/           
/////////////////////////////////////////////////


/**
* @brief 数据库异常类
*/
struct TC_Mysql_Exception : public TC_Exception
{
    TC_Mysql_Exception(const std::string &sBuffer) : TC_Exception(sBuffer){};
    ~TC_Mysql_Exception()noexcept {};    
};

/**
* @brief 数据库配置接口
*/
struct TC_DBConf
{
    /**
    * 主机地址
    */
    std::string _host;

    /**
    * 用户名
    */
    std::string _user;

    /**
    * 密码
    */
    std::string _password;

    /**
    * 数据库
    */
    std::string _database;

    /**
     * 字符集
     */
    std::string _charset;

    /**
    * 端口
    */
    int _port;

    /**
    * 客户端标识
    */
    int _flag;

    /**
    * @brief 构造函数
    */
    TC_DBConf()
        : _port(0)
        , _flag(0)
    {
    }

    /**
    * @brief 读取数据库配置. 
    * 
    * @param mpParam 存放数据库配置的map 
    *        dbhost: 主机地址
    *        dbuser:用户名
    *        dbpass:密码
    *        dbname:数据库名称
    *        dbport:端口
    */
    void loadFromMap(const std::map<std::string, std::string> &mpParam)
    {
        std::map<std::string, std::string> mpTmp = mpParam;

        _host        = mpTmp["dbhost"];
        _user        = mpTmp["dbuser"];
        _password    = mpTmp["dbpass"];
        _database    = mpTmp["dbname"];
        _charset     = mpTmp["charset"];
        _port        = atoi(mpTmp["dbport"].c_str());
        _flag        = 0;

        if(mpTmp["dbport"] == "")
        {
            _port = 3306;
        }
    }

};

/**
* @brief Mysql数据库操作类 
* 
* 非线程安全，通常一个线程一个TC_Mysql对象；
* 
* 对于insert/update可以有更好的函数封装，保证SQL注入；
* 
* TC_Mysql::DB_INT表示组装sql语句时，不加””和转义；
* 
* TC_Mysql::DB_STR表示组装sql语句时，加””并转义；
*/
class TC_Mysql 
{
public:
    /**
    * @brief 构造函数
    */
    TC_Mysql();

    /**
    * @brief 构造函数. 
    *  
    * @param sHost        主机IP
    * @param sUser        用户
    * @param sPasswd      密码
    * @param sDatebase    数据库
    * @param port         端口
    * @param iUnixSocket  socket
    * @param iFlag        客户端标识
    */
    TC_Mysql(const std::string& sHost, const std::string& sUser = "", const std::string& sPasswd = "",
        const std::string& sDatabase = "", const std::string &sCharSet = "", int port = 0, int iFlag = 0);

    /**
    * @brief 构造函数. 
    * @param tcDBConf 数据库配置
    */
    TC_Mysql(const TC_DBConf& tcDBConf);

    /**
    * @brief 析构函数.
    */
    ~TC_Mysql();

    /**
    * @brief 初始化. 
    *  
    * @param sHost        主机IP
    * @param sUser        用户
    * @param sPasswd      密码
    * @param sDatebase    数据库
    * @param port         端口
    * @param iUnixSocket  socket
    * @param iFlag        客户端标识
    * @return 无
    */
    void init(const std::string& sHost, const std::string& sUser  = "", const std::string& sPasswd  = "",
        const std::string& sDatabase = "", const std::string &sCharSet = "", int port = 0, int iFlag = 0);

    /**
    * @brief 初始化. 
    *  
    * @param tcDBConf 数据库配置
    */
    void init(const TC_DBConf& tcDBConf);

    /**
    * @brief 连接数据库. 
    *  
    * @throws TC_Mysql_Exception
    * @return 无
    */
    void connect();
 
    /**
    * @brief 断开数据库连接. 
    * @return 无
    */
    void disconnect();

    /**
     * @brief 获取数据库变量. 
     * 
     * @return 数据库变量
     */
    std::string getVariables(const std::string &sName);

    /**
    *  @brief 直接获取数据库指针. 
    *  
    * @return MYSQL* 数据库指针
    */
    MYSQL *getMysql();

    /**
    *  @brief 字符转义. 
    *  
    * @param sFrom  源字符串
    * @param sTo    输出字符串
    * @return       输出字符串
    */
    std::string escapeString(const std::string& sFrom);

    /**
    * @brief 更新或者插入数据. 
    *  
    * @param sSql  sql语句
    * @throws      TC_Mysql_Exception
    * @return
    */
    void execute(const std::string& sSql);

    /**
     *  @brief mysql的一条记录
     */
    class MysqlRecord
    {
    public:
        /**
         * @brief 构造函数.
         *  
         * @param record
         */
        MysqlRecord(const std::map<std::string, std::string> &record);

        /**
         * @brief 获取数据，s一般是指数据表的某个字段名 
         * @param s 要获取的字段
         * @return  符合查询条件的记录的s字段名
         */
        const std::string& operator[](const std::string &s);
    protected:
        const std::map<std::string, std::string> &_record;
    };

    /**
     * @brief 查询出来的mysql数据
     */
    class MysqlData
    {
    public:
        /**
         * @brief 所有数据.
         * 
         * @return vector<map<string,string>>&
         */
        std::vector<std::map<std::string, std::string> >& data();

        /**
         * 数据的记录条数
         * 
         * @return size_t
         */
        size_t size();

        /**
         * @brief 获取某一条记录. 
         *  
         * @param i  要获取第几条记录 
         * @return   MysqlRecord类型的数据，可以根据字段获取相关信息，
         */
        MysqlRecord operator[](size_t i);

    protected:
        std::vector<std::map<std::string, std::string> > _data;
    };

    /**
    * @brief Query Record. 
    *  
    * @param sSql sql语句
    * @throws     TC_Mysql_Exception
    * @return     MysqlData类型的数据，可以根据字段获取相关信息
    */
    MysqlData queryRecord(const std::string& sSql);

    /**
     * @brief 定义字段类型， 
     *  DB_INT:数字类型 
     *  DB_STR:字符串类型
     */
    enum FT
    {
        DB_INT,     
        DB_STR,    
    };

     /**
     * 数据记录
     */
    typedef std::map<std::string, std::pair<FT, std::string> > RECORD_DATA;

    /**
    * @brief 更新记录. 
    *  
    * @param sTableName 表名
    * @param mpColumns  列名/值对
    * @param sCondition where子语句,例如:where A = B
    * @throws           TC_Mysql_Exception
    * @return           size_t 影响的行数
    */
    size_t updateRecord(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns, const std::string &sCondition);

    /**
    * @brief 插入记录. 
    *  
    * @param sTableName  表名
    * @param mpColumns  列名/值对
    * @throws           TC_Mysql_Exception
    * @return           size_t 影响的行数
    */
    size_t insertRecord(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns);

    /**
    * @brief 替换记录. 
    *  
    * @param sTableName  表名
    * @param mpColumns   列名/值对
    * @throws            TC_Mysql_Exception
    * @return            size_t 影响的行数
    */
    size_t replaceRecord(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns);

    /**
    * @brief 删除记录.  
    *  
    * @param sTableName   表名
    * @param sCondition   where子语句,例如:where A = B
    * @throws             TC_Mysql_Exception
    * @return             size_t 影响的行数
    */
    size_t deleteRecord(const std::string &sTableName, const std::string &sCondition = "");

    /**
    * @brief 获取Table查询结果的数目. 
    *  
    * @param sTableName 用于查询的表名
    * @param sCondition where子语句,例如:where A = B
    * @throws           TC_Mysql_Exception
    * @return           size_t 查询的记录数目
    */
    size_t getRecordCount(const std::string& sTableName, const std::string &sCondition = "");

    /**
    * @brief 获取Sql返回结果集的个数. 
    *  
    * @param sCondition where子语句,例如:where A = B
    * @throws           TC_Mysql_Exception
    * @return           查询的记录数目
    */
    size_t getSqlCount(const std::string &sCondition = "");

    /**
     * @brief 存在记录. 
     *  
     * @param sql  sql语句
     * @throws     TC_Mysql_Exception
     * @return     操作是否成功
     */
    bool existRecord(const std::string& sql);

    /**
    * @brief 获取字段最大值. 
    *  
    * @param sTableName 用于查询的表名
    * @param sFieldName 用于查询的字段
    * @param sCondition where子语句,例如:where A = B
    * @throws           TC_Mysql_Exception
    * @return           查询的记录数目
    */
    int getMaxValue(const std::string& sTableName, const std::string& sFieldName, const std::string &sCondition = "");

    /**
    * @brief 获取auto_increment最后插入得ID. 
    *  
    * @return ID值
    */
    long lastInsertID();

    /**
    * @brief 构造Insert-SQL语句. 
    *  
    * @param sTableName  表名
    * @param mpColumns  列名/值对
    * @return           string insert-SQL语句
    */
    std::string buildInsertSQL(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns);

    /**
    * @brief 构造Replace-SQL语句. 
    *  
    * @param sTableName  表名
    * @param mpColumns 列名/值对
    * @return           string insert-SQL语句
    */
    std::string buildReplaceSQL(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns);

    /**
    * @brief 构造Update-SQL语句. 
    *  
    * @param sTableName  表名
    * @param mpColumns   列名/值对
    * @param sCondition  where子语句
    * @return            string Update-SQL语句
    */
    std::string buildUpdateSQL(const std::string &sTableName,
        const std::map<std::string, std::pair<FT, std::string> > &mpColumns, const std::string &sCondition);

    /**
     * @brief 获取最后执行的SQL语句.
     * 
     * @return SQL语句
     */
    std::string getLastSQL() { return _sLastSql; }

    /**
     * @brief 获取查询影响数
     * @return int
     */
     size_t getAffectedRows();
protected:
    /**
    * @brief copy contructor，只申明,不定义,保证不被使用 
    */
    TC_Mysql(const TC_Mysql &tcMysql) = delete;

    /**
    * 
    * @brief 只申明,不定义,保证不被使用
    */
    TC_Mysql &operator=(const TC_Mysql &tcMysql) = delete;


private:

    /**
    * 数据库指针
    */
    MYSQL         *_pstMql;

    /**
    * 数据库配置
    */
    TC_DBConf   _dbConf;
    
    /**
    * 是否已经连接
    */
    bool        _bConnected;

    /**
     * 最后执行的sql
     */
    std::string      _sLastSql;
  
};

}
#endif //_TC_MYSQL_H
