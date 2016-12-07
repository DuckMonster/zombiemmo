#pragma once
#include "ui_textbox.h"

class CGame;

namespace ui {
	class CUILog {
	public:
		static size_t LOG_LENGTH;

	private:
		class Entry {
		public:
			Entry( std::string text, CUIElement* base );

			void Update( float update );
			void SetIndex( int index );
			void Tick( );

		private:
			const std::string	m_Text;
			CUITextBox			m_TextBox;
			int					m_Tick;
		};

	public:
		CUILog( CGame* game );
		~CUILog( );

		void Add( std::string msg );
		void Update( float delta );
		void Render( float delta );
		void Tick( );

	private:
		CGame* const			m_Game;

		std::vector<Entry*>		m_Entries;
		CUIElement				m_Base;
	};
}