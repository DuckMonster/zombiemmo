#pragma once

class Timer {
public:
	Timer( ) : Timer( 0.f ) { }
	Timer( float duration, float value = 0.f ) : m_Duration( duration ), m_Value( value ) { }

	void Update( float delta ) { if (m_Duration == 0) return; m_Value = glm::clamp( m_Value + (delta / m_Duration), 0.f, 1.f ); }
	bool Finished( ) { return m_Duration == 0 || m_Value >= 1.f; }
	void Reset( ) { Reset( m_Duration ); }
	void Reset( float duration ) { m_Value = 0.f; m_Duration = duration; }

	float Sub( float start, float duration ) {
		float scaled = m_Value * m_Duration;
		return glm::clamp( (scaled - start) / duration, 0.f, 1.f );
	}

	float	m_Value;

private:
	float	m_Duration;
};