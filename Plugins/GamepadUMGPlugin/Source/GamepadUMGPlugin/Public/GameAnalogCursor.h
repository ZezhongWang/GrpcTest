/*
	This code was written by Nick Darnell
	
	Plugin created by Rama
*/
#pragma once
#if USING_CODE_ANALYSIS
MSVC_PRAGMA(warning(push))
MSVC_PRAGMA(warning(disable : ALL_CODE_ANALYSIS_WARNINGS))
#endif // USING_CODE_ANALYSIS
#pragma warning(push)
#pragma warning(disable:4996)

#include "CoreMinimal.h"
#include "GenericPlatform/ICursor.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/AnalogCursor.h"

class FGameAnalogCursor : public FAnalogCursor
{
public:

	GAMEPADUMGPLUGIN_API static void EnableAnalogCursor(class APlayerController* PC, TSharedPtr<SWidget> WidgetToFocus);
	GAMEPADUMGPLUGIN_API static void DisableAnalogCursor(class APlayerController* PC);
	
	/** Ctor */
	FGameAnalogCursor(class APlayerController* PC, float _Radius);

	/** Dtor */
	virtual ~FGameAnalogCursor()
	{}

	/** Uses different logic than base cursor */
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;

	/** Getter */
	FORCEINLINE FName GetHoveredWidgetName() const
	{
		return HoveredWidgetName;
	}

	/** Getter */
	FORCEINLINE bool IsHovered() const
	{
		return HoveredWidgetName != NAME_None;
	}

	/** Getter */
	FORCEINLINE FVector2D GetCurrentPosition() const
	{
		return CurrentPosition;
	}

	/** Getter */
	FORCEINLINE FVector2D GetVelocity() const
	{
		return Velocity;
	}

	/** Getter */
	FORCEINLINE bool GetIsUsingAnalogCursor() const
	{
		return bIsUsingAnalogCursor;
	}

	/** Getter */
	FORCEINLINE FVector2D GetLastCursorDirection() const
	{
		return LastCursorDirection;
	}

	/** Getter */
	FORCEINLINE float GetRadius() const
	{
		return Radius;
	}

private:

	/** Helper Function. Takes in values from the analog stick, returns a vector that represents acceleration */
	FVector2D GetAnalogCursorAccelerationValue(const FVector2D& InAnalogValues, float DPIScale);

	/** Current velocity of the cursor */
	FVector2D Velocity;

	/** Current position of the cursor. Stored outside of ICursor's position to avoid float->int32 truncation */
	FVector2D CurrentPosition;

	/** Unit vector derived from Velocity */
	FVector2D LastCursorDirection;

	/** The name of the hovered widget */
	FName HoveredWidgetName;

	/** Is this thing even active right now? */
	bool bIsUsingAnalogCursor;

	/** The radius of the analog cursor */
	float Radius;

	/** The Player's Context */
	FLocalPlayerContext PlayerContext;

	FORCEINLINE FVector2D& GetAnalogStickValue(EAnalogStick Stick = EAnalogStick::Right)
	{
		return AnalogStickValues[static_cast<uint8>(Stick)];
	}

	/** Input from the gamepad */
	FVector2D AnalogStickValues[static_cast<uint8>(EAnalogStick::Max)];

};