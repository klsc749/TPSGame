// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseCharacter.h"

#include "GameplayDebuggerTypes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TPSPlayerController.h"
#include "Components/AudioComponent.h"
#include "UI/BaseGameHUD.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChange.AddUObject(this, &ABaseCharacter::OnHealthChange);
	
	//Create health text component
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthReaderComponent");
	HealthTextComponent->SetupAttachment(RootComponent);
	HealthTextComponent->SetOwnerNoSee(false);
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetMaxHealth())));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("ChangeWeapon");
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetComponentTickEnabled(false);
	
	//Create Weapon Component
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChange.AddUObject(this, &ABaseCharacter::OnHealthChange);
	
	//Create health text component
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthReaderComponent");
	HealthTextComponent->SetupAttachment(RootComponent);
	HealthTextComponent->SetOwnerNoSee(false);
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetMaxHealth())));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("ChangeWeapon");
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetComponentTickEnabled(false);
	
	//Create Weapon Component
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

float ABaseCharacter::GetAOPitch() const
{
	float Ao_Pitch = GetBaseAimRotation().Pitch;
	if(Ao_Pitch > 90.0f && !IsLocallyControlled())
	{
		const FVector2D InRange(270.0f, 360.0f);
		const FVector2D OutRange(-90.0f, .0f);
		Ao_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, Ao_Pitch);
	}
	return Ao_Pitch;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	 TPSPlayerController = Cast<ATPSPlayerController>(GetController());
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::BeginSprint()
{
	IsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABaseCharacter::EndSprint()
{
	IsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void ABaseCharacter::BeginCrouch()
{
	Crouch();
}

void ABaseCharacter::EndCrouch()
{
	UnCrouch();
}

void ABaseCharacter::BeginFire()
{
	WeaponComponent->StartFire();
}

void ABaseCharacter::EndFire()
{
	WeaponComponent->StopFire();
}

bool ABaseCharacter::IsRunning() const
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size() > 0;
}

void ABaseCharacter::OnHealthChangeOnServer_Implementation(float Health)
{
	OnHealthChangeMulticast(Health);
}

void ABaseCharacter::OnHealthChangeMulticast_Implementation(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	TPSPlayerController = Cast<ATPSPlayerController>(GetController());
	if(!TPSPlayerController)
		return;
	TPSPlayerController->SetHealth(HealthComponent->GetHealthPercent());
}

void ABaseCharacter::OnDeathServer_Implementation()
{
	OnDeathMulticast();
}

void ABaseCharacter::OnDeathMulticast_Implementation()
{
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
	if(Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::OnDeath()
{
	if(HasAuthority())
	{
		OnDeathMulticast();
	}
	else
	{
		OnDeathServer();
	}
}

void ABaseCharacter::OnHealthChange(float Health)
{
	if(HasAuthority())
	{
		OnHealthChangeMulticast(Health);
	}
	else
	{
		OnHealthChangeOnServer(Health);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::EndSprint);
	// Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::EndCrouch);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::EndFire);

	//Change weapon
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UWeaponComponent::NextWeapon);

	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UWeaponComponent::Reload);

	//Throw
	PlayerInputComponent->BindAction("Throw", IE_Pressed, WeaponComponent, &UWeaponComponent::BeginThrow);
	PlayerInputComponent->BindAction("Throw", IE_Released, WeaponComponent, &UWeaponComponent::FinishThrow);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);
}

