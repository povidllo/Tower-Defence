#ifndef TOWERDEFENCE_WAVECHAIN_H
#define TOWERDEFENCE_WAVECHAIN_H

#include <string>
#include <vector>

#include "Serializable.h"

class WaveChain : public ISerializable {
public:
	using json = nlohmann::ordered_json;

	WaveChain() = default;
	explicit WaveChain(const json &j) { fromJson(j); }

	json toJson() const override;
	void fromJson(const json &j) override;

	const std::vector<std::string> &getChain() const { return chain; }
	std::vector<std::string> &getChain() { return chain; }

	const std::vector<double> &getDelays() const { return delays; }
	std::vector<double> &getDelays() { return delays; }

	void setChain(std::vector<std::string> waves);
	void setDelays(std::vector<double> values);

	void addStep(const std::string &waveName, double delayBefore);
	void removeStep(size_t index);
	void removeWaveReference(const std::string &waveName);
	void renameWaveReference(const std::string &oldName, const std::string &newName);

private:
	void normalizeDelays();

	std::vector<std::string> chain;
	std::vector<double> delays;
};

#endif // TOWERDEFENCE_WAVECHAIN_H
