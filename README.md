### 필수 command

- [X] PASS : 서버에 연결하기 위한 패스워드 확인
- [X] NICK : 닉네임 설정 / 변경
  - [ ] 설정하려는 닉네임 있을 경우 _ 붙이기
- [X] USER : 사용자 정보 설정
- [X] USERHOST : 사용자의 호스트 정보 확인
- [X] JOIN : 채널 입장
- [X] PART : 채널 나가기
- [X] PRIVMSG : 특정 사용자 또는 채널에 메시지를 보내기

운영자와 일반 사용자 존재
- [X] KICK : 특정 채널에서 유저를 내보냄
- [ ] INVITE : 특정 채널로 유저 초대
- [△] TOPIC : 특정 채널의 주제 설정
- [ ] MODE : 채널의 모드 설정
    - [ ] i : 초대 전용 채널 설정/제거
    - [ ] t : 운영자가 아닌 일반 유저에 대한 TOPIC 명령 제한 설정/제거
      - +t : TOPIC 설정 불가
    - [ ] k (password): 채널키(비밀번호) 설정/제거
    - [ ] o (nickname): 채널 운영자 권한 부여/받기
    - [ ] l (user_limit): 채널에 대한 사용자 제한을 설정/해제
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