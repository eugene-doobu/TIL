# 델리게이트와 이벤트

## Delegate

메소드에 대한 참조, 변수가 아닌 메소드 자체를 넘기고 싶을 때 델리게이트를 사용한다.

### 델리게이트 체인

델리게이트에 메소드를 등록할 떄 += 연산자 또는 Delegate.Combine()메소드를 이용하여 델리게이트 실행시 실행될 메소드를 등록할 수 있다. 이 때 여러개의 메소드를 동시에 참조할 수 있는데 이를 델리게이트 체인이라고 한다.

델리게이트 체인은 참조된 순서대로 함수들을 호출하며, -= 연산자 또는 Delegate.Remove()메소드를 이용하여 체인을 끊을 수 있다.

## 익명 메소드

델리게이트로 참조할 메소드가 다시 사용할 일이 없다고 판단되면 익명 메소드를 통해 메소드를 연결시켜주는것이 효율적일 수 있다. 익명 메소드는 아래와 같은 형식으로 선언할 수 있다.

```C#
delegate(매개변수){ /*실행코드*/}
()=>{/*실행코드*/}
```

### Action과 Func

델리게이트를 조금 더 간편하게 쓸 수 있는 Action과 Func타입이 있다.

Action: 리턴값이 없는 델리게이트
Func: 리턴값이 있는 델리게이트로, 마지막 매개변수가 리턴타입이다.

## Event

어떤 일이 생겼을 때 이를 알려주는 객체가 필요할 때가 있는데, 이 때 사용하는 것이 이벤트

## 델리게이트와 이벤트 차이

- 델리게이트: 자신이 선언되어 있는 클래스 외부에서 사용 가능. 주로 콜백용도로 쓰임
- 이벤트: 자신이 선언되어 있는 클래스 외부에서 호출 불가능, 객체 내부에서 객체상태변화나 사건의 발생을 알리는 용도로 쓰임