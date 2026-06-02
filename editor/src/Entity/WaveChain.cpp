#include "../../include/Entity/WaveChain.h"

#include <algorithm>

WaveChain::json WaveChain::toJson() const {
	json chainArray = json::array();
	for (const auto &waveName : chain) {
		chainArray.push_back(waveName);
	}

	json delaysArray = json::array();
	for (const double delay : delays) {
		delaysArray.push_back(delay);
	}

	return {
		{"chain", chainArray},
		{"delays", delaysArray},
	};
}

void WaveChain::fromJson(const json &j) {
	chain.clear();
	delays.clear();

	if (j.contains("chain") && j["chain"].is_array()) {
		for (const auto &item : j["chain"]) {
			if (item.is_string()) {
				chain.push_back(item.get<std::string>());
			}
		}
	}

	if (j.contains("delays") && j["delays"].is_array()) {
		for (const auto &item : j["delays"]) {
			delays.push_back(item.get<double>());
		}
	}

	normalizeDelays();
}

void WaveChain::setChain(std::vector<std::string> waves) {
	chain = std::move(waves);
	normalizeDelays();
}

void WaveChain::setDelays(std::vector<double> values) {
	delays = std::move(values);
	normalizeDelays();
}

void WaveChain::addStep(const std::string &waveName, const double delayBefore) {
	chain.push_back(waveName);
	delays.push_back(delayBefore);
}

void WaveChain::removeStep(const size_t index) {
	if (index >= chain.size()) {
		return;
	}
	chain.erase(chain.begin() + static_cast<std::ptrdiff_t>(index));
	if (index < delays.size()) {
		delays.erase(delays.begin() + static_cast<std::ptrdiff_t>(index));
	}
	normalizeDelays();
}

void WaveChain::removeWaveReference(const std::string &waveName) {
	for (size_t i = 0; i < chain.size();) {
		if (chain[i] == waveName) {
			removeStep(i);
		} else {
			++i;
		}
	}
}

void WaveChain::renameWaveReference(const std::string &oldName, const std::string &newName) {
	for (auto &waveName : chain) {
		if (waveName == oldName) {
			waveName = newName;
		}
	}
}

void WaveChain::normalizeDelays() {
	if (chain.empty()) {
		delays.clear();
		return;
	}

	if (delays.size() < chain.size()) {
		delays.resize(chain.size(), 0.0);
	} else if (delays.size() > chain.size()) {
		delays.resize(chain.size());
	}
}
