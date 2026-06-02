#pragma once
#include "../../../../../editor/include/Entity/Player.h"
namespace TDEngine {
	namespace Inner {
		class EnginePlayer : public Player{
		public:
			enum Status { IDLE, PLAYING, LOST, WON };
			explicit EnginePlayer(Player sample)
			: Player(std::move(sample)) {}

			double currentCurrency;
			double currentHp;
			std::shared_ptr<Team> team;
			Status status;
		};
	} // Inner
} // TDEngine