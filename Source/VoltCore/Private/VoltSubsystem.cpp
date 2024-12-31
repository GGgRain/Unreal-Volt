// Fill out your copyright notice in the Description page of Project Settings.


#include "VoltSubsystem.h"
#include "VoltAnimationManager.h"
#include "VoltModuleRunnable.h"
#include "VoltProxy.h"
#include "VoltSettings.h"
#include "Engine/Engine.h"


UVoltSubsystem::UVoltSubsystem()
{
	CacheCleanUpInterval();
	
	PopulateModuleUpdateThreadIfNeeded();

	//We do not start the tick with it
	if(this != UVoltSubsystem::StaticClass()->GetDefaultObject())
	{
		BindOnSlateApplicationPreTick();
	}

	SetUtilizingMultithreading(UVoltSettings::Get() ? UVoltSettings::Get()->bUseMultithreadingOnModuleUpdate : false);
}

UVoltSubsystem::~UVoltSubsystem()
{
	ReleaseModuleUpdateThread();

	//We do not start the tick with it
	if(this != UVoltSubsystem::StaticClass()->GetDefaultObject())
	{
		UnbindOnSlateApplicationPreTick();
	}
}

void UVoltSubsystem::RegisterAnimationManager(UVoltAnimationManager* AnimationManager)
{

	if(RegisteredAnimationManager.Contains(AnimationManager)) return;

	RegisteredAnimationManager.Add(AnimationManager);
	
	if(IsUtilizingMultithreading()) AddAnimationManagerOnModuleUpdateThread(AnimationManager);	

}

void UVoltSubsystem::UnregisterAnimationManager(UVoltAnimationManager* AnimationManager)
{
	if(!RegisteredAnimationManager.Contains(AnimationManager)) return;

	RegisteredAnimationManager.Remove(AnimationManager);
	
	if(IsUtilizingMultithreading()) RemoveAnimationManagerOnModuleUpdateThread(AnimationManager);	

}

void UVoltSubsystem::TryDiscardAbandonedInstancesByInterval()
{
	if(const float& CurTime = FPlatformTime::Seconds(); CurTime - CleanUpLastTime > CleanUpInterval)
	{
		DestructUnnecessaryRegisteredAnimationManager();
		DiscardInvalidVoltInterfaces();

		CleanUpLastTime = CurTime;
	}
}

void UVoltSubsystem::UpdateAnimations(float DeltaTime)
{
	
	if(IsUtilizingMultithreading())
	{
		ModuleUpdateThread->TriggerTask(DeltaTime);
	}

	for (UVoltAnimationManager* AnimationManager : RegisteredAnimationManager)
	{
		if(AnimationManager == nullptr) continue;
			
		AnimationManager->Tick(DeltaTime);
	}

	//UVolt_ASM_InterpColor* ColorInModule = ObjectInitializer.CreateDefaultSubobject<UVolt_ASM_InterpColor>(this,"ColorInterpIn");
	//Modules.Add(ColorInModule);
}

const bool UVoltSubsystem::IsModuleUpdateThreadWorking() const
{
	// Should ensure the validity of the thread either.
	const UVoltSubsystem* Subsystem = Get();
	
	return Subsystem && Subsystem->ModuleUpdateThread.IsValid() && Subsystem->ModuleUpdateThread->IsWorking();
}

void UVoltSubsystem::AddAnimationManagerOnModuleUpdateThread(UVoltAnimationManager* AnimationManager)
{
	if(!IsUtilizingMultithreading()) return;

	if(!AnimationManager) return;
	
	ModuleUpdateThread->AddAnimationManager(AnimationManager);
}

void UVoltSubsystem::RemoveAnimationManagerOnModuleUpdateThread(UVoltAnimationManager* AnimationManager)
{
	if(!IsUtilizingMultithreading()) return;

	if(!AnimationManager) return;

	ModuleUpdateThread->RemoveAnimationManager(AnimationManager);
}

void UVoltSubsystem::PopulateModuleUpdateThreadIfNeeded()
{
	ReleaseModuleUpdateThread();

	//We do not create a thread on the CDO.
	if(this == UVoltSubsystem::StaticClass()->GetDefaultObject()) return;

	//fallback
	if(!bIsUtilizingMultiThread || ModuleUpdateThread != nullptr) return;

	ModuleUpdateThread = MakeUnique<FVoltModuleRunnable>();

}

void UVoltSubsystem::ReleaseModuleUpdateThread()
{
	if(ModuleUpdateThread == nullptr) return;

	// Stop the thread.
	ModuleUpdateThread->Stop();
}

const bool UVoltSubsystem::IsUtilizingMultithreading() const
{
	return bIsUtilizingMultiThread && ModuleUpdateThread != nullptr;
}

