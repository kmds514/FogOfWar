# 1. FogOfWar
탑다운 게임에서 사용하는 전장의 안개 시스템을 구현하는 프로젝트입니다.  
https://drive.google.com/file/d/1npRdF_VlxJgz_ky6LV_dMP3w4FjsmYBK/view?usp=sharing  
https://youtu.be/8Igz48XFkBk  



# 2. Content/Core
### 2-1. Bluerpints
게임 인스턴스와 로비 관련 블루프린트입니다.

### 2-2. Maps
프로젝트에서 사용하는 모든 레벨입니다.  
DreamscapeMeadows 레벨과 LostRuin 레벨을 열기 위해서는 추가 에셋이 필요합니다.  
https://www.unrealengine.com/marketplace/ko/product/dreamscape-nature-meadows
https://www.unrealengine.com/marketplace/ko/product/modular-lost-ruins-kit  



# 3. Content/FogOfWar
### 3-1. Blueprints
TopDownGrid를 참조하여 안개 텍스처를 업데이트하는 블루프린트입니다.  

### 3-2. Materials
안개 텍스처를 참조하는 머리티얼입니다.  



# 4. Content/TopDown
### 4-1. Blueprints
탑다운 게임 방식을 위한 블루프린트입니다.  



# 5. Content/UI
### 5-1. Blueprints
프로젝트에서 사용하는 모든 위젯 블루프린트입니다.  



# 6. Source\FogOfWar\Public\FogOfWar
### 6-1. FogManager
TopDownGrid와 TopDownPC를 참조합니다. FogTexture를 생성하고 업데이트합니다.  
TopDownPC가 소유한 유닛의 정보를 FogTexture에게 전달합니다. TopDownPC가 소유하지 않은 유닛의 가시성을 결정합니다.  

### 6-2. FogTexture
실제 텍스처(Texture2D)를 생성하고 업데이트하는 클래스입니다. 텍스처를 업데이트하기 위해 2개의 버퍼(SoruceBuffer, UpscaleBuffer)를 사용합니다.  
FogManager에게 받은 데이터로 SourceBuffer로 업데이트합니다. SourceBuffer의 데이터로 UpscaleBuffer를 업데이트합니다. UpscaleBuffer의 데이터로 텍스처를 렌더링합니다.  


# 7. Source\FogOfWar\Public\TopDown
### 7-1. TopDownCamera
탑다운 뷰를 지원하는 카메라 컴포넌트만 가지고 있는 폰입니다. 기본적인 화면 이동을 지원합니다.  

### 7-2. TopDownGameInstance, TopDownGameMode
멀티플레이에서 각 플레이어를 구별하는데 사용합니다.  

### 7-3. TopDownGrid
레벨에 가상의 그리드맵을 생성해주는 클래스입니다.  
월드 좌표를 그리드 좌표로 변환해주고 각 타일의 높이 값과 월드 좌표를 맵의 형태로 저장합니다.  

### 7-4. TopDownHUD
TopDownUnit을 드래그로 선택하는데 사용합니다.

### 7-5. TopDownPlayerController
자신이 소유한 TopDownUnit을 구별하고 조종하는데 사용합니다.  

### 7-6. TopDownUnit
TopDownUnitInterface를 구현합니다.  
팀 정보와 시야 정보를 저장합니다.  

### 7-7. TopDownUnitInterface
유닛 선택 효과와 피아식별을 위한 인터페이스입니다.  
