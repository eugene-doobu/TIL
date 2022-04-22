C++이나 Java처럼 C#을 사용하면 C#의 고유한 장점을 살릴 수 없을 뿐더러 심각한 문제 상황에 처할 수 있다. 이번 챕터에서는 C#을 C#답게 쓰기 위한 핵심 요소를 살펴볼 것이다.

## 아이템 1: 지역변수를 선언할 때는 var를 사용하는 것이 낫다

- C#언어는 익명타입을 지원하기 위해 타입을 암시적으로 선언할 수 있는 손쉬운 방법을 제공한다.
- 정확한 반환 타입을 알지 못한 채 올바르지 않은 타입을 명시적으로 지정하게 되면 득보다 실이 많다. → IEnumerable과 이를 상속하는 IQueryable 등
- 정확히 기술된 타입 자체보다는 타입을 유추할 수 있는 변수의 이름이 더 큰 도움이 된다.

### var를 사용했는데 더 어지러운 경우

- 메서드 이름만으로 반환 타입을 짐작하기 어려운 경우가 있는데, 이럴때 var의 타입이 무엇인지 알기 어려워진다.
- C#이 제공하는 내장 숫자 타입들은 매우 다양한 형변환 기능을 가지고 있고, 정밀도도 각각 다르기 때문에 이와 var를 함께 사용할 때는 주의해야 한다.

  

 코드를 읽을 때 지역변수의 타입을 명확히 유추할 수 없고 모호함을 불러일으킬 가능성이 있다면 차라리 타입을 명시적으로 선언하는 것이 좋다. 하지만 대부분의 경우에 변수의 이름을 통해서 그 역할을 명확하게 드러내도록 코드를 작성하는 것이 훨씬 낫다. 다만 내장 숫자 타입을 선언할 때는 명시적으로 타입을 선언하는 편이 낫다.

## 아이템 2: const보다는 readonly가 좋다

런타임 상수: readonly

컴파일 상수: const

**컴파일타임 상수**

- 런타임 상수보다 약간 더 빠르지만 유연성이 떨어짐
- 메서드 내부에서도 선언할 수 있음
- 상수형 변수가 사용되는 위치가 실질적인 값으로 치환됨
- 그러므로 내장 자료형(정수형, 실수형)이나 enum, string에 대해서만 사용 가능
- static이므로 항상 동일한 값

**런타임 상수**

- 메서두 내부에서 선언할 수 없음
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

명명된 매개변수(named parameter)나 선택적 매개변수(optional parameter)도 상수의 특성과 연관이 있기 때문에 매우 신중하게 접근해야 한다.

## 아이템 3: 캐스트보다는 is, as가 좋다

- 형변환 가능성 여부는 is로, 형변환은 as로 해주는 것이 좋다
- 컴파일러는 객체가 런타임에 어떤 타입인지를 알 방법이 없다.
- as와 is 연산자는 런타임에 정확한 타입의 변환이 가능할 때에만 형변환을 수행한다.(같은 타입이거나 상속한 타입이 아니라면 컴파일에러)
- as연산자로 형변환시 try/catch문을 사용하지 않아도 된다.
- 단, as연산자는 value타입에 대해서는 동작하지 않는다. (value는 null이 될 수 없기 때문)

as연산자와 cast 연산자의 가장 큰 차이점은 사용자가 정의한 형변환 연산자를 어떻게 다루는가 하는 것이다. as나 is 연산자는 런타임에 객체의 형변환이 가능한지 확인하기 위해서 사용자가 정의한 형변환 연산자를 전혀 고려하지 않는다. → 모호함 없이 일관된 동작을 보장한다.

정확한 타입비교를 위해서는 GetType()을 이용하자. 이는 as보다 매우 엄격한 비교를 수행한다.

## 아이템4: string.Format()을 보간 문자열로 대체하라

C#6.0부터 도입된 문자열 보간 기능을 사용해보자

**장점**

- 코드 가독성 대폭 향상
- 정적 타입 검사 수행 → 개발자 실수 방지
- 기존 방식에 비해 문자열을 생성하기 위한 표현식이 더 풍성

**팁**

- 보간 문자열을 사용하면 컴파일러는 param을 이용하여 object배열을 전달하는 기종 포매팅 함수를 호출하도록 코드를 생성한다. 그런 경우 사전에 문자열로 값을 변경하여 박싱되는 것을 피할 수 있다
- 문자열 보간의 결과는 문자열일 뿐이다. SQL 명령을 만들 때 문자열 보간 기능을 사용하는 것은 그리 추천할 만한 방식이 아니다.

---

## 아이템5: 문화권별로 다른 문자열을 생성하려면 FormattableString을 사용하라

아이템4 에서 다룬 문자열 보간 기능의 결과로 생성되는 반환값은 문자열일 수도 있지만 FormattableString을 상속한 타입일 수도 있다. 다음 코드는 FormattableString 타입의 객체를 이용하여 문화권과 언어를 지정하여 문자열을 생성하는 방법을 나타낸다.

