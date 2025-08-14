# SDV_example
SDV 구독앱 개발 레퍼런스 코드

### 필수 요구 사항
---
- **Linux/Ubuntu** 환경 (v. 22.04~)
- **Docker**
- **popcornsar PACON-IDE 최신 버전 (v 1.2.16)**

### 도커 빌드 환경 구축
---
**In git root folder**

./popcorn.sh

### Build Application
---
#### 1. 도커접속

1\) ssh popcornsar@localhost -p 41004

password: 0 or edu!@#$(따로 도커 환경을 실행했을 시)

2\) docker exec -it edu_1.2.16 bash

#### 2. 환경변수 설정

source ~/sdv/SOURCE_THIS

#### 3. 폴더 구조

sdv/src/eevp_main_machine/  
├── ap_app/ # 모비스 앱 모음  
├── subscription_app/ # 구독 서비스 앱 모음  
└── adaptive_autosar/ # Adaptive Autosar 모음  

#### 4. 빌드 전 구독 앱 심볼릭링크 생성

각 어플리케이션 폴더에 adaptive_autosar폴더로 이어지는 심볼릭링크를 생성한다.

도커 환경에서  
**in eevp_main_machine**

./sub_symbollink.sh

#### 5. 어플리케이션 빌드

****빌드 전 ARXML 설계****

adaptive_autosar/arxml/eevp_reflect 이하 폴더들을 AutoSAR.io 프로젝트에 복사 후 작업  
앱 추가 시, adaptive_autosar/arxml/default_template에 있는 Template 파일을 복사 후 원하는 앱 이름으로 변경하여 추가  
ARXML 설계 완료 후 autoSAR.io 프로젝트의 gen_ara폴더를 adaptive_autosar/gen/ 경로로 복사 (최신화)  

****빌드****

5.1 직접 빌드 방법

가장 처음 빌드할 시에는 -c -p 옵션 필수 (-c는 초기화, -p는 바이너리 폴더 생성)

****필수****

bash ~/sdv/src/eevp_main_machine/ap_app/ap_sm/docker_build.sh -c -p

bash ~/sdv/src/eevp_main_machine/ap_app/ap_ipchandler/docker_build.sh

****선택(구독 어플리케이션)****

ex) bash ~/sdv/src/eevp_main_machine/subscription_app/ap_intelligent_wiper/docker_build.sh

5.2 간편 빌드 방법

cd ~/sdv/src/eevp_main_machine/  
build.sh 에서 빌드하고자 하는 modules 입력 및 주석 처리 후  
./build.sh

#### 6. 어플리케이션 실행

cd $PARA_CORE/bin  
./EM

(~/sdv/에 test.sh 스크립트로 존재)

