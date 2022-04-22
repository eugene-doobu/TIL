## Item #1. 데이터 멤버 대신에 항상 프로퍼티를 사용하라

프로퍼티는 메서드의 형태로 구현되기 때문에 멀티쓰레드 지원기능 또한 손쉽게 추가될 수 있다.

- 동기적 접근을 위한 lock
- virtual이나 abstract, interface가 될 수 있음
- 일반적으로 interface는 const가 될 수 없으나, interface와 프로퍼티를 결합하여 const와 동일한 동작을 하도록 사용할 수 있다.

### 인덱서

- 클래스를 배열처럼 사용할 수 있도록, 클래스 내의 배열 데이터에 배열 연산자 []를 사용해 접근 가능하게 해 주는 프로퍼티와 유사한 기능

```csharp
class MyArray
{
	public int this [int index]
	{
		get
		{
			return _theValues[index];
		}
		set
		{
			theValues[index] = value;
		}
	}
}

MyArray myArray = new MyArray();
myArray[0] = 9; // arr[0]에 9 대입
```

- 숫자가 아닌 인덱스를 사용하는 경우 맵이나 디렉토리와 같은 자료구조의 표현도 가능하다
- 일차원 배열이 아닌 다차원 배열 형태로 인덱서를 구성할 수도 있다.

최초 구현시에는 public 데이터 멤버로 선언하고 나중에 프로퍼티의 이점이 필요한 시점에 프로퍼티를 사용호도록 코드를 작성하고 싶을 수도 있다. 하지만 이는 적절하지 못하다.

- 프로퍼티에 접근하는 IL코드는 데이터 멤버에 접근하는 IL코드와 서로 다른 코드를 만들어낸다.

프로퍼티 사용시 수행성능에 큰 영항을 미치지 못한다.

- JIT Compiler는 property accessor와 같은 메서드 호출에 대해 inline을 수행한다.
- 만약 inline을 수행하지 않는다 해도 실질적인 수행성능은 메서드 호출에 대한 비용 정도로 아주 사소해 사실상 측정하기 불가능하다.

## Item #2. const보다는 readonly가 좋다

런타임 상수: readonly

컴파일 상수: const

**컴파일타임 상수**

- 상수형 변수가 사용되는 위치가 실질적인 값으로 치환됨
- 그러므로 내장 자료형(정수형, 실수형)이나 enum, string에 대해서만 사용 가능
- static이므로 항상 동일한 값

**런타임 상수**

- 컴파일타임 상수와 다르게 실질적인 값으로 대체되지 않고 상수에 대한 참조자가 위치하게됨
- 생성자 호출 이후 값을 변경할 수 없지만 생성자 내에서는 값이 할당될 수 있음
- 타입의 인스턴스 별로 서로 다른 값을 가질 수 있음

**Assembly A**

```csharp
public class UsefulValues
{
	public static readonly int StartValue = 5;
	public const int EndValue = 10;
}
```

**Assembly B**

```csharp
for (int i = UsefulValues.StartValue; i < UsefulValues.EndValue; i++)
{
	Console.WriteLine("Value is {0}", i);
}
```

만약 위 Assembly A, B가 포함된 코드에서 A의 StartValue, EndValue를 수정 후 A만 컴파일한다면 B의 EndValue는 컴파일타임 상수라 값이 상수로 고정되어있어 for문이 제대로 동작하지 않을 수 있다.

## Item #3. cast보다는 is나 as가 좋다

- 형변환 가능성 여부는 is로, 형변환은 as로 해주는 것이 좋다
- as연산자가 C언어의 blinding casting보다 좀 더 안정적이며 성능이 좋다.
- as와 is 연산자는 런타임에 정확한 타입의 변환이 가능할 때에만 형변환을 수행한다.(같은 타입이거나 상속한 타입이 아니라면 컴파일에러)
- as연산자로 형변환시 try/catch문을 사용하지 않아도 된다.
- 단, as연산자는 value타입에 대해서는 동작하지 않는다. (value는 null이 될 수 없기 때문)

as연산자와 cast 연산자의 가장 큰 차이점은 사용자가 정의한 형변환 연산자를 어떻게 다루는가 하는 것이다. as나 is 연산자는 런타임에 객체의 형변환이 가능한지 확인하기 위해서 사용자가 정의한 형변환 연산자를 전혀 고려하지 않는다. → 모호함 없이 일관된 동작을 보장한다.

