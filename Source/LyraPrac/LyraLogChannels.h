// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

// 로깅 매크로
// 타고 들어가보면 "extern" struct... 이렇게 되어있는데 extern을 설정 해놓고 따로 정의를 해주지 않으면 문제 발생, (선언만 해둔 상태임)
// 첫번째 인자: 우리가 사용하고 싶은 명칭, 두번째 인자: 분류, 세번째 인자: 보여줄 영역
DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);

// 매크로란? 텍스트 치환 시스템 (전처리기가 컴파일 전에 매크로로 정의도니 부분을 실제 코드로 바꿔줌)
