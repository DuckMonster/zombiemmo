#pragma once
namespace ui {
	struct Orientation {
		int m_Left, m_Top, m_Right, m_Bottom;

		Orientation( ) :
			m_Left( 0 ), m_Top( 0 ), m_Right( 0 ), m_Bottom( 0 ) { }

		Orientation( int left, int top, int right, int bottom ) :
			m_Left( left ), m_Top( top ), m_Right( right ), m_Bottom( bottom ) { }
	};

	enum AnchorType {
		Relative,
		Absolute
	};

	class CUIElement {
	public:
		CUIElement( ) :
			CUIElement( glm::ivec2( 0 ), glm::ivec2( 0 ), NULL ) { }
		CUIElement( glm::ivec2 position, glm::ivec2 size ) :
			CUIElement( position, size, NULL ) { }
		CUIElement( CUIElement* parent ) :
			CUIElement( glm::ivec2( 0 ), glm::ivec2( 0 ), parent ) { }
		CUIElement( glm::ivec2 position, glm::ivec2 size, CUIElement* parent );

		virtual ~CUIElement( );

		virtual void AttachTo( CUIElement* parent );

		void SetVisible( bool visible ) { m_Visible = visible; }
		bool IsVisible( ) { return m_Visible; }

		glm::ivec2 GetPosition( ) { return m_Position; }
		virtual void SetPosition( glm::ivec2 position );
		glm::ivec2 GetSize( ) { return m_Size; }
		virtual void SetSize( glm::ivec2 size );

		AnchorType GetAnchor( ) { return m_Anchor; }
		void SetAnchor( AnchorType anchor );

		Orientation GetOrientation( ) { return m_Orientation; }
		void SetOrientation( Orientation& orientation );

		CUIElement* GetParent( ) { return m_Parent; }

		virtual void Render( float delta );

	protected:
		void SetDirty( );
		void Clean( ) { m_Dirty = false; }
		bool IsDirty( ) { return m_Dirty; }

		virtual void Build( );

		glm::ivec2 GetUIPosition( );
		void AttachChild( CUIElement* child );
		void DetachChild( CUIElement* child );

		void BuildOrientation( );
		void ApplyOrientation( );

	private:
		glm::ivec2					m_Position;
		glm::ivec2					m_Size;

		AnchorType					m_Anchor;
		Orientation					m_Orientation;

		CUIElement*					m_Parent;
		std::vector<CUIElement*>	m_Children;

		bool						m_Dirty;
		bool						m_Visible;
	};
}