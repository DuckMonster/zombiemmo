#include "stdafx.h"
#include "component.h"

CComponent::CComponent( CEntity * entity ) : m_Entity( entity ) {
}

CComponent::~CComponent( ) {
}

CEntity * const CComponent::GetEntity( ) { return m_Entity; }
CGame * const CComponent::GetGame( ) { return m_Entity->GetGame( ); }
