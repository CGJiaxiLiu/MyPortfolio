// Fill out your copyright notice in the Description page of Project Settings.

#include "DrivePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Engine/StaticMesh.h"

ADrivePawn::ADrivePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			//: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO"))
			: PlaneMesh(TEXT("/Game/Assets/Blueprints/Meshes/SM_Hang_Glider.SM_Hang_Glider"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	CurrentForwardSpeed = MinSpeed;

}


void ADrivePawn::BeginPlay()
{
	Super::BeginPlay();

	if (SocketCreate())
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Black, FString("SocketClient Created Success!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Black, FString("Failed to Create SocketClient"));
	}
}

void ADrivePawn::BeginDestroy()
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Black, FString("Begin Destroy!"));
	if (SocketClient)
	{
		SocketClient->Close();
		UE_LOG(LogTemp, Warning, TEXT("SocketClient Closed!"));
	}
	Super::BeginDestroy();
}


void ADrivePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




	SocketSend(FString("Hello!!!"));
	SocketReceive(received, response);
	UE_LOG(LogTemp, Warning, TEXT("***response: %s"), *response);
	response.Split("z", &temp, &z);
	temp.Split("x", &left, &right);

	x = FCString::Atof(*left);
	y = FCString::Atof(*right);
	UE_LOG(LogTemp, Warning, TEXT("***result: %f %f"), x, y);

	MoveUpInput(x);
	RollRightInput(y);

	FVector LocalMove;
	if (z == "l")
		acce = true;
	else if (z == "r")
		acce = false;

	if (acce)
		LocalMove = FVector(CurrentForwardSpeed * 5 * DeltaTime, 0.f, 0.f);
	else
		LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);

	AddActorLocalOffset(LocalMove, true);
	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

}


void ADrivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ADrivePawn::ThrustInput);
	//PlayerInputComponent->BindAxis("MoveUp", this, &ADrivePawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADrivePawn::MoveRightInput);
	//PlayerInputComponent->BindAxis("Roll", this, &ADrivePawn::RollRightInput);

}


void ADrivePawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ADrivePawn::ThrustInput(float Val)
{
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.05f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ADrivePawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val *  MaxPitchSpeed * -1.f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ADrivePawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * MaxTurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 0.5);

}

void ADrivePawn::RollRightInput(float Val)
{
	// Target roll speed is based on input
	float TargetRollSpeed = (y * MaxRollSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 0.5);
}


bool ADrivePawn::SocketCreate()
{
	FIPv4Address ip = FIPv4Address::FIPv4Address(127, 0, 0, 1);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(4484);

	SocketClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	if (SocketClient->Connect(*addr))
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("Connect Succ!"));
		UE_LOG(LogTemp, Warning, TEXT("Connect Succ!"));
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 2.0f, FColor::Green, TEXT("Connect failed!"));
		UE_LOG(LogTemp, Warning, TEXT("Connect failed!"));
		return false;
	}
}

void ADrivePawn::SocketSend(FString meesage)
{
	TCHAR *seriallizedChar = meesage.GetCharArray().GetData();
	int32 size = FCString::Strlen(seriallizedChar) + 1;
	int32 sent = 0;

	if (SocketClient->Send((uint8*)TCHAR_TO_UTF8(seriallizedChar), size, sent))
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send Succ!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send Succ!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send failed!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send failed!"));
	}
}

void ADrivePawn::SocketReceive(bool & bReceive, FString & recvMessage)
{
	recvMessage = "";
	bReceive = false;
	if (!SocketClient)
	{
		return;
	}

	TArray<uint8> ReceiveData;
	uint32 size = 4;
	uint8 element = 0;
	while (SocketClient->HasPendingData(size))
	{
		ReceiveData.Init(element, FMath::Min(size, 65507u));

		int32 read = 0;
		SocketClient->Recv(ReceiveData.GetData(), ReceiveData.Num(), read);

	}

	if (ReceiveData.Num() <= 0)
	{
		return;
	}
	FString log = "Total Data read! num: " + FString::FromInt(ReceiveData.Num());

	GEngine->AddOnScreenDebugMessage(10, 2.0f, FColor::Green, log);
	//UE_LOG(LogTemp, Warning, TEXT("Recv log:   %s"), *log);

	FString ReceivedUE4String = (ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData())));

	ReceivedUE4String = ReceivedUE4String.Left(ReceiveData.Num());
	log = "Server:" + ReceivedUE4String;
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, log);
	//UE_LOG(LogTemp, Warning, TEXT("*** %s"), *log);

	recvMessage = ReceivedUE4String;

	bReceive = true;
}