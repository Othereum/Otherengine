# Otherengine

C++와 OpenGL로 만드는 게임 엔진 (W.I.P)

* C++20, OpenGL 3.3, SDL2

* Windows, OS X, Linux 지원

* 수학 라이브러리 [otm](https://github.com/Othereum/otm)

* 3D 렌더링
  * 커스텀 GLSL 쉐이더 지원
  * 3D 메시 로드 (.omesh 커스텀 포맷)
  * 텍스처 로드 (.png)

* 게임 프레임워크
  * UE4 스타일
  * 액터 - 컴포넌트
  * 월드 타이머

* 코어 타입
  * [Name](https://github.com/Othereum/Otherengine/blob/master/Source/Public/Name.hpp) - 실제 문자열에 대한 해시값을 가지고 있음. 복사 및 비교는 단순 정수 대상 연산이 되므로 굉장히 빠름. `map`의 키 타입으로 사용할 때 유용. 대소문자 구분 없음. UE4의 `FName`에 대응됨
  * [Path](https://github.com/Othereum/Otherengine/blob/master/Source/Public/Path.hpp) - `Name`과 비슷하나, 파일의 경로를 위한 타입. 절대/상대 경로를 섞어 써도 문제 없음. 모든 플랫폼에서 대소문자 구분 없음.
  * [Monitor\<T\>](https://github.com/Othereum/Otherengine/blob/master/Source/Public/Templates/Monitor.hpp) - 멀티 스레드 환경을 위한 wrapper 클래스. `T`에 접근할 때마다 자동으로 mutex lock을 걸어줌.
  * [SharedPtr\<T\>](https://github.com/Othereum/Otherengine/blob/master/Source/Public/Templates/SharedPtr.hpp) - `std::shared_ptr<T>`와 기능은 같지만, 단일 스레드 환경을 위한 최적화가 포함되어있음.

* 기타 예정된 기능들
  * 멀티플레이어 지원
  * [~~커스텀 메모리 관리자~~](https://github.com/Othereum/omem) (성능이 생각보다 느려서 무기한 연기)
  * ...
