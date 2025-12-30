// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2025 Mathieu Carbou
 */
#pragma once

#include <HardwareSerial.h>

#include <mutex>
#include <memory>
#include <utility>
#include <vector>

#ifdef MYCILA_JSON_SUPPORT
  #include <ArduinoJson.h>
#endif

#define MYCILA_PZEM_VERSION          "8.0.3"
#define MYCILA_PZEM_VERSION_MAJOR    8
#define MYCILA_PZEM_VERSION_MINOR    0
#define MYCILA_PZEM_VERSION_REVISION 3

#ifndef MYCILA_PZEM_ASYNC_CORE
  #define MYCILA_PZEM_ASYNC_CORE 0
#endif

#ifndef MYCILA_PZEM_ASYNC_PRIORITY
  #define MYCILA_PZEM_ASYNC_PRIORITY 1
#endif

#ifndef MYCILA_PZEM_ASYNC_STACK_SIZE
  #define MYCILA_PZEM_ASYNC_STACK_SIZE 3072 // 512 * 6
#endif

// Set the maximum number of PZEM instances to use in async mode
// In async mode, a list of PZEM instances is maintained
// and they are called one by one in a dedicated FreeRTOS task.
#ifndef MYCILA_PZEM_ASYNC_MAX_INSTANCES
  #define MYCILA_PZEM_ASYNC_MAX_INSTANCES 4
#endif

#define MYCILA_PZEM_ADDRESS_UNKNOWN 0x00
#define MYCILA_PZEM_ADDRESS_MIN     0x01
#define MYCILA_PZEM_ADDRESS_MAX     0xF7
// default general address in a single device configuration which is able to reach any device
// in a mutli-device configuration, the address should be set to a value between 0x01 and 0xF7
#define MYCILA_PZEM_ADDRESS_GENERAL 0xF8

namespace Mycila {
  class PZEM {
    public:
      enum class EventType {
        // PZEM has successfully read the data
        EVT_READ = 0,
        // wrong data received when reading values
        EVT_READ_ERROR,
        // timeout reached when reading values
        EVT_READ_TIMEOUT
      };

      class Data {
        public:
          // Default constructor
          Data() = default;
          
          // Copy constructor
          Data(const Data& other) {
            std::lock_guard<std::mutex> lock(other._dataMutex);
            _frequency = other._frequency;
            _voltage = other._voltage;
            _current = other._current;
            _activePower = other._activePower;
            _powerFactor = other._powerFactor;
            _apparentPower = other._apparentPower;
            _reactivePower = other._reactivePower;
            _activeEnergy = other._activeEnergy;
          }
          
          // Move constructor
          Data(Data&& other) noexcept {
            std::lock_guard<std::mutex> lock(other._dataMutex);
            _frequency = other._frequency;
            _voltage = other._voltage;
            _current = other._current;
            _activePower = other._activePower;
            _powerFactor = other._powerFactor;
            _apparentPower = other._apparentPower;
            _reactivePower = other._reactivePower;
            _activeEnergy = other._activeEnergy;
          }

          /**
           * @brief Get frequency in hertz (Hz).
           */
          float getFrequency() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _frequency;
          }

          /**
           * @brief Get voltage in volts (V).
           */
          float getVoltage() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _voltage;
          }

          /**
           * @brief Get current in amperes (A).
           */
          float getCurrent() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _current;
          }

          /**
           * @brief Get active power in watts (W).
           */
          float getActivePower() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _activePower;
          }

          /**
           * @brief Get power factor
           */
          float getPowerFactor() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _powerFactor;
          }

          /**
           * @brief Get apparent power in volt-amperes (VA).
           */
          float getApparentPower() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _apparentPower;
          }

          /**
           * @brief Get reactive power in volt-amperes reactive (VAr).
           */
          float getReactivePower() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _reactivePower;
          }

          /**
           * @brief Get active energy in watt-hours (Wh).
           */
          uint32_t getActiveEnergy() const {
            std::lock_guard<std::mutex> lock(_dataMutex);
            return _activeEnergy;
          }

          /**
           * @brief Compute the total harmonic distortion percentage of current (THDi).
           * This assumes THDu = 0 (perfect voltage sin wave).
           * See: https://fr.electrical-installation.org/frwiki/Indicateur_de_distorsion_harmonique_:_facteur_de_puissance
           * @param phi The phase shift angle in degrees (°) (0 for resistive load)
           * @return The total harmonic distortion of current (THDi) as a percentage (%)
           */
          float thdi(float phi = 0) const;

          /**
           * @brief Compute the resistance of the load in ohms (R = P / I^2).
           */
          float resistance() const;

          /**
           * @brief Compute the dimmed voltage (V = P / I).
           * @note The dimmed voltage is the voltage that would be measured at the output of a TRIAC, SSR or voltage regulator device.
           */
          float dimmedVoltage() const;

          /**
           * @brief Compute the nominal power of the load in watts (P = V^2 / R).
           * @note The voltage is the nominal voltage measured by the device and R is the measured resistance of the load, which can be regulated by a TRIAC, SSR or voltage regulator.
           */
          float nominalPower() const;

          // clear all values
          void clear();

          // compare two data
          bool operator==(const Data& other) const;
          // compare two data
          bool operator!=(const Data& other) const { return !(*this == other); }
          // copy a data
          void operator=(const Data& other);

#ifdef MYCILA_JSON_SUPPORT
          void toJson(const JsonObject& root) const;
