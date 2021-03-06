# Inter-Process Communication(IPC)

프로세스는 독립적으로 실행된다. 이런 독립적 구조를 가진 프로세스 간의 통신을 해야 하는 상황에서 사용하는것이 IPC 통신이다.

프로세스는 커널이 제공하는 IPC 설비를 이용해 프로세스간 통신을 할 수 있다.

이런 IPC 통신에서 프로세스 간 데이터를 동기화하고 보호하기 위해 세마포어와 뮤텍스를 사용한다.

## IPC의 종류

### 익명 PIPE

파이프는 두 개의 프로세스를 연결하는데 하나의 프로세스는 데이터를 쓰기만 하고, 다른 하나는 데이터를 읽기만 할 수 있다.

한쪽 방향으로만 통신이 가능한 반이중 통신이라고도 부른다.

따라서 양쪽으로 모두 송/수신을 하고 싶으면 2개의 파이프를 만들어야 한다.

매우 간단하게 사용할 수 있는 장점이 있고, 단순한 데이터 흐름을 가질 땐 파이프를 사용하는 것이 효율적이다.

### Named PIPE

익명 파이프는 통신할 프로세스를 명확히 알 수 있는 경우에 사용한다.

Named 파이프는 전혀 모르는 상태의 프로세스들 사이 통신에 사용된다.

### Message Queue

메시지 큐는 파이프처럼 데이터의 흐름이 아니라 메모리 공간이다.

사용할 데이터에 번호를 붙이면서 여러 프로세스가 동시에 데이터를 쉽게 다룰 수 있다.

### Shared Memory

공유 메모리는 프로세스간 메모리 영역을 공유해서 사용할 수 있도록 허용해준다. 중개자 없이 곧바로 메모리에 접근할 수 있어서 IPC 중에 가장 빠르게 동작한다.

### Memory Map

공유 메모리처럼 메모리를 공유해주는데, 메모리 맵은 열린 파일을 메모리에 맵핑시켜서 공유하는 방식이다. 주로 대용량 데이터를 공유해야 할 때 사용한다.

### 소켓

네트워크 소켓 통신을 통해 데이터를 공유한다. 클라이언트와 서버가 소켓을 통해서 통신하는 구조로, 원격에서 프로세스 간 데이터를 공유할 떄 사용한다.
