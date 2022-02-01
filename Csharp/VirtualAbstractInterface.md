# virtual, abstract, interface

## abstract

추상 키워드를 사용하려면 해당 클래스도 반드시 추상 클래스여야함
이 키워드로 표시한 속성이나 메서드는 본문을 정의할 수 없고, 접근제한자는 private이면 안됨

## virtual

정적 클래스를 제외한 모든 클래스에서 사용 가능
이 키워드로 표시한 속성이나 메서드는 본문을 정의할 수 있으며, abstract과 마찬가지로 접근제한자는 private이면 안됨

### C# 기본 virtual 메서드

- Equals
- GetHashCode
- ToString

## override

abstract이나 virtual로 지정된 속성이나 메서드를 재정의하는 함수. 추상클래스의 하의 클래스에서 사용이 가능하다.

## interface

- 메소드, 이벤트, 인덱서, 속성을 쓸 수 있음
- 필드를 포함할 수 없음
- 모든 멤버는 public이 기본
- 몸통이 정의되지 않은 추상적인 멤버를 가짐
- 다른 인터페이스를 상속하거나, 클래스에서 여러 인터페이스를 상속 가능