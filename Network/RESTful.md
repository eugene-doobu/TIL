# REST와 RESTFful의 개념

- Representational State Transfer (대표적인 상태 전달)의 약자
- REST는 웹의 기존 기술과 HTTP 프로토콜을 그대로 활용하기 때문에 웹의 장점을 최대한 활용할 수 있는 아키텍처 스타일
- Client 와 Server 사이의 통신 방식 중 하나

## REST의 개념

- HTTP URI(Uniform Resource Identifier)를 통해 자원(Resource)을 명시하고, HTTP Method(Post, Get, Put, Delete)를 통해 해당 자원에 대한 CRUD Operation을 적용하는 것을 의미한다.
- REST는 자원 기반의 구조. 설계의 중심에 Resource가 있고, HTTP Method를 통해 Resource를 처리하도록 설계된 아키텍처를 의미
- 웹 사이트의 이미지, 텍스트, DB 내용 등 모든 자원에 고유한 ID인 HTTP URI를 부여

## REST 특징

- Server-Client 구조
- Stateless
- Cacheable
- Layered System
- Code on Demand
- Uniform Interface

## REST가 필요한 이유

- 애플리케이션의 분리 및 통합
- 다양한 클라이언트의 등장(모바일 기기 등)
- MSA(Micro Service Application) 분산 서비스와 연관

## REST 구성 요소

### Resource URI

- 모든 자원에 고유한 id가 존재하고, 이 자원은 Server에 존재
- 자원을 구별하는 id는 'localhost:8080/user/:userId' 와 같은 HTTP URI
- Client는 URI를 이용해서 자원을 지정하고 해당 자원의 상태(정보)에 대한 조작을 Server에 요청

### 행위(Verb): Http Method

- HTTP 프로토콜의 Method를 사용
- GET, POST, PUT, DELETE, HEAD와 같은 메서드 제공

### 표현(Representation of Resource)

- Client가 자원의 상태에 대한 조작을 요청하면 Server는 이에 적절한 응답(Representation)을 보냄
- REST에서 하나의 자원은 JSON, XML, TEXT, RSS등 여러 형태의 응답으로 나타내어 질 수 있음
- 현대에는 JSON으로 표현

## RESTful

- REST 원리를 따르는 시스템
- REST를 REST답게 쓰기 위한 방법, 누군가가 공식적으로 발표한 것이 아님

### RESTful의 목적

- 이해하기 쉽고 사용하기 쉬운 REST API를 만드는 것
- RESTful API를 구현하는 근본적인 목적은 퍼포먼스 향상에 있는 것이 아니라, 일괄적인 컨벤션을 통한 API의 이해도 및 호환성을 높이는게 주 동기(퍼포먼스가 중요한 상황에서 굳이 RESTful API를 구현할 필요는 없음)
