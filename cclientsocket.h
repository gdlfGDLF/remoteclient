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
        //这里有个问题 我需要手动加一个字符串终止符。
        //读取包中长度信息
        memcpy(&nLength,cursor,sizeof(DWORD));
        cursor+=sizeof(DWORD);
        if (i + nLength > nSize) return;
        memcpy(&sCmd,cursor,sizeof(WORD));
        cursor+=sizeof(WORD);
        int payload_size = nLength - (sizeof(WORD) + sizeof(DWORD) + sizeof(WORD) + sizeof(WORD));
        if (payload_size < 0) { nSize = 0; return; }//验证数据包是否完整
        strData.resize(payload_size+1);
        memcpy(strData.data(), cursor, payload_size);
        strData[payload_size]='\0';
        cursor+=payload_size;
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
    bool StartSocket(QString Ip,QString Port);
    bool Send(const char* pData,int nSIze){return 1;};
    bool Send(const SPackeg& pack);
    bool SnedCmd(int Cmd);
    bool GetFilePath(std::string& strPath);
    bool GetMouseEvent(MouseEvent& mouse);
    bool CloseSocket();
    SPackeg GetPack(){
        return m_packet;
    }
    SOCKET AcceptClient();
  /*  bool DealCommand(){
        if(m_sock==-1)return false;
        qDebug()<<"dealcommand";
        char* buffer=new char[4096];
        memset(buffer,0,4096);
        size_t index=0;
        while (TRUE) {
            //收到包
            size_t len =recv(m_sock,buffer+index,4096-index,0);

            if(len<=0)
            {
                qDebug()<<"DealCommand len<0 return -1";
                return -1;
            }
            //创建包对象
            index +=len;
            len=index;
            m_packet=SPackeg((BYTE*)buffer,len);
            qDebug()<<m_packet.strData;
            if(len>0)
            {
                memmove(buffer,buffer+len,4096-len);
                index-=len;
                return m_packet.sCmd;
            }
        }
   }*/

    int DealCommand() {
        if (m_sock == -1) return false;

        // 【关键修正 1】使用 std::vector (RAII 自动清理)
        std::vector<char> buffer(4096);
        size_t index = 0; // 缓冲区中有效数据的总长度
        while (TRUE) {
            size_t bytes_to_read = buffer.size() - index; // 使用 vector.size() 获取容量
            if (bytes_to_read == 0) {
                qDebug() << "缓冲区已满，强制退出。";
                return false;
            }
            // 1. 接收数据：使用有符号 int 接收返回值，并使用 data() 获取指针
            int signed_len = ::recv(m_sock, buffer.data() + index, bytes_to_read, 0);
            if (signed_len <= 0) {
                // 连接断开 (0) 或错误 (-1)，vector 在函数退出时自动清理
                qDebug() << "Connection closed or recv failed. Error:" << WSAGetLastError();
                return false;
            }
            // 转换为 size_t 并更新总长度
            size_t len = (size_t)signed_len;
            index += len;
            // --- 内部循环：解析累积的数据 ---
            size_t consumed_size = 0;
            while (index >= 10) {
                size_t total_data_size = index;
                // 2. 尝试解析 (注意：m_packet 应该通过构造函数赋值)
                m_packet = SPackeg((BYTE*)buffer.data(), total_data_size); // 修正：使用 data()
                consumed_size = total_data_size;
                if (consumed_size > 0) {
                    // ... (成功解包逻辑)
                    // 3. 清理缓冲区 (memmove 使用 data() 指针)
                    // 必须在 vector 的有效数据指针上操作
                    memmove(buffer.data(), buffer.data() + consumed_size, index - consumed_size);
                    index -= consumed_size;
                    // 退出 DealCommand。vector 会自动析构。
                    return m_packet.sCmd;
                } else {
                    break; // 包不完整，等待更多数据
                }
            }
        }
        // 退出函数时，std::vector<char> buffer 会自动调用析构函数，释放内存。
        return false;
    }

private:
    //SOCKET m_client = INVALID_SOCKET;
    SOCKET m_sock = INVALID_SOCKET;
    SPackeg m_packet;
    bool SetupSocket(QString Ip,QString Port);

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
    void startConnectionSlot(QString nIp,
                             QString nPort,
                             WORD cmd,
                             const QByteArray& payload=QByteArray()) {
        qDebug()<<"startConnectionSlot";
        int ret=StartSocket(nIp,nPort);
        if(ret==-1)
        {
            qDebug()<<"StartSocket false!";
            return ;
        }
        // QByteArray byteArray=fullPath.toUtf8();
         const BYTE* pData = (const BYTE*)payload.constData();
         size_t nSize = payload.size();

        SPackeg pack{cmd,pData,nSize};
        this->Send(pack);
        //this->CloseSocket();
    };
    void GetDirInfo();
signals:
    void lockCommandReceived();
};
#endif // CCLIENTSOCKET_H