정확한 타입비교를 위해서는 GetType()을 이용하자. 이는 as보다 매우 엄격한 비교를 수행한다.

### Item #4. #if 대신 Conditional Attribute를 사용하라

#if/#endnif는 단일 소스를 이용해 debug버전과 release버전에서 서로 다르게 동작하는 실행파일을 만들거나, 서로 다른 플랫폼에서 수행가능한 실행파일을 만들기 위해 쓰여옴

이를 깔끔하게 처리해주는것이 Conditional Attribute

```csharp
[Conditional("DEBUG")]
private void CheckState(){
	...
}
```

- 만약 Conditional Attribute가 false인 경우 해당 함수 자체가 실행되지 않는다.
- 디버그의 경우 빈 함수가 실행되므로 추가적인 리소스 소모가 있다.
- 메서드 단위로 Attribute를 적용해 구조적으로 프로그램을 만들 수 있도록 강제한다.

### Item #5. 항상 ToString()을 작성하라

항상 ToStirng을 적절히 오버라이딩해서 사용하면 디버깅때나 사용자가 편하게 작업할 수 있다.

- IFormattable 인터페이스를 구현해 다채로운 형태의 결과를 만들어 낼 수 있다.

### Item #6. value타입과 reference타입을 구분하라

value타입인지 reference타입인지, struct나 class 키워드 중 어떤 것을 사용할지는 비교적 쉬운 문제다. 하지만 개발이 많이 진척된 상황에서 타입을 변경하기는 쉽지 않다.

value는 작고 단순한 타입에 좋다

class는 상속이 필요한 경우에 사용되는게 좋다

모든 value는 sealed로 선언하는 것이 좋다

value타입을 사용하는 경우. 아래가 모두 yes일때

1. 타입이 단지 데이터를 저장하기 위해서만 사용되는가?
2. 모든 public interface가 단지 내부적인 값을 획득하거나 수정하기 위해서만 사용되고 있는가?
3. 타입은 앞으로 상속될 가능성이 전혀 없다고 확신할 수 있는가?
4. 타입은 앞으로 다형적으로 다루어져야 할 필요가 없다고 확신할 수 있는가?

### Item #7. immutable atomic value 타입이 더 좋다

immutable이 다양한 환경에서 더욱 안정하다.

immutable 타입의 필드 중 일부가 mutable reference타입일 경우 주의해야 한다.

→ 배열 등을 필드로 갖는 경우 값을 복사해도 저장하여야 한다

```csharp
public struct PhoneList
{
	private readonly Phone[] _phones;

	public PhoneList(Phone[] ph)
	{
		_phones = new Phone[ph.Length];
		ph.CopyTo(_phones, 0)l
	}

	public IEnumerator phones
	{
		get
		{
			return _phones.GetEnumerator();
		}
	}
}

Phone[] phones = new Phone[10];
PhoneList p1 = new PhoneList(phones);

// 전화번호를 담고있는 배열을 변경하더라도
// p1 객체내의 전화번호는 변경되지 않는다.
phones[5] = Phone.GeneratephoneNumber();
```

immutable 타입은 코드를 좀 더 간단하고 유지하기 쉽게 만든다. 타입 내에 아무런 의미 없이 get/set accessor를 만들지 말자.

타입을 생성하는 초기에는 항상 타입이 immutable 타입이 될 수 있을지에 대해 고려해보자.

### Item #8. value 타입을 사용할 때 0이라는 값이 의미를 가질 수 있도록 하라

enum타입의 경우 항상 값을 넣어 초기화를 하도록 하고, 어쩔 수 없는 경우에는 enum의 0값을 None처럼 명시적으로 아무것도 지정되지 않았음을 표시하도록 하게하자.

닷넷 시스템은 value타입의 인스턴스를 항상 0으로 초기화한다. 사용자가 vlaue타입을 활용함에 있어 이러한 초기화 자체를 명시적으로 막을 수 있는 방법은 없다. 따라서 가능한 한 '0'이라는 값을 유효한 값의 범주로 포함시키고, 다른 어떠한 유효한 값도 가지지 않음의 의미를 유지할 수 있도록 하자.

### Item #9. Equals..

새로운 value타입을 만들 때에는 ValueType.Equals()의 구현부를 대체할 수 있는 인스턴스형 Equals()를 재작성하자.

Equals에서 비교할 두 자료형이 상속관계인 경우, 단순히 as 연산자 등으로 자료형을 비교하게 되면 동일관계의 수학적 의미를 위반하게 된다.