#endif

        private:
          friend class PZEM;
          mutable std::mutex _dataMutex;
          
          /**
           * @brief Frequency in hertz (Hz).
           */
          float _frequency = NAN; // Hz

          /**
           * @brief Voltage in volts (V).
           */
          float _voltage = NAN;

          /**
           * @brief Current in amperes (A).
           */
          float _current = NAN;

          /**
           * @brief Active power in watts (W).
           */
          float _activePower = NAN;

          /**
           * @brief Power factor
           */
          float _powerFactor = NAN;

          /**
           * @brief Apparent power in volt-amperes (VA).
           */
          float _apparentPower = NAN;

          /**
           * @brief Reactive power in volt-amperes reactive (VAr).
           */
          float _reactivePower = NAN;

          /**
           * @brief Active energy in watt-hours (Wh).
           */
          uint32_t _activeEnergy = 0;
      };

      typedef std::function<void(EventType eventType, const Data& data)> Callback;

      ~PZEM() { end(); }

      // If the same Serial instance is used by multiple PZEM instances, set shared to true
      void setSharedSerial(bool shared) { _sharedSerial = shared; }

      // - rxPin: RX pin of the board, connected to the TX of the PZEM,
      // - txPin: TX pin of the board, connected to the RX of the PZEM
      // - address: the address of the PZEM. Default to MYCILA_PZEM_DEFAULT_ADDRESS. Set to a value between 0x01 and 0xF7 included, or MYCILA_PZEM_DEFAULT_ADDRESS (default)
      void begin(HardwareSerial& serial, // NOLINT
                 int8_t rxPin,
                 int8_t txPin,
                 uint8_t address = MYCILA_PZEM_ADDRESS_GENERAL,
                 bool async = false);

      void end();

      // No need to call read in async mode
      bool read() { return read(_address); }
      bool read(uint8_t address);

      // Resets energy counters. Returns true if the reset was successful
      bool resetEnergy() { return resetEnergy(_address); }
      bool resetEnergy(uint8_t address);

      /**
       * @brief Get the address of the last device's response.
       */
      uint8_t getLastAddress() const { return _lastAddress; }

      /**
       * @brief Get the address used to send requests.
       * @return The address used to send requests (1-255) or MYCILA_PZEM_ADDRESS_GENERAL (0) for all devices.
       */
      uint8_t getDeviceAddress() const { return _address; }

      // Try to change the address. Returns true if changed
      bool setDeviceAddress(uint8_t newAddress) { return setDeviceAddress(_address, newAddress); }
      bool setDeviceAddress(uint8_t address, uint8_t newAddress);

      // read address from device and update the destination address variable if true.
      // Returns MYCILA_PZEM_ADDRESS_UNKNOWN in case of error or the device address in case of success.
      uint8_t readDeviceAddress(bool update = false);

      // Will start searching for PZEM devices on custom addresses from 0x01 to 0xF7,
      // And then also MYCILA_PZEM_DEFAULT_ADDRESS (0xF8).
      // Will stop when reaching maxCount or when no more devices are found.
      // A full scan can take up to 30 seconds
      // Returns the number of devices found
      // MYCILA_PZEM_DEFAULT_ADDRESS (0xF8) is also searched for to be able to find PZEM devices that have not been assigned an address.
      size_t search(uint8_t* addresses, const size_t maxCount);
      bool canRead(uint8_t address);

#ifdef MYCILA_JSON_SUPPORT
      void toJson(const JsonObject& root) const;
#endif

      gpio_num_t getRXPin() const { return _pinRX; }
      gpio_num_t getTXPin() const { return _pinTX; }
      bool isEnabled() const { return _enabled; }

      // get the uptime in milliseconds of the last successful read
      uint32_t getTime() const { return _time; }

      // check if the device is connected to the , meaning if last read was successful
      bool isConnected() const { return _data.getFrequency() > 0; }

      void setCallback(Callback callback) { _callback = std::move(callback); }

      // Thread-safe data getter for async mode
      Data getData() const;

    private:
      bool _enabled = false;
      bool _sharedSerial = false;
      Callback _callback = nullptr;
      gpio_num_t _pinRX = GPIO_NUM_NC;
      gpio_num_t _pinTX = GPIO_NUM_NC;
      HardwareSerial* _serial = nullptr;
      uint32_t _time = 0;
      uint8_t _buffer[32];
      uint8_t _address = MYCILA_PZEM_ADDRESS_GENERAL;
      uint8_t _lastAddress = MYCILA_PZEM_ADDRESS_UNKNOWN;
      Data _data;

    private:
      enum class ReadResult {
        READ_SUCCESS = 0,
        READ_TIMEOUT,
        READ_ERROR_COUNT,
        READ_ERROR_CRC,
        READ_ERROR_ADDRESS,
      };

    private:
      ReadResult _timedRead(uint8_t expectedAddress, size_t expectedLen);
      void _send(uint8_t address, uint8_t cmd, uint16_t rAddr, uint16_t val);
      void _openSerial(const uint8_t rxPin, const uint8_t txPin);
      size_t _drop();
      static size_t _serialUsers;

    private:
      static void _crcSet(uint8_t* buf, uint16_t len);
      static bool _crcCheck(const uint8_t* buf, uint16_t len);
      static uint16_t _crc16(const uint8_t* data, uint16_t len);

    private:
      static std::recursive_mutex _mutex;
      static std::vector<std::pair<HardwareSerial*, std::unique_ptr<std::recursive_mutex>>> _serialMutexes;
      static std::mutex _serialRegistry;
      static TaskHandle_t _taskHandle;
#if MYCILA_PZEM_ASYNC_MAX_INSTANCES > 0
      static PZEM* _instances[MYCILA_PZEM_ASYNC_MAX_INSTANCES];
      static bool _add(PZEM* pzem);
      static void _remove(PZEM* pzem);
      static void _pzemTask(void* pvParameters);
#endif
      
      // Serial-specific mutex getter
      static std::recursive_mutex& _getSerialMutex(HardwareSerial* serial);
  };
} // namespace Mycila
