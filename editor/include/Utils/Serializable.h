#ifndef EDITOR_SERIALIZABLE_H
#define EDITOR_SERIALIZABLE_H

#pragma once
#include <nlohmann/json.hpp>

class ISerializable {
public:
	virtual ~ISerializable() = default;

	virtual nlohmann::ordered_json toJson() const = 0;

	virtual void fromJson(const nlohmann::ordered_json &j) = 0;
};

#endif //EDITOR_SERIALIZABLE_H
