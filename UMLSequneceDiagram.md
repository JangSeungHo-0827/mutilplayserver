```mermaid
sequenceDiagram
    %% 참여자(객체 또는 시스템) 정의
    actor User as 로컬 사용자
    participant PC as PlayerController (로컬 클라이언트)
    participant Char_Local as MyCharacter (로컬 클라이언트 - Autonomous Proxy)
    participant ServerHost as 게임 서버 (호스트)
    participant Char_Server as MyCharacter (서버 - Authority)
    participant Other_Client as 다른 클라이언트 (원격)
    participant Char_Remote as MyCharacter (원격 클라이언트 - Simulated Proxy)

    %% 1. 로컬 플레이어의 입력 및 캐릭터의 로컬 이동
    User->>PC: 키보드/마우스 입력
    PC->>Char_Local: ProcessPlayerInput(입력값)
    activate Char_Local
    Char_Local->>Char_Local: UpdateMovementAndRotation(입력값)
    note right of Char_Local: 로컬 캐릭터 즉시 이동/회전 (클라이언트 측 예측)
    deactivate Char_Local

    %% 2. 로컬 캐릭터가 주기적으로 또는 변경 시 서버로 Transform 정보 전송 (RPC)
    loop Tick 또는 위치/회전 변경 시
        Char_Local->>Char_Server: Server_UpdateTransform(NewLocation, NewRotation)
        note left of Char_Server: 클라이언트 RPC 호출 수신
        activate Char_Server
    end

    %% 3. 서버에서 RPC 처리 및 캐릭터 상태 업데이트
    Char_Server->>Char_Server: Server_UpdateTransform_Validate(NewLocation, NewRotation)
    alt 유효성 검사 성공 (Validation Success)
        Char_Server->>Char_Server: Server_UpdateTransform_Implementation(NewLocation, NewRotation)
        note right of Char_Server: 서버 측 캐릭터의 ReplicatedTransform 변수 업데이트
    else 유효성 검사 실패 (Validation Fail)
        note right of Char_Server: 클라이언트 요청 거부 또는 수정
    end
    deactivate Char_Server

    %% 4. 서버에서 다른 클라이언트로 상태 복제 (Replication)
    %% (언리얼 엔진의 리플리케이션 시스템이 ReplicatedTransform 변수의 변경을 감지하고 자동으로 처리)
    Char_Server-->>Char_Remote: Replicate_ReplicatedTransform(UpdatedTransform)
    note left of Char_Remote: 복제된 데이터 수신
    activate Char_Remote

    %% 5. 다른 클라이언트에서 캐릭터 상태 업데이트 (복제된 데이터 반영)
    Char_Remote->>Char_Remote: OnRep_ReplicatedTransform() 또는 Tick()에서 보간
    note right of Char_Remote: 화면에 다른 플레이어 캐릭터의<br/>움직임 부드럽게 반영 (보간)
    deactivate Char_Remote
