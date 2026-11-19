////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-data-view-extensions, a DLL Plugin for
// SimCity 4 that extends the game's data views.
//
// Copyright (c) 2024 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////

#include "DeathcareFilter.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cISC4Occupant.h"
#include "cIGZVariant.h"

namespace
{
	constexpr uint32_t OccupantType_Building = 0x278128A0;
	constexpr uint32_t OccupantGroupsPropertyId = 0xAA1DD396;
	// SC4SIM deathcare type (1 cemetery, 2 crematorium), from New_Properties.xml-patches.
	constexpr uint32_t DeathcareTypePropertyId = 0x5C4DD001;
	constexpr uint32_t OccupantGroup_MaxisCemetery = 0x1700;
	constexpr uint32_t OccupantGroup_Crematorium = 0x5C4DD100;
	constexpr uint32_t OccupantGroup_Cemetery = 0x5C4DD101;
}

bool DeathcareFilter::IsOccupantIncluded(cISC4Occupant* pOccupant)
{
	if (!pOccupant || pOccupant->GetType() != OccupantType_Building)
	{
		return false;
	}

	const cISCPropertyHolder* holder = pOccupant->AsPropertyHolder();

	if (!holder)
	{
		return false;
	}

	if (holder->HasProperty(DeathcareTypePropertyId))
	{
		return true;
	}

	const cISCProperty* property = holder->GetProperty(OccupantGroupsPropertyId);
	const cIGZVariant* value = property ? property->GetPropertyValue() : nullptr;

	if (value && value->GetType() == cIGZVariant::Uint32Array)
	{
		const uint32_t* groups = value->RefUint32();
		const uint32_t count = value->GetCount();

		for (uint32_t i = 0; i < count; i++)
		{
			if (groups[i] == OccupantGroup_MaxisCemetery
				|| groups[i] == OccupantGroup_Crematorium
				|| groups[i] == OccupantGroup_Cemetery)
			{
				return true;
			}
		}
	}

	return false;
}
