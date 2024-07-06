#ifndef _ALGRAPH_H
#define _ALGRAPH_H
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<assert.h>
#define MAXFILESIZE 50
using namespace std;

struct Time {
    //时间格式: hour:minute,+day
    int hour;
    int minute;
    int day;

    Time (const int h=0, const int min=0, const int d=0) : hour(h), minute(min), day(d) {}
    Time (const Time &obj) : hour(obj.hour), minute(obj.minute), day(obj.day) {}

    void swap (Time &lt, Time &rt) {
        std::swap (lt.day, rt.day);
        std::swap (lt.hour, rt.hour);
        std::swap (lt.minute, rt.minute);
    }

    Time& operator = (Time robj) {
        swap(*this, robj);
        return *this;
    }

    int getTotalMintue () const {
        return this->day * 1440 + this->hour * 60 + this->minute;
    }

};

struct LineNode {  // 线路信息，作为边表的元素
    
    LineNode (const string scn, const string ecn, const Time st, const Time et, const Time spend_t, 
            const float spend_m, const string amt,int k) 
            : start_city_name(scn), end_city_name(ecn), 
            start_time(st), end_time(et), spend_time(spend_t),
             spend_money(spend_m), amount(amt),kind(k) {}

    string start_city_name;
    string end_city_name;
    Time start_time, end_time;
    Time spend_time;
    float spend_money;
    string amount;  // 火车或飞机的班次
    int kind;
};

struct Vnode {  // 顶点表中的头结点，存储始发站的信息

    Vnode (const string scn, const int cid) : start_city_name(scn), city_id(cid) {}
    Vnode (const char* scn, const int cid) : start_city_name(scn), city_id(cid) {}
    Vnode (const string scn) : start_city_name(scn), city_id(-1) {}
    Vnode (const char* scn) : start_city_name(scn), city_id(-1) {}
    
    string start_city_name;  // 地名
    int city_id;  // 城市编号,从0开始，便于匹配

};

struct cmp_vnode {  // 定义从 Vnode 映射 vector<InfoType> 的 map 的关键字比较关系
	bool operator() (const Vnode& x, const Vnode& k) const {
			if(x.start_city_name >= k.start_city_name)
			return false;  //STL源码规定必须这么返回，而不能改成 true
			else
			return true;
	}
};


class ALGraph {
    public:
        ALGraph() {                  //构造函数
          line_num = 0;
          city_num = 0;
          mkind = 1;                  //选择哪种票，1-飞机，2-火车，3-汽车，初始为飞机票
          together = false;           //是否可换乘，初始为否，ui设计时可根据ui按钮的初始
        }
        void changeType(); //更改单一或混合方式选择                    
        bool ifCityExist (const std:: string &city_name);  // 查询城市是否存在
        int searchCityNum (const string &city_name);  // 查询城市编号
        void addCity (const string &city_name);  // 手动添加城市
        void addCityFromFile (const char FILENAME[MAXFILESIZE]);  // 从文件读取以添加城市
        void addLine();  // 手动添加线路
        void addLineFromFile(const char FILENAME[MAXFILESIZE]);  // 从文件读取以添加线路

        // 删除线路，需要手动输入起点与终点
        void delLine(const string &sc, const string &ec, const string &amt);  
        // 删除城市，并删除以该城市为起点的航班和列车信息
        void delCity(const string &city_name);  
        //void updateFile(const char FileName[MAXFILESIZE], const string type);  // 修改后更新文件

        void showAllCity();  // 输出所有城市
        void showAllLine();  // 输出所有线路

        //输出特定要求下的直达路径
        //四种决策方式都在这一个函数里，未来可以做是否换乘那个复选框的槽函数
        void printstraightPath(const std::string sc, const std::string ec);
        
        // 返回从起点城市到终点城市的所有路径
        std::vector<std::vector<LineNode>> getPathsByCity (const string &sc, const string &ec);

        // 打印从起点城市到终点城市的所有路径
        void printPathsByCity (const string &sc, const string &ec);
        
        // 输出从起点城市到终点城市，中转次数最少的路径
        void printLeastTransferPath (const string &sc, const string &ec);  
        
        // 输出从起点城市到终点城市，花费最小的线路
        void printLeastMoneyPath (const string &sc, const string &ec);  

        // 输出从起点城市到终点城市，总时间最短的线路
        void printLeastTimePath (const string &sc, const string &ec);
        
    private:
        // 定义从 Vnode 映射 vector<InfoType> 的 map, 关键字为 Vnode 中的 start_city_name，关键字的关系为 cmp_vnode
        map <Vnode, vector<LineNode>, cmp_vnode > m;

        int city_num;
        int line_num;
        int mkind;
        bool together;

        // 通过起点城市、终点城市、班次，查询一条线路信息
        vector<LineNode> getLineNode (const string sc, const string ec, const string amt);

};
#endif
