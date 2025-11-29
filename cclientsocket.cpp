#include "cclientsocket.h"
bool Cclientsocket::SetupSocket(QString Ip,QString Port) {
    // 1. 创建 Socket
    m_sock = socket(AF_INET, SOCK_STREAM, 0); // PF_INET 等同于 AF_INET，通常用 AF_INET

    if(m_sock == INVALID_SOCKET) {
        qDebug() << "Socket creation failed: " << WSAGetLastError();
        return false;
    }
    // 2. 绑定地址
    sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(Ip.toStdString().c_str()); // 监听所有网卡
    serv_adr.sin_port = htons(Port.toInt());

    int ret=::connect(m_sock,(sockaddr*)&serv_adr,sizeof(serv_adr));//符号冲突 ：：选择c函数

    if(ret==-1)
    {
        qDebug()<<"连接服务器失败！";
        return false;
    }

    qDebug() << "Server start on port "<<m_sock;
    return true;
}
bool Cclientsocket::StartSocket(QString Ip,QString Port) {
    return SetupSocket(Ip,Port);
}

bool Cclientsocket::CloseSocket()
{
    ::closesocket(m_sock);
    return true;
}





bool Cclientsocket::GetFilePath(std::string& strPath){
    if(m_packet.sCmd>=2&&m_packet.sCmd<=4)
    {
        strPath=m_packet.strData.data();
        return TRUE;
    }
    return false;
}

bool Cclientsocket::Send(const SPackeg& pack) {
    qDebug()<<"sending!";

    // 假设 SPackeg 类有一个 getPacketBuffer() 方法，返回 const std::vector<char>&
    const std::vector<char>& buffer = pack.getPacketBuffer();

    // 1. 获取完整数据的指针
    const char* pData = buffer.data();

    // 2. 获取准确的长度
    size_t totalSize = buffer.size(); // 或者使用 pack.nLength，但 buffer.size() 更可靠

    // 3. 调用 send，并检查是否所有字节都发送成功
    int bytes_sent = ::send(m_sock, pData, totalSize, 0);

    if (bytes_sent == SOCKET_ERROR) {
        // 处理错误
        qDebug() << "Send failed:" << WSAGetLastError();
        return false;
    }
    qDebug()<<"sended!";
    // 检查是否所有字节都发送出去了 (TCP send 不保证一次发完)
    return (size_t)bytes_sent == totalSize;
}




bool Cclientsocket::SnedCmd(int Cmd)
{
    SPackeg pack(Cmd,NULL,0);
    this->Send(pack);
    return true;
}
