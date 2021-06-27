#include "Poco/Net/Net.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
 
const Poco::UInt16 PORT = 32452;
 
int main()
{
    Poco::Net::StreamSocket ss;
 
    try
    {
        std::cout << "서버에 연결 시도..." << std::endl;
 
        // 넌블록킹모드로 접속 요청을 하고 바로 다음 루틴으로 넘어간다.
        ss.connect(Poco::Net::SocketAddress("localhost", PORT));
 
        Poco::Timespan timeout(1, 0);    // 1 초
 
        while (ss.poll(timeout, Poco::Net::Socket::SELECT_WRITE) == false)
        {
            std::cout << "서버에 연결 중..." << std::endl;
        }
        std::cout << "서버에 연결 완료" << std::endl;
 
        // 서버에 메시지 보내기
        char szMessage[] = "Send Mesage From Client";
        auto nMsgLen = (int)strnlen(szMessage, 128 - 1);
 
        ss.sendBytes(szMessage, nMsgLen);
    
        // 서버로부터 메시지를 받을 때까지 대기 중..
        Poco::DateTime now1;
        std::cout << "poll 시작: " << now1.second() << std::endl;
 
        while (ss.poll(timeout, Poco::Net::Socket::SELECT_READ) == false)
        {
            std::cout << "receive 대기 중..." << std::endl;
        }
 
        Poco::DateTime now2;
        std::cout << "poll 완료: " << now2.second() << std::endl;
 
        char buffer[256] = { 0 };
        auto len = ss.receiveBytes(buffer, sizeof(buffer));
        std::cout << "서버로부터 받은 메시지: " << buffer << std::endl;
 
        // 접속 종료
        ss.close();
    }
    catch (Poco::Exception& exc)
    {
        std::cout << "서버 접속 실패: " << exc.displayText() << std::endl;
    }
 
    getchar();
    return 0;
}
