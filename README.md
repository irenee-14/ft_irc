# IRC(Internet Relay Chat) server

### IRC(Internet Relay Chat) server made in C++ 98

인터넷 릴레이 챗(Internet Relay Chat, IRC)은 실시간 채팅 프로토콜입니다. <br />
상용 클라이언트인 `irssi`를 통해 접속이 가능합니다.
I/O multiplexing을 이용하여 여러 클라이언트의 동시 접속을 지원하며, 클라이언트가 보낸 메시지를 파싱하여 명령을 수행합니다.

<br />

## Run

```bash
make
./ircserv <port> <server_password>
```
<img width="400" alt="image" src="https://github.com/irenee-14/ft_irc/assets/74870834/c8669bc9-d9fd-43bd-b4d8-22125a9d0ccc">

<br />

## Connect
1. 상용 클라이언트 irssi 이용
    
    ```bash
    /connect <server_name> <port> <server_password>
    ```
    <img width="320" alt="image" src="https://github.com/irenee-14/ft_irc/assets/74870834/744ed8bf-bf87-4360-8622-e615ed774ba7">

    <img width="700" alt="image" src="https://github.com/irenee-14/ft_irc/assets/74870834/34d5dfb0-43f5-4512-94f9-23f4e914e76d">


1. `nc` 명령어 이용
    
    ```bash
    nc <server_name> <port>
    ```
    
   포트와 연결 후 irssi가 보내는 정보와 동일한 명령어를 입력하여 접속
        
   ```bash
   CAP END
   PASS <server_password>
   NICK <nickname>
   USER <username> <username> <server_name> :<real_name>
   ```
   <img width="700" alt="image" src="https://github.com/irenee-14/ft_irc/assets/74870834/9a0bad87-3654-4363-8a05-7dc82a37a64e">


<br />

## Commands
### Connection Message

| Command | Usage | Description |
|--------|------|--------|
| NICK   | `/NICK [new_nickname]` | 닉네임 설정 / 변경 |
| PASS   | `/PASS <server_password>` | 서버에 연결하기 위한 패스워드 확인 |
| PONG   | `/PONG <daemon>` | 클라이언트-서버 간의 연결을 확인 |
| QUIT   | `/QUIT [<message>]` | 연결 종료 |
| USER   | `/USER <username> <hostname> <servername> <realname>` | 사용자 정보 설정 |

### Channel Operator

| Command | Usage | Description |
|--------|------|--------|
| INVITE | `/INVITE <nickname> <channel>` | 특정 채널로 유저 초대 |
| JOIN   | `/JOIN <channel>{,<channel>} [<key>{,<key>}]` | 채널 입장 |
| KICK   | `/KICK <channel> <user> [<comment>]` | 특정 채널에서 유저를 내보냄 |
| LIST   | `/LIST [<channel>{,<channel>} [<server>]` | 현재 서버에서 사용 가능한 채널 목록을 조회 |
|        | LIST -yes | 전체 채널 목록 조회 |
|        | LIST #channel | 특정 채널 정보 조회 |
| PART   | `/PART <channel>{,<channel>}` | 채널 나가기 |
| PRIVMSG| `/PRIVMSG <receiver>{,<receiver>} <text to be sent>` | 특정 사용자 또는 채널에 메시지를 보내기 |
| NOTICE | `/NOTICE <receiver>{,<receiver>} <text to be sent>` | 특정 채널 또는 사용자에게 공지를 보내기 |
| TOPIC  | `/TOPIC <channel> [<topic>]` | 특정 채널의 주제 설정 |
| MODE   | `/MODE <channel> {[+\|-]\|o\|i\|t\|n} [<limit>] [<user>]` | 채널의 모드 설정 |
|        | i | 초대 전용 채널 설정/제거 |
|        | t | 일반 유저의 TOPIC 명령 제한 설정/제거 |
|        | k <password> | 채널키(비밀번호) 설정/제거 |
|        | o <user> | 채널 운영자 권한 부여/받기 |
|        | l <limit> | 채널에 대한 사용자 제한을 설정/해제 |

### Optional Message

| Command | Usage | Description |
|--------|------|--------|
| USERHOST | `/USERHOST <nickname>` | 사용자의 호스트 정보 확인 |
| WHOIS    | `/WHOIS <nickname>` | 사용자 정보 조회 |
| MOTD     | `/MOTD` | 접속 시 전송되는 메시지, Message of the Day |


<br/>

# Docs

[RFC 1459 (Internet Relay Caht Protocol)](https://datatracker.ietf.org/doc/html/rfc1459)

[RFC 2812 (Client)](https://www.rfc-editor.org/rfc/rfc2812)

[RFC 2813 (Server Protocol)](https://www.rfc-editor.org/rfc/rfc2813)
