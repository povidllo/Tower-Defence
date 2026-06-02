#ifndef TOWERDEFENCE_EFFECTSAMPLE_H
#define TOWERDEFENCE_EFFECTSAMPLE_H

#include <memory>
#include <string>
#include <vector>

#include "Serializable.h"

class EffectSample : public ISerializable {
public:
	using json = nlohmann::ordered_json;

	enum class Kind {
		Enemy,
		Tower
	};

	explicit EffectSample(std::string name, Kind kind);

	~EffectSample() override = default;

	[[nodiscard]] const std::string &getName() const;

	void setName(std::string name);

	[[nodiscard]] Kind getKind() const;

	[[nodiscard]] std::string getKindString() const;

	[[nodiscard]] double getDurationSeconds() const;

	void setDurationSeconds(double value);

	[[nodiscard]] double getPeriodSeconds() const;

	void setPeriodSeconds(double value);

	[[nodiscard]] bool isStackable() const;

	void setStackable(bool value);

	[[nodiscard]] const std::vector<std::string> &getEffectsAfterFinish() const;

	void setEffectsAfterFinish(std::vector<std::string> effects);

	void addEffectAfterFinish(const std::string &name);

	bool removeEffectAfterFinish(const std::string &name);

	[[nodiscard]] const std::string &getVisualTexturePath() const;

	void setVisualTexturePath(std::string path);

protected:
	void readCommonFields(const json &j);
	json writeCommonFields() const;

	std::string name;
	Kind kind;
	double durationSeconds{0};
	double periodSeconds{0};
	bool stackable{false};
	std::vector<std::string> effectsAfterFinish;
	std::string visualTexturePath;
};

std::shared_ptr<EffectSample> createEffectFromJson(const EffectSample::json &j);
std::shared_ptr<EffectSample> createEffect(const std::string &name, EffectSample::Kind kind);

#endif // TOWERDEFENCE_EFFECTSAMPLE_H
