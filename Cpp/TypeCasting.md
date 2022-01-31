# C++ 타입 캐스팅 연산자

static_cast, const_cast, reinterpret_cast, dynamic_cast

- static_cast: C스타일 캐스터와 똑같은 의미의 기본적인 캐스트 연산자. 형 변환에 타입체크를 런타임에 하지 않고 컴파일 타임에 정적으로 수행. 논리적으로 변환 가능한 타입을 변환한다.
- const_cast: 상수속성 및 volatile을 제거할 때 사용
- reinterpret_cast: 어떠한 포인터 타입으로 변환 가능. 강제 형변환이므로 강력하지만 위험성이 있음
- dynamic_cast: 런타임에 동적으로 다형성을 이용하여 형변환. 모호한 타입 캐스팅을 시도할 때 NULL을 반환
