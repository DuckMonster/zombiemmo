#pragma once
#include "ui_pane.h"
#include "ui_textbox.h"
#include "ui_sprite.h"
#include "inventory.h"

namespace ui {
	class CUIInventory {
	private:
		class UIItem {
		public:
			UIItem( int index );

			void Init( const Item& item, CUIPane* pane );
			void SetSelected( bool selected );

		private:
			int			m_Index;
			Item		m_Data;
			CUITextBox	m_TextBox;
			CUISprite	m_Sprite;
		};

	public:
		CUIInventory( );
		~CUIInventory( );

		void SetOpen( bool open ) { m_Base.SetVisible( open ); }
		bool IsOpen( ) { return m_Base.IsVisible( ); }

		void Build( CInventory* inventory );
		int GetSelected( ) { return m_SelectedItem; }

		void Render( float delta );
		void Select( int index );
		void ScrollSelected( int delta );

	private:
		void AddItem( const Item& item );
		void Clear( );

		CUIPane					m_Base;
		CUIPane					m_ItemBase;
		CUITextBox				m_Title;
		
		std::vector<UIItem*>	m_Items;

		int						m_SelectedItem;
	};
}