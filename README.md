### 필수 command

- [X] PASS : 서버에 연결하기 위한 패스워드 확인
- [X] NICK : 닉네임 설정 / 변경
    - [X] 설정하려는 닉네임 있을 경우 _ 붙이기
- [X] USER : 사용자 정보 설정
- [X] USERHOST : 사용자의 호스트 정보 확인
- [X] JOIN : 채널 입장
	- [X] key, invite only mode, limit 확인하고 입장
- [X] PART : 채널 나가기
- [X] PRIVMSG : 특정 사용자 또는 채널에 메시지를 보내기

운영자와 일반 사용자 존재
- [X] KICK : 특정 채널에서 유저를 내보냄
- [X] INVITE : 특정 채널로 유저 초대
- [X] TOPIC : 특정 채널의 주제 설정
    - MODE가 +t 일때 관리자만 변경 가능
- [X] MODE : 채널의 모드 설정
    - [X] i : 초대 전용 채널 설정/제거
    - [X] t : 운영자가 아닌 일반 유저에 대한 TOPIC 명령 제한 설정/제거
      - +t : TOPIC 설정 불가
    - [X] k (password): 채널키(비밀번호) 설정/제거
    - [X] o (nickname): 채널 운영자 권한 부여/받기
    - [X] l (user_limit): 채널에 대한 사용자 제한을 설정/해제
- [X] QUIT : IRC 서버에서 나가기

--------------------------------------------
### 필요한 command
- [X] PING : 클라이언트-서버 간의 연결을 확인

--------------------------------------------
### 추가 command
- [X] WHOIS : 사용자 정보 조회
- [X] NOTICE : 특정 채널 또는 사용자에게 공지를 보내기
    - privmsg와 notice의 차이점?
- [X] LIST : 현재 서버에서 사용 가능한 채널 목록을 조회
    - [X] : LIST -yes : 전체 채널 목록 조회
    - [X] : LIST #channel : 특정 채널 정보 조회(#필수)


# command 분류
## Connection Message
- nick
- pass
- pong
- quit
- user

## Channel Operator
- invite
- join
- kick
- list
- part
- privMsg
- topic
- mode

## Optional Message
- userhost
- whois

## 체크리스트
- user 존재 여부
- user의 채널에 속해있는지 여부
- user가 채널의 처음인지/마지막인지
  - channel이 존재하는지
  - 마지막 user인지
- user가 operator인지

- target의 존재 여부(client/channel)
- target이 채널에 속해있는지 여부
- target이

- 필요한 인자가 모두 들어왔는지
- 별도로 설정되어 있는 모드가 있는지
- 추가/삭제가 올바르게 이루어지는지
