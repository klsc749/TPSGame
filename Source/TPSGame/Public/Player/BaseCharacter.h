// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "HealthComponent/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "WeaponComponent/WeaponComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TPSGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Camera Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cameta")
	UCameraComponent* CameraComponent;
	//Spring arm
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UWeaponComponent* WeaponComponent;
	//Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UHealthComponent* HealthComponent;
	//the text of health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UTextRenderComponent* HealthTextComponent;
	//Death animation
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint")
	float BaseSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint")
	float SprintSpeed = 1000.0f;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void BeginSprint();

	void EndSprint();

	void BeginCrouch();

	void EndCrouch();
	
	void BeginFire();

	void EndFire();

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;
	
	UFUNCTION(BlueprintCallable)
	float GetAOPitch() const;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComponent; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComponent; }

	bool IsSprinting() const {return IsSprint;}
private:
	bool IsSprint = false;
	void OnDeath();
	void OnHealthChange(float Health) const;	
};
