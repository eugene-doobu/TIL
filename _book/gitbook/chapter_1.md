# 1. 시작하기

# 1.1. 버전관리란?

이번 장에서는 Git의 탄생 배경, Git을 사용하는 이유, Git을 설정하고 사용하는 방법을 터득할 것이다.

## 버전관리란?

- 버전 관리 시스템은 파일 변화를 시간에 따라 기록했다가 나중에 특정 시점의 버전을 다시 꺼내올 수 있는 시스템
- 소스 코드는 물론 모든 컴퓨터 파일의 버전 관리 가능
- 프로젝트를 통째로 이전 상태로 되돌릴 수 있고, 시간에 따라 수정 내용을 비교해 볼 수 있고, ㄱ누가 문제를 일으켰는지 추적할 수 있고, 누가 언제 만들어낸 이슈인지도 알 수 있다.

### 로컬 버전 관리

- 간단한 방법으로, 버전을 관리하기 위해 디렉토리로 파일을 복사하는 방법
- 간단하지만, 뭔가 잘못되기 쉽다.

### 중앙집중식 버전 관리(CVCS)

- 다른 개발자와 함께 작업해야 하는 경우 생기는 문제를 해결하기 위해 개발됨
- 파일을 관리하는 서버가 별도로 있고 클라이언트가 중앙 서버에서 파일을 받아서 사용(Checkout) 한다.

**장점**

- 누가 무엇을 하고 있는지 알 수 있다.
- 관리자는 누가 무엇을 할 지 꼼꼼하게 관리할 수 있다.
- 모든 클라이언트의 로컬 데이터베이스를 관리하는 것보다 VCS 하나를 관리하기가 훨씬 쉽다.

**단점**

- 중앙 서버에 문제가 발생하면 그동안 아무도 다른 사람과 협업할 수 없고 사람들이 하는 일을 백업할 방법도 없다

### 분산 버전 관리 시스템(DVCS)

Git같은 DVCS에서의 클라이언트는 단순히 파일의 마지막 스냅샷을 Checkout하지 않는다. 그냥 저장소를 히스토리와 더불어 전부 복제한다. 서버에 문제가 생기면 이 복제물로 다시 작업을 시작할 수 있다. 클라이언트 중에서 아무거나 골라도 서버를 복원할 수 있다. Clone은 모든 데이터를 가진 진정한 백업이다.

대부분의 DVCS환경에서는 리모트 저장소가 존재한다. 리모트 저장소가 많을 수도 있다. 그래서 사람들은 동시에 다양한 그룹과 다양한 방법으로 협업할 수 있다. 계층 모델 같은 중앙집중식 시스템으로는 할 수 없는 워크플로를 다양하게 사용할 수 있다.

# 1.2. 짧게 보는 Git의 역사

Linux가 BitKeeper를 사용하다 무료 사용이 불가능해져서 만들게 된 도구가 Git이다. 다음과 같은 목표를 세우고 개발이 진행되었다.

- 빠른 속도
- 단순한 구조
- 비선형적인 개발(수천 개의 동시 다발적인 브랜치)
- 완벽한 분산
- Linux 커널 같은 대형 프로젝트에도 유용할 것(속도나 데이터 크기 면에서)

# 1.3. Git 기초

### 차이가 아니라 스냅샷

Subversion와 비슷한 놈들과 Git의 가장 큰 차이점은 데이터를 다루는 방법에 있다.

- VCS시스템 대부분은 관리하는 정보가 파일들의 목록
- Subversion와 비슷한 것들은 각 파일의 변화를 시간순으로 관리하면서 파일들의 집합을 관리(보통 델타 기반 버전관리 시스템이라고 함)
- Git은 데이터를 파일 시스템 스냅샷의 연속으로 취급하고 크기가 아주 작음. 성능을 위해 파일을 새로 저장하지 않고 단지 이전 상태의 파일에 대한 링크만 저장함.

### 거의 모든 명령을 로컬에서 실행

- 프로젝트의 모든 히스토리가 로컬 디스크에 있기 때문에 모든 명령이 순식간에 실행된다.
- 오프라인 상태이거나 VPN에 연결하지 않아도 일을 할 수 있다.

### Git의 무결성

- Git은 데이터를 저장하기 전에 항상 체크섬을 구하고, 그 체크섬으로 데이터를 관리한다.
- 체크섬을 이해하는 Git 없이는 어떠한 파일이나 디렉토리도 변경할 수 없다
- 체크섬은 Git에서 사용하는 가장 기본적인(Atomic)데이터 단위이자 Git의 기본 철학이다.
- Git은 SHA-1 해시를 사용하여 체크섬을 만든다. 만든 체크섬은 40자 길이의 16진수 문자열이다.
- Git은 모든 것을 해시로 식별하고 저장한다.

### Git은 데이터를 추가하 뿐

- Git으로 무얼 하든 Git 데이터베이스에 데이터가 추가 된다. 되돌리거나 데이터를 삭제할 방법이 없다.
- Git을 사용하면 프로젝트가 심각하게 망가질 걱정 없이 매우 여러가지 실험을 해 볼 수 있다.

## 세 가지 상태

**!이부분은 중요해서 집중해서 읽어야 한다!**

Git은 파일을 세 가지 상태로 관리한다

- Committed란 데이터가 로컬 데이터베이스에 안전하게 저장됐다는 것을 의미
- Modified는 수정한 파일을 아직 로컬 데이터베이스에 커밋하지 않은 것을 의미
- Staged란 현재 수정한 파일을 곧 커밋할 것이라고 표시한 상태를 의미

이 세 가지 상태는 Git 프로젝트의 세 가지 단계와 연결되어 있다. Git 디렉토리, 워킹 트리, Staging Area 이렇게 세 가지 단계를 이해하고 넘어가자.

