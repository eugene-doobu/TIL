 이 글은 실제 프로젝트에서 사용되는 알고리듬에 대해 간단한 소개를 하기 위한 글로, 해당 알고리듬들에 대한 자세한 설명이 포함되어 있지 않습니다.
 
 해당 글에서 사용한 코드들은 이 [저장소](https://github.com/eugene-doobu/pathfinding-example)에 있습니다.

(알고리즘 -> 알고리듬 캠페인을 실천중입니다.)

## 길찾기란 무엇인가

게임에서 캐릭터가 움직이는건 어떻게 구현할까? 유저가 특정 키를 누르면 캐릭터가 해당 방향으로 갈 수 있는지 판단하고 캐릭터의 위치를 조금 이동시킨다. 근데 이건 '나의 캐릭터'가 움직일때의 이야기이다. 

그럼 플레이어가 조종하지 않는, 예를 들어 몬스터와 같은 오브젝트의 이동은 어떻게 처리할까? 게임회사 직원들이 몬스터를 하나하나 클릭해서 wasd키를 눌러 이동시켜주는건 아니다. 게임회사 직원들의 워라밸을 위해 이걸 자동화해줄 시스템이 필요하다. 이를 위해 필요한 것이 길찾기 알고리듬이다.

<center>
	<img src ="https://velog.velcdn.com/images/eugene-doobu/post/d37d4ada-74f8-4d8b-9316-a5f7236eaaab/image.png" width="70%"> 
</center>


몬스터의 케이스는 아니더라도 길찾기 알고리듬은 게이머에게도 매우 유용하다. 퀘스트를 위해 걸어서 10분정도 걸리는 거리를 계속 키를 누르며 이동해야 한다고 하면 유저들은 매우 피곤해할 것이다. 그냥 적당히 목적지를 정해 길찾기를 시켜두고 커피한잔 사온 다음 느긋하게 월드를 구경하는 여유로운 시간을 갖게 해주기 위해서는 길찾기 알고리듬의 구현이 필요하다.

이번 글에서는 가장 간단한 형태의 길찾기 알고리듬에서 시작하여 실제 프로젝트에 많이 적용해서 사용하는 A* 길찾기까지 발전시키며 왜 이 알고리듬이 실제 프로젝트에 쓰이고 있는지 알아가볼 것이다. 이 글에 나오는 알고리듬의 구현은 Unity 게임엔진에서 C#으로 작성되었다.

## 우수법

미로 찾기 알고리듬 가장 간단한 알고리듬인 우수법에 대해 알아보자. 우수법은 오른손을 벽에 짚고 따라가다보면 결국 언젠가는 길을 찾게된다는 점에서 구현된 알고리듬이다. 이를 알고리듬으로 구현한 후 실행해보면 다음과 같은 결과가 나온다.

![righthandmaze](https://velog.velcdn.com/images/eugene-doobu/post/6f9809f8-d857-4243-aab5-cc727642aebb/image.gif)

(위 움짤은 [C# MMO시리즈](https://www.inflearn.com/course/%EC%9C%A0%EB%8B%88%ED%8B%B0-mmorpg-%EA%B0%9C%EB%B0%9C-part2) 강의에 나온 코드를 실행한 것입니다)

보기에는 굉장히 신기하지만, 실제 게임 캐릭터가 이런방식으로 움직인다고 생각해보자. 딱봐도 아무것도 없을것같은 우측 통로에 캐릭터가 머리를 박고 다시 왔던 길을 되돌아오는 모습을 본다면 속이 터져 길찾기 기능을 봉인해버리고 말것이다.

플레이어의 캐릭터와 몬스터들이 바보로 불리는 일이 일어나기 전에 똑똑해보이는 길찾기 알고리듬을 구현을 해보자. 우리가 실제 플레이하는 게임은 모두 위 예시처럼 네모네모한 미로안에 구성되어 있지 않다. 우리가 개발하고 있는 게임의 길찾기 기능을 구현한다고 생각하고, 길을 찾기 위해서 길이란 무엇인지 정의해보도록 하자.


## '길'을 정의하자
길찾기 알고리듬을 실행시키기 위해서는 '길'이 무엇인지에 대한 정의가 필요하다. 우리가 게임에서 캐릭터를 조작할 수 있는 맵의 유형에 어떤 것들이 있는지 생각해보자

![메](https://velog.velcdn.com/images/eugene-doobu/post/f31eaf7d-7da2-40f3-b4af-f929a5fbafc0/image.png)|![팩맨](https://velog.velcdn.com/images/eugene-doobu/post/dae6c291-ea7a-4c7f-9cbb-940f66fd3966/image.png) | ![와우](https://velog.velcdn.com/images/eugene-doobu/post/8df5c451-d053-4223-bb59-115fbc7cd8dc/image.png)
---|---|---|

첫번째로, 메이플 스토리의 월드맵처럼 특정 구역과 구역들이 연결되어있는 그래프에서의 경로를 생각해볼 수 있다. 두번째로는 팩맨처럼 바둑판과같은 직사각형의 행렬위에서 오브젝트들을 이동시키는 형태를 생각해볼 수 있다. 이 케이스의 경우에도 한개의 요소가 상하좌우의 요소와 연결되어 있는 그래프로도 구현할 수 있지만, 2차원 그리드로 갈 수 있는 길, 갈 수 없는 길, 오브젝트들의 위치를 표현하는것이 더 직관적일 수 있다. 세번째로는 월드 오브 워크래프트와 같은 3D 지형에서의 길이다.

사실 이 세가지 예시 모두 본질적으로 같은 방법으로 구현할 수 있다. 하지만 맵의 형태에 따라 효율적인 방법은 존재한다. 단순히 그래프 알고리듬을 구현하는 경우에도, 그래프의 특징과 표현방식을 어떻게 하느냐에 따라 성능이 달라지기도 한다. 따라서 지금 프로젝트의 맵이 어떻게 기획되었는지를 먼저 생각해보고 기획 구조에 맞는 맵의 표현 방식을 정하고, 그 위에서 길찾기 알고리듬을 구현하고 발전시켜나가야 한다.

이 글에서는 길찾기 알고리듬을 설명하기 위해 가장 구현하기 편한 형태인 2차원 그리드형태의 맵 위에서 길찾기 과정을 시각적으로 표현하는 예제를 구현해볼 것이다.

글쓴이는 길찾기 예제를 구현하기 위해 [Unity](https://unity.com/kr)게임엔진을 사용하였으며, 맵 구현에는 [CatLikeCoding의 튜토리얼](https://catlikecoding.com/unity/tutorials/hex-map/)을 참고하였다. 그리고 길찾기에 사용되는 캐릭터의 리소스는 [NineChronicles](https://github.com/planetarium/NineChronicles)의 스파인 에셋을 사용하였다. 그렇게 준비된 맵의 모양은 다음과 같다.

![MAP](https://velog.velcdn.com/images/eugene-doobu/post/2c5739d8-b676-4ff0-8e00-57ec6484fb37/image.png)

네모네모한 2차원 미로 형태로 맵울 구성 하려다가 그건 심심해보여서 벌집모양의 형태로 조금 변형을 했다. 맵을 이렇게 변경해도 본질적인 길찾기 알고리듬자체는 바뀌지 않는다는것을 보여주고 싶기도 했다.

![Map Color](https://velog.velcdn.com/images/eugene-doobu/post/a761f538-fb92-4898-bedf-455f7a69d310/image.png)

맵에서는 영역의 구분을 위해 총 5가지 컬러를 사용하고 있다. 왼쪽부터 <span style="color:#73E097">컬러0</span>, <span style="color:#49CFBF">컬러1</span>, <span style="color:#4A96CF">컬러2</span>, <span style="color:#4A4ECF">컬러3</span> 그리고 색칠되지 않은 기본 컬러값인 흰색으로 구성되어있다. 길찾기 알고리즘 구현마다 노드의 상태를 표현하기 위해 이 컬러들을 이용할 것이다.


## BFS(Breadth-first search)

코딩테스트를 위해 알고리듬을 공부하는 사람들은 한번 쯤 들어봤을 알고리듬이다. BFS는 '너비 우선 탐색'이라고도 불린다. 이는 **하나의 노드를 방문한 후, 그 노드와 인접한 모든 노드들을 우선적으로 방문하는 방법**이다. 주로 queue를 이용하여 구현하며, 현재 방문한 노드에서 다음에 방문할 수 있는 모든 노드들을 queue에 등록함으로 현재 노드에서 인접한 노드들을 우선적으로 탐색하게 되는 것이다.

여기까지가 일반적으로 알고 있는 '탐색 알고리듬'으로써의 BFS일 것이고, 여기의 조금의 변형만 가해주면 '항상 최단거리를 찾아주는' 길찾기 알고리듬으로 만들어줄 수 있다. 바로 **노드 방문시 자신을 queue에 넣었던 노드의 정보를 기록해줌으로써 특정 노드부터 시작노드까지의 경로를 구할 수 있다.** 이 때, 시작 노드는 자기 자신을 queue에 넣은 노드로 정보를 구성하여 다른 노드들과 구분할 수 있다. 

![bfsmaze](https://velog.velcdn.com/images/eugene-doobu/post/3a82b0c4-31ed-4c8e-afd4-4aa80d7aefb8/image.gif)

(위 움짤은 [C# MMO시리즈](https://www.inflearn.com/course/%EC%9C%A0%EB%8B%88%ED%8B%B0-mmorpg-%EA%B0%9C%EB%B0%9C-part2) 강의에 나온 코드를 실행한 것입니다)

이를 기반으로 아까 우수법에서 탈출했던 미로를 BFS기반 길찾기 알고리듬을 통해 탈출하면 위 움짤과 같은 결과가 나온다. 이렇게 미로를 탈출하는 결과를 보면 정말 군더더기없이 깔끔하게 미로를 탈출하는 것 처럼 보인다. 실제로 BFS알고리듬를 통해 얻은 길찾기 경로는 최단경로임이 보장된다.

<br>

이를 기반으로 우리가 설계했던 맵에서 길찾기를 구현해보자. 아직 탐색하지 않은 경로는 흰색, 이미 지나온 길들은 <span style="color:#73E097">Color0</span>, 도착지점은 <span style="color:#49CFBF">Color1</span>, 도달할 수 없는 경로는 <span style="color:#FF0000">Red</span>색상으로 채워보도록 하겠다

!youtube[qwibqp_-lZo?si=558InPY4B6NT10Fa]

[코드참고](https://github.com/eugene-doobu/pathfinding-example/blob/main/Assets/Project/Scripts/PathFinding/BFSPathFinding.cs)
<br>

BFS기반으로 길찾기를 구현해본 영상이다. 동글이가 통통 뛰어가며 경로를 향해 뛰어가고 이동 경로가 색칠되는것을 확인해볼 수 있다. 마지막 35초쯤 동글이를 벽으로 가둔 후 벽 넘어 길찾기를 시도했을 때 경로 탐색에 실패해 클릭했던 셀이 빨간색으로 변하는것을 확인할 수 있다. 이후 벽을 다른 색상으로 변경 후 다시 해당 지점으로 길찾기를 했을 경우 정상적으로 길을 찾아 갔다.

이렇게 해서 BFS기반으로 길찾기 알고리듬이 구현되었지만, 실제 게임 프로젝트에서 BFS를 길찾기를 위해 사용하는 경우는 많이 없을것이다. 게임에서의 길은 모든 경로가 평등하게 구성된다고 보장되지 않는다. BFS방법에서는 현재 내가 갈 수 있는 길들을 모두 평등하게 큐에 넣고, 순서대로 접근하기 때문에 '가기 어려운 길, 가기 쉬운 길' 이라는 개념을 적용하기가 힘들다. 

실제 게임에서는 지형에 물이 있을 수도 있고, 경사가 있는 길이 있을 수 있다. 전략적 게임에서 이러한 지형적 특성에따라 이동에 필요한 자원이 달라지는 기능은 쉽게 찾아볼 수 있다. 그리고 캐릭터 특성에 따라 누구는 날 수 있고 수영할 수 있고 하는등 이동에 대한 특징들이 다르기도 하다. 만약 '평지에서는 이동당 1, 물로 이동할때는 이동당 2의 스태미너를 소모하게 해주세요'하는 기획을 만난다면 어떻게 할 것인가?

1. 그런 기획은 안된다고 말한다.
1. 경로에 가중치를 두어서 연산하는 알고리듬을 사용한다.

![안돼](https://velog.velcdn.com/images/eugene-doobu/post/beb910b9-90ed-4f4d-b77d-12cecc7b4ac1/image.png)

프로그래머의 구현이 힘들어 기획적인 제한이 생긴다는건 매우 슬픈 일이다. 이러한 상황을 방지하기 위해 가중치가 있는 그래프에서 사용 가능한 경로 탐색 알고리듬인 다익스트라(Dijkstra) 알고리듬에 대해 알아보자. 그리고 BFS를 실전에서 사용하기 힘든 또 다른 이유가 있는데, 이것은 Dijkstra 알고리듬을 설명하면서 같이 다루도록 하겠다.

## Dijkstra

다익스트라 알고리듬은 가중치가 있는 그래프에서 경로를 찾기 위해 사용하는 알고리듬이다. 이에 대한 설명은 다음과 같다.

1. 도달하는데 가중치가 가장 적게 드는 정점을 찾는다
1. 해당 정점에 대해 그 정점을 지나 이웃 정점들까지 이동하는데 드는 가중치를 조사한다.
1. 이 과정을 모든 정점에 대해 반복한다.
1. 최종 경로를 계산한다.

(출처: [다익스트라 알고리즘(Dijkstra algorithm)](https://medium.com/@pyeonjy97/%EB%8B%A4%EC%9D%B5%EC%8A%A4%ED%8A%B8%EB%9D%BC-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-dijkstra-algorithm-5d64a6cffced))

다익스트라에 대한 내용은 알았으니 이를 한번 구현해보자. 이번에는 노드와 노드간의 가중치를 정하기 위해 <span style="color:#73E097">컬러0</span>은 BFS와 동일하게 이동한 경로, <span style="color:#49CFBF">컬러1</span>은 목표지점으로 설정했다. <span style="color:#4A96CF">컬러2</span>은 이동시 필요한 가중치가 2, <span style="color:#4A4ECF">컬러3</span>은 이동시 필요한 가중치가 3으로 설정했다. BFS와 달리 벽과 같은 개념이 없어 결과적으로 모든 노드로 이동이 가능하기에 빨간색 노드는 사용하지 않았다.

<br>

!youtube[mRG3a1NY5C4?si=rAzvt4DnvHgC5PYD]

[코드참고](https://github.com/eugene-doobu/pathfinding-example/blob/main/Assets/Project/Scripts/PathFinding/DijkstraPathFinding.cs)
<br>

이렇게 다익스트라 알고리듬을 사용함으로써 우리가 구현할 수 있는 기획의 범위가 늘어났다. 하지만 이정도로 실제 게임 프로젝트에서 사용하는 길찾기 알고리듬을 구현할 수 있다고 말할 수 있을까? 아쉽지만 이정도로도 충분하지 않다.

BFS는 'Flood Fill'알고리듬에서도 사용되는 기법이다. Flood Fill은 그래프에서 특정 노드와 연결된 모든 노드들을 찾는 알고리듬으로, 그림판에서 색을 채우는 명령을 통해 특정 픽셀과 인접한 모든 픽셀의 색을 바꿔버리는 기능에 사용되기도 한다. 그렇다. BFS를 통해 길찾기를 하게 되면, 해당 지점까지의 경로를 찾기 위해 인접한 모든 노드들을 탐색 해버리게 된다. 이와 같은 현상은 Dijkstra알고리듬 에서도 개선되지 못하였다.

지금 예제처럼 작은 형태의 맵이라면 상관 없겠지만, 초-MMORPG의 초대형 맵에서 이러한 방식으로 길찾기를 해버린다면 어떻게 될까? 클라이언트 입장에서는 길찾기 딸깍 명령 후 엄청난 시간 후에 응답을 받게 될 것이며, 게임회사 입장에서는 막대한 서버연산을 감당하지 못하고 파산해버리고 말 것이다.

![감자](https://velog.velcdn.com/images/eugene-doobu/post/c18e7565-8a4f-42ad-a71c-67442891026c/image.png)


<center>
서버: 크윽.. 이정도 연산량이면 감자가 폭발해버리고말아..!!
</center>

![기다리다지쳐](https://velog.velcdn.com/images/eugene-doobu/post/17a144c1-cb7e-4231-8931-aa9d6522cf6c/image.png)

<center>
유저:
</center>

## A* 알고리듬
모든 경로를 탐색하지 않고 길을 탐색하려면 '휴리스틱'한 방법을 사용할 필요가 있다.

휴리스틱이란 불충분한 시간이나 정보로 합리적인 판단을 할 수 없거나, 체계적이면서 합리적인 판단이 굳이 필요하지 않은 상황에서 사람들이 빠르게 사용할 수 있게 보다 용이하게 구성된 간편추론의 방법이다. (출처: [위키백과](https://ko.wikipedia.org/wiki/%ED%9C%B4%EB%A6%AC%EC%8A%A4%ED%8B%B1_%EC%9D%B4%EB%A1%A0))

![끝까지어쩌구](https://velog.velcdn.com/images/eugene-doobu/post/3041e45c-f9cf-4e8b-b499-c1a96efa7e7d/image.png)

결국 '모든 경로를 탐색하지 않기 위해서'는 '항상 최선의 길을 찾을 각오'를 버려야 하고, 알잘딱깔센하게 연산하여 보기에 나쁘지 않은 정도의 결과를 보여주는 길찾기를 구현하는 것이다. 하지만 길찾기를 처음 구현해보는 입장에선 '알잘딱깔센'을 어떻게 해야하는지에 대해서 혼동이 온다.. 이를 이해하기 위해 일단 Astar알고리듬이 어떻게 구현되는지 먼저 알아보자.
<br>

a* 알고리듬의 공식은 '**f(x)=g(x)+h(x)**' 로 표현할 수 있다. 

- g(x)는 시작노드부터 현재 노드까지의 가중치의 합
- h(x)는 현재 노드부터 도착 노드까지의 '예상 가중치', 위에서 설명한 휴리스틱한 값이 들어가
- 결국 Astar알고리듬은 이 **휴리스틱 함수에 의해 좌지우지된다.** 상황에 맞는 적절한 휴리스틱 함수를 결정해야만 효율적인 길찾기를 할 수 있게 된다.
<br>

사실 이렇게 설명을 들어도 휴리스틱한 방법을 어떻게 구성해야할지 감이 안온다. 여기서는 일단 가장 간단한 경험적인 방법인 **맨해튼 거리**를 사용해보도록 하자. 2차원에서의 맨해튼거리는 두 점(p1, p2)와 (q1, q2)가 주어졌을 때, abs(q1-p1) + abs(p2-q2)연산으로 구할 수 있다.

이제 이를 코드로 구현해보자. 일단 기존과 동일하게 <span style="color:#73E097">컬러0</span>은 이동한 경로, <span style="color:#49CFBF">컬러1</span>은 목표지점으로 설정했다. 그리고 <span style="color:#4A96CF">컬러2</span>는 길찾기 알고리듬 수행시 방문한 노드, <span style="color:#4A4ECF">컬러3</span>은 이동할 수 없는 벽으로 설정하였다. 기존 알고리듬과 다르게 목적지 방향을 중심으로 길찾기 알고리듬을 수행한다는 것을 시각적으로 보여주기 위해 방문한 노드를 색칠해보았다.

<br>

!youtube[ZmyATJPxUSk?si=X7w5pFUfZ-CxGLs6]

[코드참고](https://github.com/eugene-doobu/pathfinding-example/blob/main/Assets/Project/Scripts/PathFinding/AStarPathFinding.cs)
<br>

길찾기가 아름답게 구현되었다! 우리가 구현한 방법으로는 장애물이 없는 곳에서 특히나 효율적으로 길을 찾는것으로 보인다. 이 경우 거의 목적지 방향으로 일직선으로 이동하는것을 확인할 수 있다. Astar를 적용함으로써 BFS보다 얼마나 효율적인지 비교해보기 위해 BFS알고리듬에서 방문한 노드를 색칠해보도록 하겠다.

![BFS가 안되는이유](https://velog.velcdn.com/images/eugene-doobu/post/9552af17-0403-4c04-aa7f-a952505d178d/image.png)

와우, BFS알고리즘과의 비교를 위해 이전 BFS코드에서 방문한 노드를 <span style="color:#4A4ECF">컬러3</span>으로 색칠해보도록 변경한 결과이다. 모든 노드가 파랗고 아름답게 물든것을 볼 수 있다. Astar영상과 비교해보면 왜 실전에서 BFS를 쓰기 힘든지 쉽게 알 수 있을 것이다.

![돼](https://velog.velcdn.com/images/eugene-doobu/post/c8511685-4555-4d1c-801a-bee962306555/image.png)

BFS와 Dijsktra를 넘어 A* 까지 왔다!! 이제 길찾기 기능구현 요청에 '된다'라고 자신있게 말하고 다닐 수 있을것같다. 하지만 이를 실제 프로젝트에 적용할 걸 생각해본다면 현실은 쉽지 않음을 알 수 있다. 예를 들어 이러한 문제들을 생각해보자

1. 지금까지 오브젝트는 한 노드에 1개씩 존재했다. 하지만 짱크고 강력한 캐릭터가 등장해 5x5크기의 노드를 차지해야한다면? (스타의 저글링, 울라리를 생각해보자)
1. 근데 저 저글링과 울라리가 100마리씩 맵의 끝에서 끝까지 길찾기 연산을 시켜주려면?? 아무리 A* 라고 해도 감당이 될까?
1. 온라인 게임이라고 한다면, 맵의 노드 정보는 서버가 어떻게 들고있어야 할까?
1. 과연.. 노드에서 노드로 이동하는 길찾기가 **보기 좋을까?**
1. 게임에서 벗어나, 길찾기 알고리듬을 실생활에 적용한다고 생각해보자. 만약 A* 알고리듬을 기반으로 실시간으로 변하는 교통량에 적응하는 네비게이션을 만든다고 했을때 이게 제대로 동작할 것인가?

![답이없네~](https://velog.velcdn.com/images/eugene-doobu/post/b5f75105-267c-42cc-bf17-89e0933e97d1/image.png)


그렇다. 사실 길찾기 알고리듬이란 답이 없다. 말 그대로 '알잘딱깔센'해서 현재 구현할 수 있는 만큼 적당히 연산부하 덜면서 보기좋을정도로 계속해서 손질하는 수 밖에 없다.

## A* 실적용 예시
상황에따라 위에 구현된 Astar 만으로는 실제 프로젝트에 적용하기 부족할 수 있다. 이제 실제 프로젝트에 맞게 A* 알고리듬을 개조시켜서 적용하는 예시 몇가지를 소개하고 글을 마치도록 하겠다.

### 지그재그 워킹 문제

우리는 육각형으로 시각화된 맵 위에서 길찾기를 했다. 동글이는 이동 경로는 육각형의 중앙에서 중앙으로 이동하며, 이러한 행동이 부자연스러워 보이진 않았다. 하지만 논리적으로는 쪼개져있지만 아트 에셋상에서 이러한 논리적인 경계가 뚜렷하게 보이지 않는 맵에서 이렇게 이동하면 어떻게 될까? 아래 영상을 봐보도록 하자.

!youtube[vWSXTyiCmmI?si=rqFOXPbVpb9TNs22]
<figcaption style="text-align:center; font-size:15px; color:#808080; margin-top:40px">"게임 Voxel Horizon의 길찾기 개발영상"</figcaption>

게임 맵의 바닥이 삼각형들로 분할되어있다. 이 삼각형 하나하나가 길찾기에 사용되는 노드들이라고 보면 된다. 특정 노드를 클릭시 해당 삼각형이 노란색으로 바뀌며, 얻게된 길찾기 경로에 초록색 선이 표현되며 캐릭터는 해당 경로를 따라 달려간다. 이 때 경로의 노드들은 삼각형의 무게중심을 기준으로 생성된다. 실제 게임에서 캐릭터가 저런 무빙을 보여주면 어떨까?

![깊은빡침](https://velog.velcdn.com/images/eugene-doobu/post/d7389e9c-fb54-4b2d-81ec-2e137fac8744/image.png)

바로 이런 표정을 지어버리면서 길찾기는 봉인한 상태로 키보드 수동 입력으로 이동을 하려 할 것이다. 위에서 이야기했던 '노드에서 노드로 이동하는 길찾기가 보기 좋을까'라고 물었던 이유가 바로 이것이다. 유저가 보기에 논리적인 경계가 뚜렷하게 보이지 않는 맵에서 A* 로 주어진 길찾기 경로를 지그재그로 이동하며 굉장히 멍청하게 이동하는 것처럼 보일 것이다.

이걸 보고 '저 삼각형을 잘게 쪼개면 되는거 아닌가요?'라고 생각할 수 있다. 어떤 상황에서는 그게 정답이 될 수도 있지만, 팰월드와 같은 넓은 월드에서 삼각형을 잘게 쪼개버리면 연산량이 폭발적으로 늘어나게 될 것이고, 우리의 감자 서버는 다시 한번 폭발을 하게 될 것이다. 그럼 위 게임의 제작자는 이러한 문제를 어떻게 해결했는지 구경해보자.

<br>

!youtube[qcm1nqCHN6k?si=vLmdL70gemp49MXh]

<br>

영상을 보면 길찾기 도착 위치까지의 초록색 삼각형이 렌더링되는것을 확인할 수 있다. 이는 Stupid Funnel 알고리즘과 유사한 방법을 적용해 구현된 것으로, 각 삼각형 노드들에 대해 가시성 검사와 유사한 방식으로 테스트를 해 한번에 직선으로 이동할 수 있는 만큼 최대한 이동하게끔 알고리즘이 구현되었다. 현재 캐릭터의 위치에서 다음 노드의 맞닿아 있는 면까지를 최초 프러스텀으로 하여, 다음 경로의 맞닿은 면이 이전 프러스텀에 포함되게끔 재귀적으로 다음 프러스텀을 구하며, 프러스텀 면의 크기가 캐릭터의 크기보다 작아지게 되면 그 위치까지 직선으로 이동하는 것이다.

![프러스텀](https://velog.velcdn.com/images/eugene-doobu/post/2f7eabf2-cc14-4076-ac8c-31ae2103a50f/image.png)


[블로그 글](https://megayuchi.com/2020/11/14/3d-%ea%b8%b8%ec%b0%be%ea%b8%b0-%ea%b5%ac%ed%98%84%ec%a4%91-3-visibility-%ed%85%8c%ec%8a%a4%ed%8a%b8%ec%99%80-%ec%9c%a0%ec%82%ac%ed%95%9c-stupid-funnel-%ec%95%8c%ea%b3%a0%eb%a6%ac%ec%a6%98-%ec%a0%81/) / [강좌](https://github.com/megayuchi/ppt/blob/main/docs/2021_0706_%EB%84%A4%EB%B9%84%EA%B2%8C%EC%9D%B4%EC%85%98%20%EB%A7%A4%EC%8B%9C%EB%A5%BC%20%EC%9D%B4%EC%9A%A9%ED%95%9C%203D%EA%B2%8C%EC%9E%84%20%EA%B8%B8%EC%B0%BE%EA%B8%B0%20%EC%A0%84%EB%9E%B5.pdf)

![Stupid funnel](https://velog.velcdn.com/images/eugene-doobu/post/e4213f70-bc31-4e89-b869-f10445471729/image.png)

[FunnelModifier 문서](https://arongranberg.com/astar/documentation/dev_4_1_7_6425cc50/class_pathfinding_1_1_funnel_modifier.php)

이러한 개선방식의 단점으로는 벽을 끼고 이동하는 경우 캐릭터가 벽에 붙어서 이동하는듯한 느낌을 준다는 것이다. 위 사진의 가장 오른쪽에 있는 'Shortest path in funnel' 이미지에 있는 경로같은 느낌으로 캐릭터가 이동한다고 생각하면 된다.

### 단체 길찾기 문제

![단체 길찾기 이상편](https://velog.velcdn.com/images/eugene-doobu/post/3c3953c3-9979-45fd-803a-15c932447d0f/image.gif)
<figcaption style="text-align:center; font-size:15px; color:#808080; margin-top:40px">"단체 길찾기 이상편"</figcaption>

![단체 길찾기 현실편](https://velog.velcdn.com/images/eugene-doobu/post/3ad8e715-7634-4901-99dc-67b2715299ec/image.gif)
<figcaption style="text-align:center; font-size:15px; color:#808080; margin-top:40px">"단체 길찾기 현실편"</figcaption>

([움짤 출처](https://gamedev.stackexchange.com/questions/191954/how-can-i-create-the-arriving-engaging-in-combat-movement-like-in-starcraft-2))

스타와 같은 게임에서 대규모 군대가 충돌한다고 했을때는 어떻게 구현해야 유닛들의 동작이 이쁘게 구현될 수 있을까? 위 단체 길찾기 현실편 예시만 보아도 이와 같은 구현이 쉽지 않음을 알 수 있다. 이와 관련하여 [스타크래프트2에서의 길찾기 방법에 대한 강연](https://gdcvault.com/play/1014514/AI-Navigation-It-s-Not)에서 찾아볼 수 있다고 한다.

스타2에서는 델로네 삼각분할로 제한된 노드 위에서 Astar알고리즘과 Funnel알고리즘을 이용하여 기본적인 길찾기 알고리즘을 구현하였고, 부대 안에서 유닛들의 간격은 Boid알고리즘을 통해 구현되었다고 한다. 이에 대한 내용은 여백이 부족하여 적지 않겠다..

![여백이모잘라](https://velog.velcdn.com/images/eugene-doobu/post/e8dd6af2-8e22-407e-8cf6-636e36c0b469/image.png)

이외에도 길찾기 알고리듬을 실전에 적용하기 위해서나 변태들이 지적 유희를 위해 변형시킨 Astar알고리듬들이 존재한다. 전쟁게임의 경우 부대가 굉장히 넓은 맵에서 이동 경로를 완벽하게 결정한 후 이동해야하기 때문에 넓은 맵에서 특히 유용한 [JPS](https://www.youtube.com/watch?v=rfOgaPXCADQ)라는 알고리듬을 사용하기도 하고, 넓은 지형에 사용하면서도 유동적인 도로 상태를 실시간으로 반영하기 위해 내비게이션에서는 [CCH](https://tech.kakao.com/2021/05/10/kakaomap-cch/)알고리즘을 사용한다고도 한다.


## 다음편 예고..

![공간분할](https://velog.velcdn.com/images/eugene-doobu/post/df77c521-f8c3-4001-8410-2a93f1936b33/image.png)

### 끝

![땡큐](https://velog.velcdn.com/images/eugene-doobu/post/36e57082-b7c0-4aa9-a0e6-a251d740cf27/image.png)

---
#### 참고
[C#과 유니티로 만드는 MMORPG 게임 개발 시리즈](https://www.inflearn.com/course/%EC%9C%A0%EB%8B%88%ED%8B%B0-mmorpg-%EA%B0%9C%EB%B0%9C-part2/dashboard)
[네비게이션 매시를 이용한 3D게임 길찾기 전략](https://www.youtube.com/watch?v=8-4KzycX_9o&list=PL00yTT-RECdWsBjP-rQcDBelgehOyToy3&index=19)
[Boids for RTS](https://www.jdxdev.com/blog/2021/03/19/boids-for-rts/)
[[알고리즘][길찾기] A* 알고리즘](https://zprooo915.tistory.com/78)
[네비게이션 매시를 이용한 3D게임 길찾기 전략](https://github.com/megayuchi/ppt/blob/main/docs/2021_0706_%EB%84%A4%EB%B9%84%EA%B2%8C%EC%9D%B4%EC%85%98%20%EB%A7%A4%EC%8B%9C%EB%A5%BC%20%EC%9D%B4%EC%9A%A9%ED%95%9C%203D%EA%B2%8C%EC%9E%84%20%EA%B8%B8%EC%B0%BE%EA%B8%B0%20%EC%A0%84%EB%9E%B5.pdf)