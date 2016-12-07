#include "stdafx.h"

#include "entity.h"
#include "game.h"
#include "shadow.h"

#include "collision.h"
#include "tiletext.h"

CEntity::CEntity( CGame* game, size_t id ) :
	m_Game( game ), m_ID( id ), m_Collision( collision::Solid | collision::Visibility ),
	m_VisiblePosition( 0 ), m_Visibility( ENone ) {

	SetSprite( TileData( GetTileChar( '?' ) ) );
}

void CEntity::Update( float delta ) {
}

void CEntity::Render( float delta ) {
	if (m_Visibility == EKnown)
		m_Drawer.Render( m_Position );
	else if (m_Visibility == ELast)
		m_Drawer.Render( m_VisiblePosition );
}

void CEntity::Destroy( ) {
	GetGame( )->DestroyEntity( this );
}

void CEntity::SetPosition( glm::ivec2 position ) {
	m_Position = position;
	UpdateVisibility( );
}

void CEntity::UpdateVisibility( ) {
	if (m_Visibility == EKnown) {
		if (!GetGame( )->GetShadowController( )->IsVisible( m_Position )) {
			m_VisiblePosition = GetPosition( );
			m_Visibility = ELast;
		}
	}
	else if (m_Visibility == ELast) {
		if (GetGame( )->GetShadowController( )->IsVisible( m_Position ))
			m_Visibility = EKnown;
		else if (GetGame( )->GetShadowController( )->IsVisible( m_VisiblePosition ))
			m_Visibility = ENone;
	}
	else if (m_Visibility == ENone) {
		if (GetGame( )->GetShadowController( )->IsVisible( m_Position )) {
			m_Visibility = EKnown;
		}
	}
}

CGame * const CEntity::GetGame( ) {
	return m_Game;
}

void CEntity::SetSprite( TileData data ) {
	GetDrawer( ).Clear( );
	GetDrawer( ).AddTile( data );
}