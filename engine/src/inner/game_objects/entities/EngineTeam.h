#pragma once
#include <memory>


#include "../../../../../editor/include/Entity/Team.h"
#include "EnginePlayer.h"
namespace TDEngine {
	namespace Inner {
		class EngineTeam : public Team{
		public:
			explicit EngineTeam(Team sample)
			: Team(std::move(sample)) {}

			std::vector<std::shared_ptr<EnginePlayer>> teamPlayers;
		};
	} // Inner
} // TDEngine