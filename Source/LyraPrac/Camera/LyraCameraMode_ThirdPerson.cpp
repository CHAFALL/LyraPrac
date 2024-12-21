// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

ULyraCameraMode_ThirdPerson::ULyraCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

// 스프링 암과 달리 부드러운 느낌을 낼 수 있음을 알 수 있음.

void ULyraCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// (추가된 점)
	// TargetOffsetCurve가 오버라이드되어 있다면, Curve에 값을 가져와서 적용 진행
	// - Camera 관점에서 Charater의 어느 부분에 Target으로 할지 결정하는 것으로 이해하면 됨
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
