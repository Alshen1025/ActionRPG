#pragma once

#include "CoreMinimal.h"
#include "ActionRPGBaseCharacter.h"
#include "WarriorCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputConfigDataAsset;
struct FInputActionValue;


UCLASS()
class ACTIONRPG_API AWarriorCharacter : public AActionRPGBaseCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion


#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UInputConfigDataAsset* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

#pragma endregion
};
