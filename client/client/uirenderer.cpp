#include "stdafx.h"
#include "uirenderer.h"

namespace {
	float t = 0.f;
}

ui::UIRenderer::UIRenderer( ) {
	m_Pane1.SetSize( glm::ivec2( 30, 20 ) );
	m_Pane1.m_Color = color( 1.f );

	m_Pane2.AttachTo( &m_Pane1 );
	m_Pane2.SetOrientation( Orientation( 0, 1, 0, 0 ) );
	m_Pane2.m_Color = color( 0.2, 0.2, 0.2, 1.f );

	m_Divider.AttachTo( &m_Pane2 );
	m_Divider.SetPosition( glm::ivec2( 2, 6 ) );
	m_Divider.SetSize( glm::ivec2( m_Pane2.GetSize( ).x - 4, 1 ) );
	m_Divider.SetAnchor( ui::Absolute );
	m_Divider.m_Color = color( 1.f );

	m_TextBox.AttachTo( &m_Pane2 );
	m_TextBox.SetOrientation( Orientation( 0, 0, 0, 0 ) );
	m_TextBox.SetAlignment( CUITextBox::Center );
	m_TextBox.SetText( "HELLO!\nHow are you this fine day?\nI am fine :)AA" );
	m_TextBox.m_Foreground = color( 0.2f, 0.2f, 0.2f, 1.f );
	m_TextBox.m_Background = color( 1.f );
}

void ui::UIRenderer::Render( float delta ) {
	t += delta * 5.f;

	int a = (int)(t) % 40;
	int b = (int)(t / (5 * 5)) % (5 * 5);

	/*
	m_PDrawer.Quad( glm::ivec2( 2, 2 ), glm::ivec2( 30, 50 ), color( 0.f, 0.f, 0.f, 1.f ) );
	m_PDrawer.Quad( glm::ivec2( 2, 2 ), glm::ivec2( 30, 1 ), color( 1.f ) );
	m_PDrawer.String( "Inventory", glm::ivec2( 6, 2 ), color( 0.f, 0.f, 0.f, 1.f ) );

	m_PDrawer.Quad( glm::ivec2( 3, 4 ), glm::ivec2( 28, 1 ), color( 1.f, 0.3f, 0.f, 1.f ) );
	m_PDrawer.String( "Perfect Handgun", glm::ivec2( 3, 4 ), color( 0.f, 0.f, 0.f, 1.f ) );
	m_PDrawer.String( "E", glm::ivec2( 2, 4 ), color( 0.f, 0.8f, 0.f, 1.f ) );

	m_PDrawer.String( "Broken Handgun", glm::ivec2( 3, 5 ), color( 0.5f, 0.5f, 0.5f, 1.f ) );
	m_PDrawer.String( "----------------------------", glm::ivec2( 3, 7 ), color( 1.f ) );
	m_PDrawer.String( "241 5mm Rounds", glm::ivec2( 3, 9 ), color( 0.8f, 0.8f, 0.8f, 1.f ) );
	m_PDrawer.String( "125 50. Rounds", glm::ivec2( 3, 10 ), color( 0.8f, 0.8f, 0.8f, 1.f ) );
	m_PDrawer.String( "2 Grenades", glm::ivec2( 3, 11 ), color( 0.f, 0.6f, 0.f, 1.f ) );

	m_PDrawer.Quad( glm::ivec2( 33, 2 ), glm::ivec2( 40, 20 ), color( 0.f, 0.f, 0.f, 1.f ) );
	m_PDrawer.String( "Perfect Handgun", glm::ivec2( 34, 3 ), color( 1.f ) );
	m_PDrawer.String( "1.62 kg | 5mm", glm::ivec2( 34, 4 ), color( 0.6f, 0.6f, 0.6f, 1.f ) );
	*/

	//m_SDrawer.Render( );
	//m_PRenderer.Quad( glm::ivec2( 5, 5 ), glm::ivec2( 1, 1 ) );

	m_Pane1.SetPosition( glm::ivec2( a, 0 ) );
	m_Pane1.SetSize( glm::ivec2( 1 + a, 30 ) );
	m_Pane1.Render( delta );
}
