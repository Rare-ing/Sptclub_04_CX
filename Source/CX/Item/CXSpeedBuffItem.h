// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXSpeedBuffItem.generated.h"

class USceneComponent;
class UPrimitiveComponent;
class USphereComponent;
class USoundBase;
class UParticleSystem;

UCLASS()
class CX_API ACXSpeedBuffItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACXSpeedBuffItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OhterActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPickupEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<USphereComponent> OverlapSphere;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Coin")
	TObjectPtr<UParticleSystem> PickupParticle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Coin")
	TObjectPtr<USoundBase> PickupSound;

};
