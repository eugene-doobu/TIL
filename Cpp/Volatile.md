# volatile

컴파일러의 최적화를 막아주는 키워드

- 외부적인 요인으로 그 값이 언제든지 바뀔 수 있음을 뜻함
- volatile로 선언된 변수에 대해서는 최적화를 수행하지 않음
- volatile변수 참조 시 레지스터에 로드된 값을 사용하지 않고 매번 메모리를 참조

## 컴파일러 최적화

같은 메모리에 값을 여러번 쓰는 경우, 컴파일러는 속도를 향상시키기 위해 '가장 마지막에 대입 되는 값'만을 남기고 다른 연산은 제거시킴. 하지만 멀티 쓰레드 프로그래밍이나 MMIO(Memmory-mapped I/O)처럼 메모리 주소에 연결된 하드웨어 레지스터에 값을 쓰는 경우 이런 최적화가 오동작을 일으킴. 이때 volatile 키워드를 사용하면 이러한 컴파일러 최적화를 수행하지 않고 기능을 수행할 수 있음

## volatile을 사용하는 경우

- MMIO
- Interrupt Service Routine
- 멀티 스레드