```csharp
public static string ToGerman(FormattableString src) =>
    string.Format(null,
        System.Globalization.CultureInfo.CreateSpecificCulture("de-de"),
        src.Format, src.GetArguments());
public static string ToFrench(FormattableString src) =>
    string.Format(null,
        System.Globalization.CultureInfo.CreateSpecificCulture("fr-CA"),
        src.Format, src.GetArguments());
```

## 아이템6: nameof() 연산자를 적극 활용하라

 이질적인 플랫폼 사이에서 데이터를 주고 받 는 경우, 이름이나 문자열 식별자에 의존하는 간단한 라이브러리가 사용되는 일이 많다. 이 같은 방식이 간편할지는 몰라도 그에 준하는 추가 비용이 발생한다는 것을 잊어서는 안된다. 대표적인 단점이 바로 타입 정보를 손실한다는 것이다.

 C# 언어 설계팀은 이러한 문제를 인지하고 C# 6.0에 nameof()라는 연산자를 추가했다. 이 키워드의 역할은 심볼 그 자체를 해당 심볼을 포함하는 문자열(로컬 네임)로 대체해준다.

```csharp
public string Name
{
    get => name;
    set
    {
        if(value != name)
        {
            name = value;
            PropertyChange?.Invoke(this,
                new PropertyChangedEvent(nameof(Name)));
        }
    }
}

private string name;
```

 nameof() 연산자를 사용했기 때문에 속성의 이름을 변경할 경우 이벤트의 인자로 전달해야 하는 문자열도 쉽게 변경할 수 있다. nameof()는 심볼의 이름을 평가하여 타입, 변수, 인터페이스, 네임스페이스에 대하여 사용 할 수 있다. 다만 제네릭 타입을 사용할 경우 부분적으로 제약이 있어 모든 타입 매개변수를 지정한 닫힌 제너릭 타입만을 사용할 수 있다.

 문자열을 하드코딩하는 대신 nameof 연산자를 이용하면 실수를 줄일 수 있다. 심볼의 이름을 완전히 바꾸거나 수정할 경우에도 손쉽게 그 변경 사항을 반영할 수 있다.

## 아이템7: 델리게이트를 이용하여 콜백을 표현하라

 콜백은 서버가 클라이언트에게 비동기적으로 피드백을 주기 위해서 주로 사용하는 방법이다. 이를 위해 멀티스레딩 기술도 사용되고, 동기적으로 상태를 갱신하는 기법도 활용된다. 콜백은 C#에서 델리게이트를 이용하여 표현된다.

 델리게이트를 활용하면 타입 안정적인 콜백을 정의할 수 있다. 대부분의 경우 event와 함께 사용되지만 반드시 그래야 하는 것은 아니다. .NET Framewordk 라이브러리는 Predicate<T>, Action<>, Function<>와 같은 형태로 자주 사용되는 델리게이트를 정의해 두고 있다.

```csharp
List<int> numbers = Enumerable.Range(1, 200).ToList();

var oddNumbers = numbers.Find(n => n % 2 == 1);
var test = numbers.TrueForAll(n => n < 50);

numbers.RemoveAll(n => n % 2 == 0);
numbers.ForEach(item => Console.WriteLine(item));
```

 LINQ는 이러한 개념을 기반으로 만들어졌다. Find() 메서드는 Predicate<int> 형식의 델리게이트를 사용하여 리스트 내에 포함된 요쇼에 대하여 테스트를 수행한다. TrueForAll() 메서드는 각 요소를 개별적으로 테스트하되 모든 항목이 테스트를 통과한 경우 true를 반환한다.

 멀티캐스트 델리게이트의 경우 두 가지 주의해야 할 부분이 있다. 먼저 예외에 안전하지 않고, 마지막으로 호출된 대상 함수의 반환값이 델리게이트의 반환값으로 간주된다. 델리게이트는 어떤 예외도 잡지(catch) 않으며, 예외가 발생하면 함수 호출 과정이 중단된다. 이러한 문제를 해결하려면 델리게이트에 포함된 호출 대상 콜백 함수를 직접 다뤄야 한다.

```csharp
public void LengthyOperation(Func<bool> pred)
{
    bool bContinue = true;
    foreach(ComplicatedClass cl in container)
    {
        cl.DoLengthyOperation();
        foreach (Func<bool> pr in pred.GetInvocationList())
            bContinue &= pr();

        if (!bContinue)
            return;
    }
}
```

 이와 같이 작성하면 델리게이트에 추가된 개별 메서드가 true를 반환한 경우에만 다음 메서드에 대한 호출을 이어간다.

## 아이템8: 이벤트 호출 시에는 null 조건 연산자를 사용하라

 이벤트 핸들러가 결합되어 있는지 확인하는 코드를 추가하다 이벤트가 있는지 확인하는 코드와 이벤트를 발생시키는 코드 사이에 경쟁 조건(race condition)이 발생할 가능성이 있다. 이 문제는 C# 6.0의 null 조건 연산자를 사용하면 깔끔하게 해결할 수 있다.