#include "stdafx.h"
using namespace glm;
using namespace std;

vec3		math::rotateVector( vec3 vector, vec3 axis, float angle ) {
	// Rotation matrix
	// Converting first b vec4 for matrix multiplication, then converted back b vec3



	return vec3( (rotate( mat4( 1.f ), angle, axis ) * vec4( vector, 1.f )) );
	//return vec3( angleAxis( radians(angle), axis ) * vec4( vector, 1.f ) );
}

vec3		math::circleTangent( vec3 a, vec3 b ) {
	if (a == b)
		return vec3( 0.f, 1.f, 0.f );

	// Direct direction between the two points
	vec3	forward = normalize( b - a );
	vec3	up = normalize( b + a );

	// Great Circle tangent between the two points
	vec3	tangent = normalize( cross( up, forward ) );

	// Tangent length is 0, which mean the two points are on the opposite sides of the sphere
	if (tangent.length( ) <= 0.001f)
		return vec3( 0.f, 1.f, 0.f );

	else
		return tangent;
}

float		math::speedToAngle( float speed, float radius ) {
	// Get the circle radius
	float		circumference = radius * 2 * pi<float>( );

	// Speed / Circumference ratio
	float		traveledUnits = speed / circumference;

	// Convert b radians
	return traveledUnits * 2 * pi<float>( );
}

float		math::vecToAng( vec2 vector ) {
	return		atan2( vector.y, vector.x );
}
float		math::vecToAngD( vec2 vector ) { return degrees( vecToAng( vector ) ); }

vec2		math::angToVec( float angle ) {
	return		vec2( cos( angle ), sin( angle ) );
}
vec2		math::angToVecD( float angle ) { return angToVec( radians( angle ) ); }

glm::vec2 math::lerp( glm::vec2 a, glm::vec2 b, float f ) {
	return a + (b - a) * glm::clamp( f, 0.f, 1.f );
}

glm::vec3 math::lerp( glm::vec3 a, glm::vec3 b, float f ) {
	return a + (b - a) * glm::clamp( f, 0.f, 1.f );
}

glm::ivec2 math::inormalize( glm::ivec2 v ) {
	if (abs( v.x ) > abs( v.y ))
		return glm::ivec2( sign( v.x ), 0 );
	else
		return glm::ivec2( 0, sign( v.y ) );
}

int math::ilength( glm::ivec2 v ) {
	return glm::abs( v.x ) + glm::abs( v.y );
}

glm::ivec2 math::dirToVec( direction::Direction dir ) {
	switch (dir) {
		case direction::East: return glm::ivec2( 1, 0 );
		case direction::North: return glm::ivec2( 0, -1 );
		case direction::West: return glm::ivec2( -1, 0 );
		case direction::South: return glm::ivec2( 0, 1 );
		default: return glm::ivec2( 0 );
	}
}

direction::Direction math::vecToDir( glm::ivec2 v ) {
	v = inormalize( v );

	if (v.y == 0) {
		return v.x > 0 ? direction::East : direction::West;
	}
	else {
		return v.y > 0 ? direction::South : direction::North;
	}
}

//RANDOM
// Engine
namespace {
	default_random_engine randomEngine( std::random_device{ }() );
}

float		random::frand( ) { return random::frand( 0.f, 1.f ); }
float		random::ufrand( ) { return random::frand( -1.f, 1.f ); }
float		random::frand( float max ) { return random::frand( 0.f, max ); }
float		random::frand( float min, float max ) {
	uniform_real_distribution<float>		distribution( min, max );
	return		distribution( randomEngine );
}

int		    random::rand( ) {
	return		random::rand( 0, INT_MAX );
}

int		    random::rand( int max ) {
	return		random::rand( 0, max );
}

int		    random::rand( int min, int max ) {
	uniform_int_distribution<int>			distribution( min, max );
	return		distribution( randomEngine );
}

bool        random::chance( float percent ) {
	return frand( ) <= glm::clamp( percent, 0.f, 1.f );
}

glm::ivec2  random::vrand( glm::ivec2 min, glm::ivec2 max ) {
	return glm::ivec2( rand( min.x, max.x ), rand( min.y, max.y ) );
}

vec2		random::circle( ) {
	return		math::angToVec( random::frand( pi<float>( ) * 2.f ) );
}

vec3		random::sphere( ) {
	return		normalize( vec3( random::ufrand( ), random::ufrand( ), random::ufrand( ) ) );
}