#ifndef TOWERDEFENCE_MAP_H
#define TOWERDEFENCE_MAP_H
#include <utility>

#include "Serializable.h"
#include "TowerSample.h"
#include "WaveSample.h"

class QWidget;

class Map : protected ISerializable {
public:
	using json = nlohmann::json;

	Map(std::string name, const int height, const int width);

	explicit Map(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	int getTile(int x, int y) const;

	void setTile(int x, int y, const int tileId);

	std::vector<std::vector<int> > &getTiles();

	[[nodiscard]] std::string getName();

	void setName(const std::string &n);

	int getHeight() const;

	int getWidth() const;

	std::vector<std::shared_ptr<WaveSample> > &getWaves() { return waves; }

	std::vector<std::shared_ptr<TowerSample> > &getSpots() { return spots; }

	double getHp();

	void setHp(double hp);

	double getStartCurrency();

	void setStartCurrency(double currency);

private:
	std::string name;
	int height{0};
	int width{0};
	double startCurrency{0};
	double hp{0};

	std::vector<std::shared_ptr<WaveSample> > waves;
	std::vector<std::shared_ptr<TowerSample> > spots;
	std::vector<std::vector<int> > tiles;
};

#endif // TOWERDEFENCE_MAP_H
