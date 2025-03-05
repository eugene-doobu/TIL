3D 게임을 만들다 보면, 언젠가 꼭 마주치게 되는 장면이 있다. “**캐릭터가 울퉁불퉁한 바위 위에 서 있는데, 발이 바위 표면을 뚫고 들어가거나 공중에 둥둥 떠 있는 모습**”을 보게 되는 순간 그래픽의 몰입감이 떨어짐과 함께 '개발자들이 급했나보군'라는 생각이 들게 된다. 이러한 현상은 게임을 하다보면 금방 잊게 되지만, 게임의 완성도를 의심하게 하기도 하고 개발자의 자존심에도 상처를 주기 때문에 오늘은 이와 같은 현상을 방지하는 기능을 구현해보고자 한다.

간단한 레이캐스트와 Unity Animator기본 기능의 조합만으로도 캐릭터의 발이 바닥(또는 지형)에 착 밀착되도록 만들 수 있다. 이러한 기능을 **Feet IK**(Foot IK)라고 정의하고 기능을 구현해보자.

## 1. 이게 왜 문제일까  
---
### 발이 바닥에 묻히거나 떠 있는 상황

![발 파묻힘 장애물](https://velog.velcdn.com/images/eugene-doobu/post/cb350864-e13c-44c9-9201-c3716454e9cc/image.png)|![발 파묻힘 경사](https://velog.velcdn.com/images/eugene-doobu/post/21a13e58-4b49-4e42-8674-7001913ce993/image.png)
---|---|

3D 캐릭터가 디자이너가 만든 예쁜 돌무더기나 지형 위를 걸을 때, 애니메이션은 기본적으로 평지에서 걷는 것만 가정하고 있다. 당연히 경사가 달라지거나 바닥 높이가 변하면, 발바닥이 바위에 반쯤 파묻히거나 뜨게 된다. 아무리 모델이 멋있어도, 발이 어딘가에 박혀 있으면 시각적 몰입감이 크게 줄어들게 된다.

![좌절핑](https://velog.velcdn.com/images/eugene-doobu/post/cb2b4b3c-bac5-45fa-b2b6-51d953e13793/image.png)

 (사실 이런거 신경쓰는 개발자의 기분이 제일 큰 피해를 본다.)
~~일정에 조금만 여유가 있었다면...~~

## 2. Foot IK의 기본 아이디어  
---
### IK란?

>?IK란 역운동학(Inverse Kinematics)이라는 의미로 쉽게 풀어서 설명하자면 `위치`를 통해 `각도`를 구하는 것이라고 생각하면 된다. 

일반적으로 애니메이션은 스켈레톤에 조인트 각도를 미리 정해진 값으로 회전하여 만들어진다. 부모로부터 회전 값이 가중되며 관절의 끝 점의 최종 포지션이 결정되는 형태인데 이를 **순운동학(FK)**이라고 부른다.

이러한 조인트 포즈 작업을 반대 시각에서 바라 보는 것이 유용한 경우도 있다. 예를 들어 아바타가 특정 물체를 잡는 경우 특정 물체의 포지션으로 부터 역으로 각 관절의 회전 값을 계산하여 적용하는 것이다. 이와 같은 방식이 오늘 적용할 **역운동학(IK)**방식이다.

![IK](https://velog.velcdn.com/images/eugene-doobu/post/7c3f1a17-c242-4977-b65e-8fefc752d86b/image.png)

우리는 아바타의 발이 발 아래쪽에 위치한 지면에 딱 붙을 수 있는 위치를 구하고, 양 발의 위치를 기반으로 전체 스켈레톤의 포지션을 조정해주는 IK방식으로 이 문제를 해결해 볼 것이다.

### 레이캐스트 + Animator IK

`Foot IK`는 크게 두 단계로 작동한다.

1. **레이캐스트**: 캐릭터의 발 아래로 레이를 쏴서 “실제 바닥이 어디인지”를 찾습니다.  
   - “아, 여기 돌이 있으니 y좌표가 원래보다 0.3m 높구나!” 식으로 계산.  
   - 이 때 레이캐스트로 검출할 메시 데이터(Collider)와 레이어에 대한 셋팅은 해줘야 한다.
2. **Animator IK**: 유니티에서 지원하는 **`OnAnimatorIK(int layerIndex)`** 메서드로 발(LeftFoot, RightFoot)의 위치·회전값을 직접 설정한다.
   - 이때 Foot IK가 적용되는 모델은 `Humanoid Avatar`형태이여야 한다. 인간형이 아닌 병아리나 강아지와 같은 모델에 해당 기능을 적용할 수 없다. 
   
이렇게 하면 발이 지면을 따라 딱 달라붙는 연출을 그래픽스나 휴머노이드에 대한 지식 없이 쉽게 구현할 수 있다.
     
### 높낮이 & 경사도 보정  

- **경사도 보정**: 기울기를 계산해 발이 기울어지도록 설정해준다. 발의 포지션이 바뀌어도 회전값이 그대로면 어색한 상황이 지속된다.
- **골반 보정**: 발이 올라갔을 때 캐릭터 전체가 조금씩 올라와 자연스럽게 지면 위에 서 있게 보정해준다.

이 부분이 제대로 구현 되면 언덕, 돌무더기, 경사진 경로 등에서도 발이 그럴싸하게 따라간다.


## 3. 실제 코드 구현
---

이번에는 **Feet IK**를 구현한 실제 코드 예시를 살펴보자. 아래 코드는 크게 두 부분으로 나눌 수 있다.

1. **`SolveFeetPositions()`**: 발이 지면에 닿는 위치(및 회전)를 미리 계산해 두는 과정 
2. **`SetFeetIK()`**: Unity 애니메이션 IK(`OnAnimatorIK`)에서 실제 발 위치/회전을 적용하고, 골반(Pelvis)을 보정해주는 과정

## Unity Event Functions

![Unity Event Call](https://velog.velcdn.com/images/eugene-doobu/post/c646a61a-6eb0-4391-b988-464c28d344b7/image.png)

유니티의 이벤트 메서드들은 일정한 주기로 실행된다. 우리는 아바타의 애니메이션 계산이 종료되고 렌더링 하기 전에 발의 위치를 이동시키는 일종의 **'후처리 애니메이션'**을 진행 할 것이다. 전체적인 렌더링 순서는 아래와 같이 진행된다.

1. `Update` 메서드에서 사용자의 입력 값에 따라 캐릭터의 위치값 변경과 애니메이션 파라메터에 값 전달
1. `ProcessAnimation`에서 유니티 애니메이터에 지정된 내용대로 애니메이션 수행
1. 애니메이션이 수행 된 이후 캐릭터 파츠의 좌표 값을 이용하여 `OnAnimatorIK` 메서드에서 위치값 조정
1. `WriteTransform`에서 `OnAnimatorIK`에서 지정해 준 값대로 트랜스폼 적용
1. 아바타 렌더링

<br>

### 3.1. SolveFeetPositions

```C#
        private FootIkSolverData FeetPositionSolver(Vector3 fromSkyPosition)
        {
            if (!Physics.Raycast(fromSkyPosition, Vector3.down, out var feetOutHit,
                                 raycastDownDistance + heightFromGroundRaycast, environmentLayer))
            {
            	// return Default Value
            	...
            }

            var feetIkPositions = fromSkyPosition;
            feetIkPositions.y = feetOutHit.point.y;
            var feetIkRotations = Quaternion.FromToRotation(Vector3.up, feetOutHit.normal) * transform.rotation;

            return new FootIkSolverData
            {
                IsDetectGround = true,
                FootPosition   = feetIkPositions,
                FootRotation   = feetIkRotations
            };
        }
```

IK를 적용시키기 전 미리 발의 포지션과 로테이션 값을 계산해준다.

`fromSkyPosition`은 실제 발의 포지션에 up방향으로 `heightFromGroundRaycast`을 곱해 더해준 값이며, 이 위치에서 down방향으로 레이캐스트를 발사한다. 이 과정에서 검출되는 메시가 없다면 실패했다고 가정하고 default value를 리턴하며, 성공하면 적당한 Position과 Rotation값을 셋팅해준다.

- `FootPosition`: 기존 발의 포지션에서 y값만 실제 바닥에 해당하는(raycast가 충돌한) 값으로 셋팅해준다.
- `FootRotation`: 발의 각도를 현재 경사에 딱 맞는 각도로 변환시킨다. 앞의 `FromToRotation()`은 up벡터를 지면의 normal벡터로 회전시킬 수 있는 회전 값을 계산해 주는 함수이다.

<br>

### 3.2. MovePelvisHeight

```C#
            var transformPositionY = transform.position.y;
            var lOffsetPosition    = _leftFootSolverData.FootPosition.y - transformPositionY;
            var rOffsetPosition    = _rightFootSolverData.FootPosition.y - transformPositionY;

            var totalOffset = lOffsetPosition < rOffsetPosition ? lOffsetPosition : rOffsetPosition;
            var newPelvisPosition = ObjAnimator.bodyPosition + Vector3.up * totalOffset;

            newPelvisPosition.y = Mathf.Lerp(_lastPelvisPositionY, newPelvisPosition.y, pelvisUpAndDownSpeed);

            ObjAnimator.bodyPosition = newPelvisPosition;
            _lastPelvisPositionY = ObjAnimator.bodyPosition.y;
```

현재 양 발의 위치가 원래의 오브젝트 포지션(발 끝의 포지션)에서부터 얼마나 y방향으로 멀어졌는지 체크하여 골반을 보정해주는 함수이다. 우리의 아바타는 밀.집모자 루피처럼 발이 늘어나지 않기 때문에 양 발의 위치가 변함에 따라 전체적인 스켈레톤의 위치가 이동해줘야 할 필요가 있다.

안타깝게도 유니티 Animator 시스템에서는 이와 같은 스마트한 기능이 구현되어 있지 않기 때문에 우리가 알아서 전체 바디 포지션의 위치를 조절해줘야 한다. 이는 신체의 중심(bodyPosition)인 골반(Pelvis)만 조정해주면 손쉽게 구현이 가능하다.


#### 골반보정 전/후

![골반보정 전](https://velog.velcdn.com/images/eugene-doobu/post/341fd3b9-472b-476f-9368-2d7f85b5ad1e/image.png)(골반 보정 전)|![골반보정 후](https://velog.velcdn.com/images/eugene-doobu/post/798c461c-2eea-4608-8187-24d55a41f633/image.png)(골반 보정 후)
---|---|

골반 보정 전 후를 비교해보면 그 차이를 쉽게 알아볼 수 있을 것이다. 골반 보정 전에는 기존 BodyPosition에서 발만 아래로 뻗고 제대로 바닥과 발이 밀착되지 못한 모습을 볼 수 있는데, 골반 보정 후에는 전체적인 스켈레톤 포지션이 내려옴과 동시에 발이 바닥에 닿고(희미한 그림자 참고) 무릎도 이쁘게 굽히고 있는걸 볼 수 있다.

골반 위치를 조정해주는 부분에 디버깅을 걸어보면 전체적인 바디포지션이 `-0.26` 정도 낮아지는 것을 확인할 수 있다.

![골반보정 오프셋](https://velog.velcdn.com/images/eugene-doobu/post/3b870dae-0346-411a-aadb-e955ab3cb248/image.png)


<center>
	<img src ="https://velog.velcdn.com/images/eugene-doobu/post/521fa4d8-fabc-4b7b-be1b-4a5bbaf79a0b/image.png" width="50%"> 
</center>

~~킹든갓택2와 같은 게임만이 고무고무 능력을 제어할 수 있다.~~

<br>

### 3.3 MoveFeetToIkPoint

이제 위에서 계산된 값을 바탕으로 IK를 적용해보자.

```C#
        private void MoveFeetToIkPoint(AvatarIKGoal foot, FootIkSolverData solverData, ref float lastFootPositionY)
        {
            if (!solverData.IsDetectGround) return;
            var positionIkHolder = solverData.FootPosition;
            var rotationIkHolder = solverData.FootRotation;
            var targetIkPosition = ObjAnimator.GetIKPosition(foot);

            targetIkPosition = transform.InverseTransformPoint(targetIkPosition);
            positionIkHolder = transform.InverseTransformPoint(positionIkHolder);

            var yVariable = Mathf.Lerp(lastFootPositionY, positionIkHolder.y, feetToIkPositionSpeed);
            targetIkPosition.y += yVariable;
            lastFootPositionY = yVariable;
            targetIkPosition = transform.TransformPoint(targetIkPosition);

            ObjAnimator.SetIKRotation(foot, rotationIkHolder);
            ObjAnimator.SetIKPosition(foot, targetIkPosition);
        }
```

1. 먼저 `GetIKPosition` 함수를 통해 현재 프레임에서 애니메이션 된 발의 포지션을 가져온다. 해당 프레임에서 이 함수를 호출하기 전 별도로 IK셋팅을 하지 않았다면 순운동학으로 애니메이션이 계산된 포지션의 값을 리턴한다.
1. 이후 현재의 아바타의 발의 포지션과 기존에 계산된 타겟 IK포지션을 `모델 좌표계`로 변환시킨다.
1. 이전 프레임에서 계산된 보정값과 기존에 계산된 타겟값을 `선형보간`하여 IK포지션이 부드럽게 셋팅될 수 있게 한다.
1. 보정치를 타겟 IK포지션에 더해주고 월드좌표계로 변환 뒤 실제 IK Position에 적용된다.
1. 이후 애니메이션을 보정해주는 작업은 유니티에게 맡기고 구현된 기능을 확인한다.

<br>

#### 보충: 모델 좌표계

게임 세계에는 다양한 좌표계가 존재한다. 그 중 모델 좌표계는 모델의 중심(휴머노이드 아바타의 경우 일반적으로 발끝)을 원점으로 하는 좌표계를 의미한다. 해당 좌표계에서는 모델이 게임에 배치됨에 따라 적용되는 변환값들에 영향을 받지 않고 좌표계산을 진행할 수 있다.

#### 보충: 선형보간

1차원 직선상에서 두 점의 값이 주어졌을 때 그 사이의 값을 추정하기 위해 선형적으로 계산(비례식)하는 방법이다. Lerp함수의 마지막에 넣어주는 값(`feetToIkPositionSpeed`)에 따라 `lastFootPositionY`에 가까운 값을 반환할지, `positionIkHolder.y`에 가까운 값을 반환할지 결정된다. 게임에서는 오브젝트의 부드러운 변환을 보여주기 위해 이러한 보간 함수를 자주 이용한다.

## 4. 최종 적용 영상

---

!youtube[Cn24_EwDsOw?si=YZAnQpuBfTKblLM2]

와... 정말 Chill하다..

### 더 해볼만한 것

사실 위 영상만 봐도 파라메터 셋팅이 제대로 되지 않았다.

- 쉐도우에 대한 처리가 제대로 되지 않아 위치감을 느끼기 어려움
- 교복 캐릭터가 위로 경사로 위로 올라가면서 발 끝이 오브젝트에 묻힘
- 고양이귀 캐릭터가 움직이면서 발이 떨리는 포인트가 있음

#### 파라메터 셋팅

![끼용](https://velog.velcdn.com/images/eugene-doobu/post/e4a4dbbb-056c-4473-b159-fced6f183306/image.png)|![끄앙](https://velog.velcdn.com/images/eugene-doobu/post/cd872be2-a9d9-43a1-808c-ef187f6b315d/image.png)
---|---|

게임에 따라 디테일한 씬의 구성이 다르고 사용하는 모델에 따라서 추가적인 위치 보정이 필요할 수도 있다. 따라서 사용할 아바타 마다 직접 테스트 씬을 돌아다니며 디테일한 파라메터를 조절해 줄 필요가 있다. 위의 아름다운 케이스들은 사용하는 파라메터를 과하게 조정했을때 보이는 현상들이다.

- 추가적인 Pelvis Offset
- Pelvis와 Foot의 포지션 보정때 사용하는 선형보간의 파라메터

보통 회사에서는 어떤 파라메터를 제어할지는 클라이언트(혹은 TA)개발자가 지정하고 이를 기획자가 원하는 씬을 돌아다니며 파라메터를 조절하고 아무리 조절해도 마음에 안드는 경우 클라이언트에게 추가 수정을 요청하는 식으로 업무가 진행된다. 하지만 이러한 업무분담이 안되는 안타까운 상황이라면 클라 개발자가 알잘딱깔센으로 다 해야한다.


#### 특정 상황에서만 IK 적용

![발이 쭉](https://velog.velcdn.com/images/eugene-doobu/post/2930e190-cd9d-4ac3-9ea7-5da1fc9bfdd5/image.png)

특정 상태에서는 발에 땅이 닿으면 어색한 경우도 있다.

점프를 하거나 특정 애니메이션을 수행하는 경우 IK를 지정하기 싫다면 함수 시작부 early return을 하는 부분을 참고하여 추가적인 리턴 조건을 추가하면 될 것이다.
(예를 들어 특정 애니메이션 상태인 경우에만 Set IK로직을 수행하는 방식 등..)


### 보충: Animation Rigging

만약 애니메이션 리깅을 사용한다면, 아래 프로젝트를 참고하면 될 것 같다. 위와 유사한 방식으로 Animation Rigging패키지를 사용하여 IK를 구현하였다.

!youtube[0muAp4M7I5s?si=H1q08ppuzpmtdxbI]

[rob1997/Grounder](https://github.com/rob1997/Grounder)

(Animation Rigging에 대해서는 별도의 글로 다룰 수 있으면 좋겠다)


## 5. 마무리

---

**Foot IK**는:  

- 간단한 레이캐스트 + Unity Animator IK를 통해 
- 캐릭터 발이 지형 높이에 맞춰지도록 하여
- 발이 파묻히거나 공중에 뜨는 어색함을 없애줍니다.

게임의 몰입감은 디테일에서 온다고 생각한다. 발 하나만 제대로 붙여줘도 플레이어가 느끼는 세상의 몰입도가 크게 올라갈 수 있다. 한 발만 제대로 내딛어도 게임은 더욱 게임다워진다. 이러한 테크닉을 하나씩 적용해가며 더욱 사실감 있는 게임개발을 진행해 보자.

[구현 PR](https://github.com/eugene-doobu/Ganshin-Impact/pull/162)

<br>

### 참고
---
[역운동학](https://docs.unity3d.com/kr/2019.4/Manual/InverseKinematics.html)
[execution-order(Unity Event Functions)](https://docs.unity3d.com/6000.0/Documentation/Manual/execution-order.html)
[HumanoidAvatar](https://docs.unity3d.com/Manual/AvatarCreationandSetup.html)
[Build a Foot IK System from Scratch for Unity (C#)](https://www.youtube.com/watch?v=MonxKdgxi2w)
