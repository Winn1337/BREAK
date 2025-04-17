#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GRAPPLINGHOOK_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrappleComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireGrapple();
	void ReleaseGrapple();

protected:
	// The Grapple Blueprint to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Grapple")
	TSubclassOf<AActor> GrappleClass;

	// How Strong the swingforce is left/right
	UPROPERTY(EditAnywhere, Category = "Swing")
	float SwingForce = 3000.f;

private:
	// Reference to the spawned grapple actor
	AActor* ActiveGrapple = nullptr;

	UPROPERTY()
	bool bIsGrappleHooked = false;
};
