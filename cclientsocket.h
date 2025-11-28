#ifndef CCLIENTSOCKET_H
#define CCLIENTSOCKET_H
#include "QDebug"
#include "WinSock2.h"
#include <vector>
#include <cstring>
#include <QObject>
#pragma comment(lib, "ws2_32.lib")
typedef struct MouseEvent{
    MouseEvent(){
        nAction=0;
        nButton=-1;
        ptXY.x=0;
        ptXY.y=0;
    }
    WORD nAction; //点击 移动 双击
    WORD nButton; //左键 右键 中键
    POINT ptXY;//坐标
}MOUSEV,*PMOUSEV;
class SPackeg
{
public:
    SPackeg():sHead(0),nLength(0),sCmd(0),sSum(0){};
    SPackeg(const SPackeg& pack){
        sHead=pack.sHead;
        nLength=pack.nLength;
        sCmd=pack.sCmd;
        strData=pack.strData;
        sSum=pack.sSum;
    }
    //打包
    SPackeg(WORD nCmd,const BYTE* data,size_t nSize) //nSize=data长度 head+2 length+4 cmd+2 sum+2
    {
        qDebug()<<"打包";
        sHead=0XFEFF;
        nLength=nSize+10;
        sCmd=nCmd;
        if(nLength>0)
        {
            qDebug()<<"nsize>0";
            strData.resize(nSize);
            memcpy(strData.data(),data,nSize);
            //对象内存地址虽然连续 但是因为对齐问题 对象数据之间需要填充
            //打包成连续的内容
            this->packetBuffer.resize(nLength);
            char* cursor = this->packetBuffer.data();
            memcpy(cursor, &sHead, sizeof(WORD));
            cursor += sizeof(sHead);
            memcpy(cursor, &nLength, sizeof(DWORD));
            cursor += sizeof(nLength);
            memcpy(cursor, &sCmd, sizeof(WORD));
            cursor += sizeof(sCmd);
            memcpy(cursor, strData.data(), strData.size());
            cursor += strData.size();
            memcpy(cursor, &sSum, sizeof(WORD));
            //qDebug()<<"packet:"<<packetBuffer.data()[6];
        }
        else{
            strData.clear();
        }
        sSum=0;
        for (size_t j = 0; j < strData.size(); j++) {
            sSum += (unsigned char)strData[j];
        }
    }
    //解包
    SPackeg(const BYTE* pData,size_t& nSize){
        const BYTE* cursor=pData;
        size_t i=0;
        WORD temp_head;
        for (; i < nSize-sizeof(DWORD); i++) { // 循环直到找到包头
            // 使用 memcpy 安全地读取潜在的包头
            memcpy(&temp_head, pData + i, sizeof(WORD));
            if (temp_head == 0XFEFF) {
                // 包头找到，游标移动到包头后的位置
                cursor = pData + i + sizeof(WORD);
                break;
            }
        }
        //cursor=pData+i;//游标走到包头结束处
        //读取包中长度信息
        memcpy(&nLength,cursor,sizeof(DWORD));
        cursor+=sizeof(DWORD);
        if (i + nLength > nSize) return;
        memcpy(&sCmd,cursor,sizeof(WORD));
        cursor+=sizeof(WORD);
        int payload_size = nLength - (sizeof(WORD) + sizeof(DWORD) + sizeof(WORD) + sizeof(WORD)); // 总长度 - 10B 固定头部
        if (payload_size < 0) return; // 载荷长度非法
        strData.resize(payload_size);
        memcpy(&strData,cursor,payload_size);
        cursor+=sizeof(strData);
        memcpy(&sSum, cursor, sizeof(WORD));
        cursor += sizeof(WORD);
        WORD calculated_sum = 0;
        // 5. 计算校验和：
        for (size_t j = 0; j < strData.size(); j++) {
            // 使用 unsigned char 确保求和是基于 0-255 的值
            calculated_sum += (unsigned char)strData[j];
        }
        // 6. 校验结果
        if (calculated_sum != sSum) {
            qDebug() << "校验和不匹配，数据包可能已损坏！";
            return; // 校验失败
        }
        nSize = cursor - pData;
    }
    ~SPackeg(){

    };
    SPackeg& operator=(const SPackeg& pack){
        if(this != &pack)
        {
            sHead=pack.sHead;
            nLength=pack.nLength;
            sCmd=pack.sCmd;
            strData=pack.strData;
            sSum=pack.sSum;
        }
        return *this;
    }

    const std::vector<char>& getPacketBuffer() const
    {
        return packetBuffer;
    }

public:
    WORD sHead;//固定0XFEFF 2
    DWORD nLength;//包长度 4 long=4?
    WORD sCmd;//控制命令 2
    std::vector<char> strData;//包数据
    WORD sSum;//和校验 2
    std::vector<char> packetBuffer{};
    //包头固定长度 10+data；
    //解析过程 先提取包头 符合就继续解包
};
//TODO:搭建网络框架，单例模式，智能指针
class Cclientsocket :public QObject
{
    Q_OBJECT // 必须有，才能使用信号/槽
public:
    static Cclientsocket& GetInstance() {
        static Cclientsocket instance;
        return instance;
    }
    bool StartSocket(const std::string& ip);
    bool Send(const char* pData,int nSIze){return 1;};
    bool Send(const SPackeg& pack);
    bool GetFilePath(std::string& strPath);
    bool GetMouseEvent(MouseEvent& mouse);
    SOCKET AcceptClient();
    bool DealCommand(){
        if(m_sock==-1)return false;
        char* buffer=new char[4096];
        memset(buffer,0,4096);
        size_t index=0;
        while (TRUE) {
            //收到包
            size_t len =recv(m_sock,buffer+index,4096-index,0);
            if(len<=0)
            {
                return -1;
            }
            //创建包对象
            index +=len;
            len=index;
            m_packet=SPackeg((BYTE*)buffer,len);
            if(len>0)
            {
                memmove(buffer,buffer+len,4096-len);
                index-=len;
                return m_packet.sCmd;
            }
        }
    }
private:
    //SOCKET m_client = INVALID_SOCKET;
    SOCKET m_sock = INVALID_SOCKET;
    SPackeg m_packet;
    bool SetupSocket(int port,const std::string& strIPAddress);
    bool InitWSA(){
        WSADATA data{};
        int result =WSAStartup(MAKEWORD(1,1),&data);
        if(result!=0){
            qDebug()<< "WSAStartup failed with error: " << result;
            return FALSE;
        };
        return true;
    };

    Cclientsocket(){
        if(!InitWSA())
        {
            qDebug()<<"初始化网络环境错误！";
            exit(0);
        }
    };
    ~Cclientsocket()
    {
        qDebug()<<"析构！";
        if (m_sock != INVALID_SOCKET) closesocket(m_sock);
        WSACleanup();
    };
    //todo:禁用拷贝赋值
    Cclientsocket(const Cclientsocket&) = delete;
    Cclientsocket& operator=(const Cclientsocket&) = delete;


public slots:
    void startConnectionSlot() {
        qDebug()<<"startConnectionSlot";
        int ret=StartSocket("127.0.0.1"); // 假设IP地址是固定的
        if(ret==-1)
        {
            qDebug()<<"StartSocket false!";
            return ;
        }
        SPackeg pack(12138,NULL,0);
        this->Send(pack);
    }

signals:
    void lockCommandReceived();
};



#endif // CCLIENTSOCKET_H
