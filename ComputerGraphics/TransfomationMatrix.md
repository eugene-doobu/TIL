# 변환행렬

Local → World → View → Projection → Screen

World행렬은 물체마다 따로 있음

## Local Space

Right, Up, Look Vector

## World Space

(x, y, z, 1)에 world matrix를 곱해주면

(

Rx, Ry, Rz, 0

Ux, Uy, Uz, 0

Lx, Ly, Lz, 0

Px, Py, Pz, 1

)

P: Position

SRT

## View

카메라의 월드변환행렬의 역행렬이 키

Idea

1) 카메라가 원점에 있다고 가정하고, 카메라 이동의 반대를 모델들에 적용

2) 카메라의 월드변환행렬의 역행렬

3) XMMatrixLookAtLH

LH: Left Hand, 왼손 좌표계

Rotation행렬은 직교행렬! → 대치로 역

Translation행렬은 단순 x,y,z에 -를 곱함으로써 역

## Projection

투영정책: 원근, 직교

FoV: 카메라가 찍을 수 있는 각도, 일반적으로 60도
계산하기 위해서는 90도(tan45=1을 이용)로 가정하는게 편하다

투영좌표계의 핵심은 모든것을 비율로 생각

r = w/h

r을 구하는 목적은 화면의 비율이 커진다고 물체의 비율도 커지는 것을 방지하기 위함

시야각처리: tan(theta)/2

투영연산은 1차식이 아니므로 1개의 행렬로 처리 불가, 일단 x,y에 대한 행렬식을 적용한 후 z에 대한 연산은 따로 처리