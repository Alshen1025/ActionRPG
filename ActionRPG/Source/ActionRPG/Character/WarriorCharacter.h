#pragma once

#include "CoreMinimal.h"
#include "ActionRPGBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "WarriorCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputConfigDataAsset;
struct FInputActionValue;
class UHeroCombatComponent;
class UHeroUIComponent;

UCLASS()
class ACTIONRPG_API AWarriorCharacter : public AActionRPGBaseCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorCharacter();

#pragma region PawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
#pragma endregion

#pragma region PawnUIInterface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
#pragma endregion


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* HeroUIComponent;

#pragma endregion


#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UInputConfigDataAsset* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(const FInputActionInstance&, FGameplayTag InputTag);
	void Input_AbilityInputReleased(const FInputActionInstance&, FGameplayTag InputTag);

#pragma endregion

#pragma region Getter/Setter
public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return CombatComponent; }
#pragma endregion
};
