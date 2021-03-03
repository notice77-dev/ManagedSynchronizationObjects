# Managed RAII Lock Class Example

1. 사용 방법
- lock.h 파일의 "#define LOCK_NAME (NAME)" 코드 아래에 "NAME(ALLOCATOR_LOCK)" 와 같이 추가하려는 Lock 종류를 열거.
- "NAME(ALLOCATOR_LOCK)”의 경우 매크로에 의해 ALLOCATOR_LOCK enum 타입이 추가되고 내부 문자열 배열 변수에 "ALLOCATOR_LOCK" 문자열이 추가됨.
- 동기화가 필요한 곳에 lock.h 파일 인클루드후 동기화가 필요한 Lock 종류에 맞춰 사용. (예: "Common::CSLock<Common::Lock::ALLOCATOR_LOCK> lock(true)")
- 자동 해제 방식을 사용할 때는 초기화 매개변수를 true로, ManualLock 또는 TryLock 함수 사용시에는 false

2. 장점
- 모든 동기화 객체를 한곳에서 관리
- 동기화 객체별로 인스턴스가 하나만 생성되고 관리되는 것을 보장하여 무분별한 동기화 객체 사용을 방지
- 동기화 사용 코드 자체에 동기화 대상 명칭이 포함되어 있어 코드 분석이 용이하고 실수를 방지
- 동기화 객체의 이름 문자열을 출력 가능하여 문제 발생시 분석 및 추적 용이