![햣.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/c1d0d7b1-5969-42b7-9af4-13b4cbfee4ff/햣.png)

Git 디렉토리는 Git이 프로젝트의 메타데이터와 객체 데이터베이스를 저장하는 곳을 말한다. 이 Git 디렉토리가 Git의 핵심이다. 다른 컴퓨터에 있는 저장소를 Clone할 때 Git 디렉토리가 만들어진다.

워킹 트리는 프로젝트의 특정 버전을 Checkout한 것이다. Git 디렉토리는 지금 작업하는 디스크에 있고 그 디렉토리 안에 압축된 데이터베이스에서 파일을 가져와서 워킹 트리를 만든다.

Staging Area는 Git 디렉토리에 있다. 단순한 파일이고, 곧 커밋할 파일에 대한 정보를 저장한다. Git에서는 기술용어로 “Index”라고 하지만, “Staging Area”라는 용어를 써도 상관 없다.

Git으로 하는 일은 기본적으로 아래와 같다.

1. 워킹 트리에서 파일을 수정한다.
2. Staging Area에 파일을 Stage해서 커밋할 스냅샷을 만든다. 모든 파일을 추가할 수도 있고 선택하여 추가할 수도 있다.
3. Staging Area에 있는 파일들을 커밋해서 Git 디렉토리에 영구적인 스냅샷으로 저장한다.

Git 디렉토리에 있는 파일들은 Committed 상태이다. 파일을 수정하고 Staging Area에 추가했다면 Staged이다. 그리고 Checkout하고 나서 수정했지만, 아직 Staging Area에 추가하지 않았으면 Modified이다.

# 1.4. CLI

Git을 시작하는 방법은 많지만 이 책에서는 CLI를 사용한다. Git의 모든 기능을 지원하는 것은 CLI 뿐이다.

# 1.5. Git 설치

스킵

# 1.6. Git 최초 설정

Git을 설치하고 나면 Git의 사용 환경을 적절하게 설정해 주어야 한다. ‘git config’라는 도구로 설정 내용을 확인하고 변경할 수 있다. Git은 이 설정에 따라 동작한다. 이 때 사용하는 설정 파일은 세 가지나 된다.

1. /etc/gitconfig: 시스템의 모든 사용자와 모든 저장소에 적용되는 설정이다. git config —system 옵션으로 이 파일을 읽고 쓸 수 있다.
2. ~/.gitconfig, ~/.config/git/config: 특정 사용자(즉 현재 사용자)에게만 적용되는 설정이다. git config —global 옵션으로 이 파일을 읽고 쓸 수 있다. 특정 사용자의 모든 저장소 설정에 적용된다.
3. .git/config: 이 파일을 Git 디렉토리에 있고, 특정 저장소에만 적용된다. —local 옵션을 사용하면 이 파일을 사용하도록 지정할 수 있다. 하지만 기본적으로 이 옵션이 적용되어 있다.

이 설정은 역순으로 우선시된다.

## 사용자 정보

Git을 설치하고 나서 가장 먼저 해야하는 것은 사용자이름과 이메일 주소를 설정하는 것이다. Git은 커밋할 때 마다 이 정보를 사용한다.

```arduino
$ git config --global user.name "John Doe"
$ git config --global user.email johndoe@example.com
```

## 편집기

사용자 정보를 설정하고 나면 Git에서 사용할 텍스트 편집기를 고른다. 기본적으로 Git은 시스템의 기본 편집기를 사용한다.

## 설정 확인

git config —list 명령을 실행하면 설정한 모든 것을 보여주어 바로 확인할 수 있다.

```arduino
$ git config --list
user.name=John Doe
user.email=johndoe@example.com
color.status=auto
color.branch=auto
color.interactive=auto
color.diff=auto
...
```

Git은 같은 키를 여러 파일에서 읽기 때문에 같은 키가 여러 개 있을 수도 있다. 그러면 Git은 나중에 나온 값을 사용한다.

git config <key> 명령으로 특정 key에 대해 어떤 값을 사용하는지 확인할 수 있다.

# 1.7. 도움말 보기

명령어에 대한 도움말이 필요할 때 보는 방법은 두 가지 있다.

```arduino
$ git help <verb>
$ man git-<verb>
```

Git 명령을 사용하기 위해 매우 자세한 도움말 전체를 볼 필요 없이 각 명령에서 사용할 수 있는 옵션들에 대해서 간략히 살펴볼 수도 있다. -h, —help 옵션을 사용하면 다음과 같이 Git 명령에서 사용할 수 있는 옵션들에 대한 간단한 도움말을 출력한다.

```arduino
$ git add -h
usage: git add [<options>] [--] <pathspec>...

    -n, --dry-run         dry run
    -v, --verbose         be verbose

    -i, --interactive     interactive picking
    -p, --patch           select hunks interactively
    -e, --edit            edit current diff and apply
    -f, --force           allow adding otherwise ignored files
    -u, --update          update tracked files
    -N, --intent-to-add   record only the fact that the path will be added later
    -A, --all             add changes from all tracked and untracked files
    --ignore-removal      ignore paths removed in the working tree (same as --no-all)
    --refresh             don't add, only refresh the index
    --ignore-errors       just skip files which cannot be added because of errors
    --ignore-missing      check if - even missing - files are ignored in dry run
    --chmod <(+/-)x>      override the executable bit of the listed files
```

# 1.8. 요약

우리는 Git이 무엇이고 지금까지 사용해 온 다른 CVCS와 어떻게 다른지 배웠다. 시스템에 Git을 설치하고 사용자 정보도 설정했다. 다음 장에서는 Git의 사용법을 배운다.