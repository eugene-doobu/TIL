# MVP 패턴

Model + View + Presenter

- Model: 어플리케이션에서 사용되는 데이터와 그 데이터를 처리하는 부분

- View: 사용자에게 보여지는 UI

- Presenter: View에서 요청한 정보로 Model을 가공하며, View와 Model을 연결해주는 수단

View와 Presenter가 서로 in out으로 연결되어 있고, Presenter와 Model이 서로 in out으로 연결되어 있는 형태

Model과 View는 서로 알지 못함(인스턴스를 직접적으로 조작하지 못함)


## 동작

1. 사용자의 Action이 View를 통해 들어옴
1. View로 들어온 데이터가 Presenter를 거쳐 Model로 전달
1. Model은 전달받은 데이터에 응답하여 Presenter를 거쳐 View로 데이터를 전달
1. View가 응답받은 데이터를 이용하며 화면을 나타냄
