# 트리

트리는 비선형구조로 계층적 관계(Hierarchical Relationship)을 표현하는 자료구조이다.

### 트리 용어

- Node: 트리를 구성하고 있는 각각의 요소
- Edge: 트리를 구성하기 위해 노드와 노드를 연결한 선
- Root Node: 트리 구조에서 최상위에 있는 노드
- Terminal Node(Leaf): 하위에 다른 노드가 연결되어 있지 않은 노드
- Internal Node: 단말노드를 제외한 모든 노드로 루트노드도 포함
- Tree Traversal: 트리 순회, 트리의 모든 노드들을 방문하는 과정

## 트리의 순회(Tree Traversal)

### 전위 순회

깊이 우선 순회(DFT, Depth-First Traversal)이라고도 부름

트리를 복사하거나, 전위 표기법을 구하는데 주로 사용

트리를 복사할 때 전위 순회를 사용하는 이유는 트리를 생성할 때 자식 노드보다 부모 노드가 먼저 생성되어야 하기 때문

#### 전위 순회 순서

1. Root 노드 전위 순회
1. 왼쪽 서브트리 방문
1. 오른쪽 서브트리 전위 순회

```C++
void Preorder(Node *root){
    if(root == NULL) return;
    printf("%c ", root->data);
    Preorder(root->Left);
    Preorder(root->right);
}
```

### 중위 순회

왼쪽 오른쪽 대칭 순서로 순회를 하기 때문에 대칭 순회(Symmetric traversal)이라고도 부름

이진탐색트리(BST)에서 오름차순 또는 내림차순으로 값을 가져올 때 사용

내림차순으로 값을 가져오기 위해서는 역순(오른쪽 -> root -> 왼쪽)으로 중위 순회를 하면 됨

#### 중위 순회 순서

1. 왼쪽 서브트리 중위 순회
1. Root노드 방문
1. 오른쪽 서브트리 중위 순회

```C++
void Inorder(Node *root){
    if(root == NULL) return;
    Inorder(root->Left);
    printf("%c ", root->data);
    Inorder(root->right);
}
```

### 후위 순회

트리를 삭제하는데 주로 사용, 부모노드를 삭제하기 전에 자식 노드를 삭제해야 하기 때문

#### 후위 순회 순서

1. 왼쪽 서브트리 후위 순회
1. 오른쪽 서브트리 후위 순회
1. Root노드 방문

```C++
void Postorder(Node *root){
    if(root == NULL) return;
    Postorder(root->Left);
    Postorder(root->right);
    printf("%c ", root->data);
}
```

## 이진트리

## 이진탐색트리(BST)

## 이진 힙

## B트리

## 레드블랙트리
