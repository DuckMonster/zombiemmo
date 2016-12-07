#pragma once
#include "actor.h"
#include "ui_inventory.h"
#include "timer.h"
#include "inputcontroller.h"
#include "deathdarkness.h"

class CPlayer : public CActor, IInputReceiver {
private:
	enum Mode {
		ENone,
		EInventory,
		EAiming,
		EExiting
	};

	static const float INPUT_REPEAT_SPEED;

public:
	CPlayer( CGame* game, size_t id );

	void Move( direction::Direction dir ) override;
	void HandleInput( sf::Keyboard::Key key, int state );

	void PickupItem( CItemDrop* itemEntity ) override;
	void DropItem( const size_t index ) override;

	void SetPosition( glm::ivec2 position ) override;

	void Update( float delta ) override;
	void Render( float delta ) override;

	void RenderGameUI( float delta );
	void RenderUI( float delta);

protected:
	bool IsLocal( );

	void OpenInventory( );
	void CloseInventory( );

	void SendMove( direction::Direction dir );
	void SendFire( glm::ivec2 target );
	void SetMode( Mode mode );

	// INPUT STUFF
	void InputTick( sf::Keyboard::Key k ) override;
	void InputReleased( sf::Keyboard::Key k ) override;
	float GetInputTickFrequency( ) override;
	bool GetInputTopOnly( ) override { return m_Mode == ENone; }
	//

	void BuildAim( );
	glm::vec2 AimDirection( ) { return glm::normalize( glm::vec2( m_CursorPosition ) ); }

	void OnDeath( ) override;

private:
	Mode				m_Mode;

	CInputController	m_InputController;

	glm::ivec2			m_CursorPosition;
	ui::CUIInventory	m_InventoryUI;

	CTileDrawer			m_CursorDrawer;
	CTileDrawer			m_AimDrawer;
	CTileDrawer			m_AimTargetDrawer;

	Timer				m_DeathAnimation;
	CDeathDarkness		m_DeathDarkness;
};