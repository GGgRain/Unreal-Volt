//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "VoltAnimationTrack.h"

const FVoltAnimationTrack FVoltAnimationTrack::NullTrack = FVoltAnimationTrack(FGuid());

bool FVoltAnimationTrack::IsNullTrack() const
{
	return *this == NullTrack;
}
