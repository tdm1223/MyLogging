# MyLogging
- Logging 클래스를 설계하고 작성해보는 repository
- 프로그램 설계 능력 향상을 목표로 한다.

### 요구사항
1. 로깅 정보에는 어떤 **모듈**, **함수**, **위치**에서 문제점이 발생했는지를 알수 있어야 한다.
2. 로깅 정보가 **출력**되고, **파일**로 저장되고, 원격 목적지로도 **전송**되는 등 여러 목적지로 보낼 수 있어야 한다.
    - 파일로 로그를 기록할때 **날짜별**로 로그를 기록한다.
    - 로그 파일이 **특정 사이즈** 이상이면 파일을 새로 만들어 분리한다.
    - **일정 기간**이 지난 로그 파일은 삭제한다.
3. 모듈별로 로깅을 **활성화** 하거나 **비활성화** 할 수 있어야 한다.
4. 로깅을 얼마나 **상세**하게 할것인지 **조정**할 수 있어야 한다.
    - 로깅 수준 : `LOW`, `NORMAL`, `HIGH`, `CRITICAL`
5. **개발** 단계에서만 출력할 필요가 있는 **디버깅 정보**들은 **릴리즈** 단계에서는 자동으로 코드에서 없어져야 한다.
6. 모듈별로 **로깅 활성화**, **로깅 수준**은 프로그램 실행 중에 변경할 수 있어야 한다.
7. 로깅 정보는 **암호화**되어야 한다.

### 사용 언어
- C++
