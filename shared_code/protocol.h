#pragma once
namespace protocol {
	enum MMO {
		Ping,

		EntitySpawn,
		EntityDestroy,
		EntityPosition,

		ActorSpawn,
		ActorHealth,
		ActorMove,
		ActorFire,
		ActorProjectileHit,

		PlayerPossess
	};
}

namespace entity {
	enum Type {
		Base,
		Actor,
		Player,
		Mob,
		ItemDrop
	};
}

namespace direction {
	enum Direction {
		East,
		North,
		West,
		South
	};
}