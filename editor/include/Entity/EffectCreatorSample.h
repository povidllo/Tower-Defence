#ifndef TOWERDEFENCE_EFFECTCREATORSAMPLE_H
#define TOWERDEFENCE_EFFECTCREATORSAMPLE_H

#include <string>
#include <vector>

#include "Serializable.h"

class EffectCreatorSample : protected ISerializable {
public:
	using json = nlohmann::ordered_json;

	explicit EffectCreatorSample(std::string name);

	explicit EffectCreatorSample(const json &j);

	json toJson() const override;

	void fromJson(const json &j) override;

	[[nodiscard]] const std::string &getName() const;

	void setName(std::string name);

	[[nodiscard]] const std::vector<std::string> &getStartEffects() const;

	void setStartEffects(std::vector<std::string> effects);

	void addStartEffect(const std::string &name);

	bool removeStartEffect(const std::string &name);

	[[nodiscard]] const std::vector<std::string> &getPeriodicEffects() const;

	void setPeriodicEffects(std::vector<std::string> effects);

	void addPeriodicEffect(const std::string &name);

	bool removePeriodicEffect(const std::string &name);

	[[nodiscard]] double getRadius() const;

	void setRadius(double value);

	[[nodiscard]] double getDurationSeconds() const;

	void setDurationSeconds(double value);

	[[nodiscard]] double getPeriodSeconds() const;

	void setPeriodSeconds(double value);

	[[nodiscard]] const std::string &getTargetRelation() const;

	void setTargetRelation(std::string value);

	[[nodiscard]] const std::string &getAttachmentPreference() const;

	void setAttachmentPreference(std::string value);

private:
	std::string name;
	std::vector<std::string> startEffects;
	std::vector<std::string> periodicEffects;
	double radius{0};
	double durationSeconds{0};
	double periodSeconds{0};
	std::string targetRelation{"enemies"};
	std::string attachmentPreference{"point"};
};

#endif // TOWERDEFENCE_EFFECTCREATORSAMPLE_H
