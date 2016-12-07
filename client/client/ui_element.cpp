#include "stdafx.h"

#include "ui_element.h"

ui::CUIElement::CUIElement( glm::ivec2 position, glm::ivec2 size, CUIElement * parent ) :
	m_Position( position ), m_Size( size ),
	m_Anchor( ui::Relative ), m_Orientation( ),
	m_Parent( NULL ), m_Children( ),
	m_Dirty( false ), m_Visible( true ) {

	AttachTo( parent );
}

ui::CUIElement::~CUIElement( ) {
	AttachTo( NULL );
}

void ui::CUIElement::AttachTo( CUIElement * parent ) {
	if (parent == this)
		throw std::exception( "Trying to attach element to itself" );

	if (m_Parent)
		m_Parent->DetachChild( this );

	m_Parent = parent;
	if (m_Parent)
		m_Parent->AttachChild( this );

	SetDirty( );
}

void ui::CUIElement::SetPosition( glm::ivec2 position ) {
	if (m_Position == position)
		return;

	m_Position = position;
	BuildOrientation( );
}
void ui::CUIElement::SetSize( glm::ivec2 size ) {
	if (m_Size == size)
		return;

	m_Size = size;
	BuildOrientation( );
	SetDirty( );
}

void ui::CUIElement::SetAnchor( AnchorType anchor ) {
	m_Anchor = anchor;
	SetDirty( );
}

void ui::CUIElement::SetOrientation( Orientation & orientation ) {
	m_Orientation = orientation;
	Build( );
}

void ui::CUIElement::Render( float delta ) {
	if (!IsVisible( ))
		return;

	for (CUIElement* child : m_Children)
		child->Render( delta );
}

void ui::CUIElement::SetDirty( ) {
	m_Dirty = true;

	for (CUIElement* child : m_Children)
		child->SetDirty( );
}

glm::ivec2 ui::CUIElement::GetUIPosition( ) {
	glm::ivec2 pos = m_Position;

	if (m_Parent)
		pos += m_Parent->GetUIPosition( );

	return pos;
}

void ui::CUIElement::AttachChild( CUIElement * child ) {
	std::vector<CUIElement*>::iterator i = std::find( m_Children.begin( ), m_Children.end( ), child );
	if (i != m_Children.end( ))
		return;

	m_Children.push_back( child );
}

void ui::CUIElement::DetachChild( CUIElement * child ) {
	std::vector<CUIElement*>::iterator i = std::find( m_Children.begin( ), m_Children.end( ), child );
	if (i == m_Children.end( ))
		return;

	m_Children.erase( i );
}

void ui::CUIElement::Build( ) {
	ApplyOrientation( );
	Clean( );
}

void ui::CUIElement::BuildOrientation( ) {
	if (!GetParent( ))
		return;

	m_Orientation.m_Left = GetPosition( ).x;
	m_Orientation.m_Top = GetPosition( ).y;
	m_Orientation.m_Right = GetParent( )->GetSize( ).x - GetSize( ).x - GetPosition( ).x;
	m_Orientation.m_Bottom = GetParent( )->GetSize( ).y - GetSize( ).y - GetPosition( ).y;
}

void ui::CUIElement::ApplyOrientation( ) {
	if (!GetParent( ) || m_Anchor == Absolute)
		return;

	glm::ivec2 pSize = GetParent( )->GetSize( );

	m_Position = glm::ivec2( m_Orientation.m_Left, m_Orientation.m_Top );
	m_Size = glm::ivec2( pSize.x - m_Orientation.m_Right - m_Orientation.m_Left, pSize.y - m_Orientation.m_Bottom - m_Orientation.m_Top );

	Clean( );
}
