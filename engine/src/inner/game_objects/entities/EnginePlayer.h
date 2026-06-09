#pragma once
#include "../../../../../editor/include/Entity/Player.h"
#include "EngineTeam.h"
namespace TDEngine {
	namespace Inner {
		class EngineTeam;
		class EnginePlayer : public Player{
		public:
			enum Status { IDLE, PLAYING, LOST, WON };
			explicit EnginePlayer(Player sample)
			: Player(std::move(sample)) {}

			double currentCurrency;
			std::shared_ptr<EngineTeam> team;
			Status status;
		};
	} // Inner
} // TDEngine