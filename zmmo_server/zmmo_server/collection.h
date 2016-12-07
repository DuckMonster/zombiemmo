#pragma once
#include <array>

template<typename T>
class CCollection {
private:
	typedef std::array<T*, 1024> t_array;

public:
	CCollection( );

	size_t GetNextID( );
	size_t Add( T* obj ) { return Add( GetNextID( ), obj ); }
	size_t Add( size_t id, T* obj );
	void Remove( size_t id );
	size_t Size( ) { return m_TArray.size( ); }

	template<typename T>
	T* Get( size_t index ) { return dynamic_cast<T*>(m_TArray[index]); }

	T* operator[]( size_t id );

private:
	t_array m_TArray;
};

template<typename T>
CCollection<T>::CCollection( ) {
	for (size_t i = 0; i < m_TArray.size( ); i++) {
		m_TArray[i] = NULL;
	}
}

template<typename T>
size_t CCollection<T>::GetNextID( ) {
	for (size_t i = 0; i < m_TArray.size( ); i++) {
		if (m_TArray[i] == NULL)
			return i;
	}

	return -1;
}

template<typename T>
inline size_t CCollection<T>::Add( size_t id, T * obj ) {
	if (id == -1 || m_TArray[id] != NULL)
		throw std::exception( "Failed to spawn entity" );

	m_TArray[id] = obj;
	return id;
}

template<typename T>
void CCollection<T>::Remove( size_t id ) {
	if (m_TArray[id] == NULL)
		return;

	delete m_TArray[id];
	m_TArray[id] = NULL;
}

template<typename T>
T* CCollection<T>::operator[]( size_t id ) {
	return m_TArray[id];
}