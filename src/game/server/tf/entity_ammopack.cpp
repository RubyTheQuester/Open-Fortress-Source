//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//
#include "cbase.h"
#include "items.h"
#include "tf_gamerules.h"
#include "tf_shareddefs.h"
#include "tf_player.h"
#include "tf_team.h"
#include "engine/IEngineSound.h"
#include "entity_ammopack.h"

//=============================================================================
//
// CTF AmmoPack defines.
//

#define TF_AMMOPACK_PICKUP_SOUND	"AmmoPack.Touch"

LINK_ENTITY_TO_CLASS( item_ammopack_full, CAmmoPack );
LINK_ENTITY_TO_CLASS( item_ammopack_small, CAmmoPackSmall );
LINK_ENTITY_TO_CLASS( item_ammopack_medium, CAmmoPackMedium );

BEGIN_DATADESC( CAmmoPack )

// Inputs.
DEFINE_KEYFIELD( m_iszModel, FIELD_STRING, "powerup_model" ),

END_DATADESC()

BEGIN_DATADESC( CAmmoPackSmall )

// Inputs.
DEFINE_KEYFIELD( m_iszModel, FIELD_STRING, "powerup_model" ),

END_DATADESC()

BEGIN_DATADESC( CAmmoPackMedium )

// Inputs.
DEFINE_KEYFIELD( m_iszModel, FIELD_STRING, "powerup_model" ),

END_DATADESC()

//=============================================================================
//
// CTF AmmoPack functions.
//

//-----------------------------------------------------------------------------
// Purpose: Spawn function for the ammopack
//-----------------------------------------------------------------------------
void CAmmoPack::Spawn( void )
{
	Precache();
	if (m_iszModel==MAKE_STRING( "" )) SetModel( GetPowerupModel() );
	else SetModel( STRING(m_iszModel) );

	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: Precache function for the ammopack
//-----------------------------------------------------------------------------
void CAmmoPack::Precache( void )
{
	if (m_iszModel==MAKE_STRING( "" )) PrecacheModel( GetPowerupModel() );
	else PrecacheModel( STRING(m_iszModel) );	
	PrecacheScriptSound( STRING(m_iszPickupSound) );
}

//-----------------------------------------------------------------------------
// Purpose: MyTouch function for the ammopack
//-----------------------------------------------------------------------------
bool CAmmoPack::MyTouch( CBasePlayer *pPlayer )
{
	bool bSuccess = false;

	if ( ValidTouch( pPlayer ) )
	{
		CTFPlayer *pTFPlayer = ToTFPlayer( pPlayer );
		if ( !pTFPlayer )
			return false;

		int iMaxPrimary = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_PRIMARY];
		if ( pPlayer->GiveAmmo( ceil(iMaxPrimary * PackRatios[GetPowerupSize()]), TF_AMMO_PRIMARY, true ) )
		{
			bSuccess = true;
		}

		int iMaxSecondary = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_SECONDARY];
		if ( pPlayer->GiveAmmo( ceil(iMaxSecondary * PackRatios[GetPowerupSize()]), TF_AMMO_SECONDARY, true ) )
		{
			bSuccess = true;
		}

		int iMaxMetal = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_METAL];
		if ( pPlayer->GiveAmmo( ceil(iMaxMetal * PackRatios[GetPowerupSize()]), TF_AMMO_METAL, true ) )
		{
			bSuccess = true;
		}

		// did we give them anything?
		if ( bSuccess )
		{
			CSingleUserRecipientFilter filter( pPlayer );
			EmitSound( filter, entindex(), STRING(m_iszPickupSound) );
		}
	}

	return bSuccess;
}
