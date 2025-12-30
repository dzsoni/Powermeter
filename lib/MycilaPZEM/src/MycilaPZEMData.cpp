// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2025 Mathieu Carbou
 */
#include "MycilaPZEM.h"

static constexpr float DEG_TO_RAD_F = static_cast<float>(3.14159265358979323846 / 180.0);

float Mycila::PZEM::Data::thdi(float phi) const {
  std::lock_guard<std::mutex> lock(_dataMutex);
  if (_powerFactor == 0)
    return NAN;
  const float cosPhi = std::cos(phi * DEG_TO_RAD_F);
  return 100.0f * std::sqrt((cosPhi * cosPhi) / (_powerFactor * _powerFactor) - 1);
}

float Mycila::PZEM::Data::resistance() const {
  std::lock_guard<std::mutex> lock(_dataMutex);
  return _current == 0 ? NAN : std::abs(_activePower / (_current * _current));
}

float Mycila::PZEM::Data::dimmedVoltage() const {
  std::lock_guard<std::mutex> lock(_dataMutex);
  return _current == 0 ? NAN : std::abs(_activePower / _current);
}

float Mycila::PZEM::Data::nominalPower() const {
  std::lock_guard<std::mutex> lock(_dataMutex);
  return _activePower == 0 ? NAN : std::abs(_voltage * _voltage * _current * _current / _activePower);
}

void Mycila::PZEM::Data::clear() {
  std::lock_guard<std::mutex> lock(_dataMutex);
  _frequency = NAN;
  _voltage = NAN;
  _current = NAN;
  _activePower = NAN;
  _reactivePower = NAN;
  _apparentPower = NAN;
  _powerFactor = NAN;
  _activeEnergy = 0;
}

bool Mycila::PZEM::Data::operator==(const Mycila::PZEM::Data& other) const {
  std::lock_guard<std::mutex> lock1(_dataMutex);
  std::lock_guard<std::mutex> lock2(other._dataMutex);
  return (std::isnan(_frequency) ? std::isnan(other._frequency) : _frequency == other._frequency) &&
         (std::isnan(_voltage) ? std::isnan(other._voltage) : _voltage == other._voltage) &&
         (std::isnan(_current) ? std::isnan(other._current) : _current == other._current) &&
         (std::isnan(_activePower) ? std::isnan(other._activePower) : _activePower == other._activePower) &&
         (std::isnan(_reactivePower) ? std::isnan(other._reactivePower) : _reactivePower == other._reactivePower) &&
         (std::isnan(_apparentPower) ? std::isnan(other._apparentPower) : _apparentPower == other._apparentPower) &&
         (std::isnan(_powerFactor) ? std::isnan(other._powerFactor) : _powerFactor == other._powerFactor) &&
         (_activeEnergy == other._activeEnergy);
}

void Mycila::PZEM::Data::operator=(const Mycila::PZEM::Data& other) {
  if (this != &other) {
    std::lock_guard<std::mutex> lock1(_dataMutex);
    std::lock_guard<std::mutex> lock2(other._dataMutex);
    _frequency = other._frequency;
    _voltage = other._voltage;
    _current = other._current;
    _activePower = other._activePower;
    _reactivePower = other._reactivePower;
    _apparentPower = other._apparentPower;
    _powerFactor = other._powerFactor;
    _activeEnergy = other._activeEnergy;
  }
}

#ifdef MYCILA_JSON_SUPPORT
void Mycila::PZEM::Data::toJson(const JsonObject& root) const {
  std::lock_guard<std::mutex> lock(_dataMutex);
  if (!std::isnan(_frequency))
    root["frequency"] = _frequency;
  if (!std::isnan(_voltage))
    root["voltage"] = _voltage;
  if (!std::isnan(_current))
    root["current"] = _current;
  if (!std::isnan(_activePower))
    root["active_power"] = _activePower;
  if (!std::isnan(_reactivePower))
    root["reactive_power"] = _reactivePower;
  if (!std::isnan(_apparentPower))
    root["apparent_power"] = _apparentPower;
  if (!std::isnan(_powerFactor))
    root["power_factor"] = _powerFactor;
  root["active_energy"] = _activeEnergy;
  
  // Calculate derived values while holding the lock
  float r = _current == 0 ? NAN : std::abs(_activePower / (_current * _current));
  float d = _current == 0 ? NAN : std::abs(_activePower / _current);
  float n = _activePower == 0 ? NAN : std::abs(_voltage * _voltage * _current * _current / _activePower);
  float t = NAN;
  if (_powerFactor != 0) {
    const float cosPhi = std::cos(0 * DEG_TO_RAD_F); // phi = 0
    t = 100.0f * std::sqrt((cosPhi * cosPhi) / (_powerFactor * _powerFactor) - 1);
  }
  
  if (!std::isnan(r))
    root["resistance"] = r;
  if (!std::isnan(d))
    root["dimmed_voltage"] = d;
  if (!std::isnan(n))
    root["nominal_power"] = n;
  if (!std::isnan(t))
    root["thdi_0"] = t;
}
#endif
