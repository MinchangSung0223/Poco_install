
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
 
const Poco::UInt16 PORT = 32452;

int main()
{
    std::cout << "서버 초기화 시작" << std::endl;
 
    // 서버 초기화 설정
    Poco::Net::SocketAddress server_addr(PORT);
    Poco::Net::ServerSocket server_sock(server_addr);
 
    std::cout << "서버 초기화 완료. 클라이언트 접속 대기 중..." << std::endl;
 
    // 접속된 소켓들을 관리할 리스트를 생성한다.
    Poco::Net::Socket::SocketList connectedSockList;
 
    // 서버 소켓을 리스트에 넣어준다.
    connectedSockList.push_back(server_sock);
 
    while (true)
    {
        // 읽기 이벤트를 조사할 소켓 리스트
        Poco::Net::Socket::SocketList readList(connectedSockList.begin(), connectedSockList.end());
 
        // 쓰기가 가능한지 조사할 소켓 리스트
        Poco::Net::Socket::SocketList writeList(connectedSockList.begin(), connectedSockList.end());
 
        // 오류가 발생한 소켓의 리스트
        Poco::Net::Socket::SocketList exceptList(connectedSockList.begin(), connectedSockList.end());
 
        // 타임아웃 정보
        Poco::Timespan timeout(1);
 
        // 다수의 클라이언트를 처리하기 위한 selec모드를 초기화한다.
        auto count = Poco::Net::Socket::select(readList, writeList, exceptList, timeout);
 
        if (count == 0)
            continue;
 
        Poco::Net::Socket::SocketList delSockList;
 
        for (auto& readSock : readList)
        {
            // 서버 소켓이라면 연결 요청을 받은 것이므로 연결을 수락하고
            // 연결된 소켓 리스트에 추가해준다.
            if (server_sock == readSock)
            {
                auto newSock = server_sock.acceptConnection();
                connectedSockList.push_back(newSock);
 
                std::cout << "새 클라이언트에서 접속" << std::endl;
            }
            else
            {
                char buffer[256] = { 0 };
                
                // 클라이언트로부터 받은 메시지를 그래도 클라이언트에게 다시 보내준다.
                auto n = ((Poco::Net::StreamSocket*)&readSock)->receiveBytes(buffer, sizeof(buffer));
                if (n > 0)
                {
                    std::cout << "클라이언트에서 받은 메시지: " << buffer << std::endl;
 
                    char szSendMessage[256] = { 0 };
                    snprintf(szSendMessage, 256 - 1, "Re:%s", buffer);
                    int nMsgLen = strnlen(szSendMessage, 256 - 1);
 
                    ((Poco::Net::StreamSocket*)&readSock)->sendBytes(szSendMessage, nMsgLen);
                }
                else
                {
                    std::cout << "클라이언트와 연결이 끊어졌습니다." << std::endl;
                    delSockList.push_back(readSock);
                }
            }
        }
 
        for (auto& delSock : delSockList)
        {
            // 삭제할 소켓을 검색한다.
            auto delIter = std::find_if(connectedSockList.begin(),
                connectedSockList.end(),
                [&delSock](auto& sock)
                {
                    return delSock == sock ? true : false;
                }
            );
 
            if (delIter != connectedSockList.end())
            {
                connectedSockList.erase(delIter);
                std::cout << "connectedSockList 에서 socket 제거" << std::endl;
            }
        }
    }
 
    getchar();
    return 0;
}