void UVoltSubsystem::SetUtilizingMultithreading(const bool bNewMultithreading)
{
	if(bIsUtilizingMultiThread == bNewMultithreading) return;
	
	if(bNewMultithreading) PopulateModuleUpdateThreadIfNeeded();
	else ReleaseModuleUpdateThread();

	bIsUtilizingMultiThread = bNewMultithreading;
}


void UVoltSubsystem::DestructUnnecessaryRegisteredAnimationManager()
{
	RegisteredAnimationManager.RemoveAll([](UVoltAnimationManager* AnimationManager)
	{
		if(AnimationManager != nullptr)
		{
			if(!AnimationManager->CheckShouldDestruct()) return false;

			AnimationManager->DestructSelf();
		}
		
		return true;
	});
}

void UVoltSubsystem::AssignVoltInterface(const TScriptInterface<IVoltInterface> VoltInterfaceToAssign)
{
	if(HasVoltInterface(VoltInterfaceToAssign)) return;

	VoltInterfaces.Add(VoltInterfaceToAssign->GetTargetSlate().Pin().Get()->GetId(),VoltInterfaceToAssign);
}

TScriptInterface<IVoltInterface> UVoltSubsystem::FindVoltInterfaceFor(const TWeakPtr<SWidget>& SlateToFind)
{

	if(!SlateToFind.IsValid()) return nullptr;
	
	const FVoltInterfaceElement* FoundElem = VoltInterfaces.Find(SlateToFind.Pin()->GetId());
	
	return FoundElem != nullptr ? FoundElem->VoltInterface : nullptr;
}

TScriptInterface<IVoltInterface> UVoltSubsystem::FindOrAssignVoltInterfaceFor(const TWeakPtr<SWidget>& SlateToFind)
{
	if (SlateToFind == nullptr) return nullptr;
	
	if (const TScriptInterface<IVoltInterface> FoundInterface = FindVoltInterfaceFor(SlateToFind)) return FoundInterface;

	//None present, must create a new one

	UVoltProxy* Proxy = NewObject<UVoltProxy>(this);

	Proxy->Widget = SlateToFind;

	FVoltInterfaceElement Elem = FVoltInterfaceElement(Proxy);

	VoltInterfaces.Add(Elem.VoltInterface->GetTargetSlate().Pin()->GetId(), Elem.VoltInterface);
	
	return Proxy;
}

bool UVoltSubsystem::HasVoltInterface(const TScriptInterface<IVoltInterface>& VoltInterfaceToCheck) const
{
	return VoltInterfaces.Contains(VoltInterfaceToCheck->GetTargetSlate().Pin().Get()->GetId());
}

bool UVoltSubsystem::HasVoltInterfaceFor(const TWeakPtr<SWidget>& SlateToCheck) const
{
	return VoltInterfaces.Contains(SlateToCheck.Pin().Get()->GetId());
}

int UVoltSubsystem::GetVoltInterfaceCount() const
{
	return VoltInterfaces.Num();
}

void UVoltSubsystem::DiscardInvalidVoltInterfaces()
{
	
	TArray<uint64> Keys;

	for (const TTuple<uint64, FVoltInterfaceElement>& VoltInterface : VoltInterfaces)
	{
		if(!VoltInterface.Value.IsValid()) Keys.Add(VoltInterface.Key);
	}

	for (const uint64& Key : Keys)
	{
		VoltInterfaces.Remove(Key);
	}
}

void UVoltSubsystem::CacheCleanUpInterval()
{
	CleanUpInterval = UVoltSettings::Get()->VoltSubsystemCleanUpInterval;
}

void UVoltSubsystem::OnSlateApplicationPreTick(float DeltaTime)
{
	//do animation things;
	UpdateAnimations(DeltaTime);

	TryDiscardAbandonedInstancesByInterval();
}

void UVoltSubsystem::BindOnSlateApplicationPreTick()
{
	if(bIsTicking) return;
	
	if(FSlateApplication* SlateApplication = &FSlateApplication::Get())
	{
		SlateApplication->OnPreTick().AddUObject(this, &UVoltSubsystem::OnSlateApplicationPreTick);

		bIsTicking = true;

		//Make it unbind itself.
		SlateApplication->OnPreShutdown().AddUObject(this, &UVoltSubsystem::UnbindOnSlateApplicationPreTick);
	}
}

void UVoltSubsystem::UnbindOnSlateApplicationPreTick()
{
	if(!bIsTicking) return;

	if(FSlateApplication* SlateApplication = &FSlateApplication::Get())
	{
		SlateApplication->OnPreTick().RemoveAll(this);

		bIsTicking = false;
	}
}

UVoltSubsystem* UVoltSubsystem::Get()
{

	if(GEngine)
	{
		if(UEngineSubsystem* Subsystem = GEngine->GetEngineSubsystemBase(UVoltSubsystem::StaticClass())) return Cast<UVoltSubsystem>(Subsystem);
	}
	return nullptr;
}
