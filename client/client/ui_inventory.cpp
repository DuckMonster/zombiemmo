#include "stdafx.h"
#include "ui_inventory.h"

ui::CUIInventory::CUIInventory( ) :
	m_Base( ), m_Items( ), m_SelectedItem( 0 ) {

	m_Base.m_Color = color( 0.f, 0.f, 0.f, 0.6f );
	m_Base.SetPosition( glm::ivec2( 2, 2 ) );
	m_Base.SetSize( glm::ivec2( 25, 40 ) );
	m_Base.SetVisible( false );

	m_ItemBase.AttachTo( &m_Base );
	m_ItemBase.SetOrientation( Orientation( 2, 2, 2, 2 ) );

	m_Title.AttachTo( &m_Base );
	m_Title.SetOrientation( Orientation( ) );
	m_Title.SetText( "---Inventory---" );
	m_Title.SetAlignment( CUITextBox::Center );
	m_Title.m_Foreground = color( 0.f, 0.f, 0.f, 1.f );
	m_Title.m_Background = color( 1.f );
}

ui::CUIInventory::~CUIInventory( ) {
	Clear( );
}

void ui::CUIInventory::Build( CInventory * inventory ) {
	Clear( );

	for (const Item& i : inventory->GetItems( ))
		AddItem( i );
}

void ui::CUIInventory::AddItem( const Item& item ) {
	int index = m_Items.size( );

	UIItem* elem = new UIItem( index );
	elem->Init( item, &m_ItemBase );

	m_Items.push_back( elem );
}

void ui::CUIInventory::Clear( ) {
	for (UIItem* item : m_Items)
		delete item;

	m_Items.clear( );
	m_SelectedItem = -1;
}

void ui::CUIInventory::Render( float delta ) {
	m_Base.Render( delta );
}

void ui::CUIInventory::Select( int index ) {
	if (m_Items.size( ) == 0)
		return;

	if (m_SelectedItem != -1)
		m_Items[m_SelectedItem]->SetSelected( false );

	m_Items[index]->SetSelected( true );
	m_SelectedItem = index;
}

void ui::CUIInventory::ScrollSelected( int delta ) {
	if (m_Items.size( ) == 0)
		return;

	Select( math::mod<int>( m_SelectedItem + delta, 0, m_Items.size( ) ) );
}

ui::CUIInventory::UIItem::UIItem( int index ) :
	m_Index( index ), m_Data( ), m_TextBox( ) {
}

void ui::CUIInventory::UIItem::Init( const Item& item, CUIPane* pane ) {

	m_TextBox.AttachTo( pane );
	m_TextBox.SetPosition( glm::ivec2( 2, m_Index ) );
	m_TextBox.SetText( item.m_Name );

	m_Sprite.AttachTo( pane );
	m_Sprite.SetPosition( glm::ivec2( 0, m_Index ) );
	m_Sprite.SetIndex( item.m_Icon );
}

void ui::CUIInventory::UIItem::SetSelected( bool selected ) {
	m_TextBox.m_Foreground = selected ? color( 0.f, 0.f, 0.f, 1.f ) : color( 1.f );
	m_TextBox.m_Background = selected ? color( 1.f ) : color( 0.f );
}
