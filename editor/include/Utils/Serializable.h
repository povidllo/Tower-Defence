#ifndef EDITOR_SERIALIZABLE_H
#define EDITOR_SERIALIZABLE_H

#pragma once
#include <nlohmann/json.hpp>

class ISerializable {
public:
    virtual ~ISerializable() = default;

    virtual nlohmann::json toJson() const = 0;

    virtual void fromJson(const nlohmann::json& j) = 0;
};


#endif //EDITOR_SERIALIZABLE_H