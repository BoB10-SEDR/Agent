# 스마트 환경에서의 이상행위와 사이버 공격에 대한 모니터링 및 대응 솔루션 제작 (Agent)

## Agent 
- 언어 : Cpp

### Project Setting
- libprotocol.a 및 protocol.h 정보 갱신
    ```
    $ cd Agent/Inc
    protocol.h 파일과 protocol 디렉토리에 protocol header 파일 및 정보를 갱신한다.

    $ cd Agent/Lib
    libprotocol.a의 최신 라이브러리를 갱신한다.
    ```
- 빌드
    ```
    $ cd Agent
    $ cmake Src
    $ cmake --build .
    ```
- 실행
    ```
    $ cd Agent/Build/LinuxRelease
    $ ./agent
    ```

