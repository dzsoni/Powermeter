#include "WifiManager.h"


/**
 * @brief Sets up the SoftAP (Software Access Point) mode for the device.
 *
 * This function configures the SoftAP mode with the specified IP address, gateway, and subnet mask.
 * It then starts the SoftAP with the provided SSID and password. The SoftAP name and server IP
 * address are printed to the serial monitor.
 */
void WifiManager::setupSoftAP()
{
    Serial.println(F("Starting SoftAP."));

    WiFi.softAPConfig(IPAddress(DEF_AP_IP),
                      IPAddress(DEF_GATEWAY_IP),
                      IPAddress(DEF_SUBNETMASK));
    WiFi.softAP(_APssid.c_str(), _APpassword.c_str(), 1, 0, 4);

    Serial.print(F("SoftAP name and server IP address: "));
    Serial.print(String(_APssid + "  "));
    Serial.println(WiFi.softAPIP());
    
}


/**
 * @brief Constructs a new WifiManager object and initializes member variables.
 *
 * This constructor sets the initial path for the secret JSON file, default AP password, 
 * default AP SSID, and initializes the credentials vector with empty SSID and password pairs.
 */

WifiManager::WifiManager()
{
    _pathToSecretjson = WIFI_SECRETS_JSON;
    _APpassword       = DEFAULT_AP_PASSWORD;
    _APssid           = DEFAULT_AP_SSID;
    _apscredentials.assign(MAX_SSID_NUM, std::make_pair("", ""));
}

WifiManager::~WifiManager()
{
    _apscredentials.clear();
}

/**
 * Initializes the WifiManager by loading credentials, setting up DNS, and setting up the SoftAP.
 *
 */
void WifiManager::begin()
{
    SPIFFS.begin();
    _connecting = false;
    WiFi.mode(WIFI_AP_STA);
    loadCredentials();    
    setupSoftAP();
}

void WifiManager::disconnect()
{
    WiFi.disconnect(false);
}


/**
 * @brief Adds WiFi credentials to the specified index in the credentials vector.
 *
 * This function takes an SSID and password, and stores them as a pair at the given index
 * in the `_apscredentials` vector. The index is clamped to ensure it does not exceed the
 * maximum number of allowed SSIDs. The existing credentials at the specified index are
 * cleared before the new credentials are stored.
 *
 * @param ssid The SSID string to add.
 * @param pass The password string associated with the SSID.
 * @param index The index in the credentials vector where the credentials should be stored.
 */

void WifiManager::addCredentials(String ssid, String pass, uint8_t index)
{
    index = (index < MAX_SSID_NUM) ? index : MAX_SSID_NUM - 1;
    _apscredentials[index].first.clear();
    _apscredentials[index].second.clear(); 

    _apscredentials[index].first = ssid;
    _apscredentials[index].second= pass;
    _apscredentials.shrink_to_fit();
}

/**
 * @brief Set the path to the secret JSON file
 * @param pathTosecretjson The path to the secret JSON file
 */
void WifiManager::setPathToSecretjson(String pathTosecretjson)
{
    _pathToSecretjson = pathTosecretjson;
}

/**
 * @brief Loads WiFi credentials from a JSON file
 *
 *
 */
void WifiManager::loadCredentials()
{
    SimpleJsonParser sjsonp;
    _apscredentials.clear();
    for (uint8_t i = 0; i < MAX_SSID_NUM; i++)
    {
        String assid = sjsonp.getValueByKeyFromFile(_pathToSecretjson, "ssid" + String(i));
        String apass = sjsonp.getValueByKeyFromFile(_pathToSecretjson, "pass" + String(i));

        _apscredentials.push_back(std::make_pair(assid, apass));
    }
    _APpassword = sjsonp.getValueByKeyFromFile(_pathToSecretjson, "APpassw");
}

/**
 * Saves the WiFi credentials to a JSON file.
 *
 * @throws None
 */
void WifiManager::saveCredentials()
{
    String jsonString = "{";
    jsonString.concat("\"APpassw\":\"");
    jsonString.concat(_APpassword);
    jsonString.concat("\",");

    jsonString.concat("\"ssid0\":\"");
    jsonString.concat(_apscredentials[0].first);
    jsonString.concat("\",");

    jsonString.concat("\"pass0\":\"");
    jsonString.concat(_apscredentials[0].second);
    jsonString.concat("\",");

    jsonString.concat("\"ssid1\":\"");
    jsonString.concat(_apscredentials[1].first);
    jsonString.concat("\",");

    jsonString.concat("\"pass1\":\"");
    jsonString.concat(_apscredentials[1].second);
    jsonString.concat("\",");

    jsonString.concat("\"ssid2\":\"");
    jsonString.concat(_apscredentials[2].first);
    jsonString.concat("\",");

    jsonString.concat("\"pass2\":\"");
    jsonString.concat(_apscredentials[2].second);
    jsonString.concat("\"}");

    File file = SPIFFS.open(_pathToSecretjson, "w");
    if (!file)
    {
        _WIFIMANAGER_PL(F("Error opening file for writing"));
        return;
    }

    file.print(jsonString);
    file.flush();
    file.close();
}

/**
 * Sets the password for the SoftAP.
 *
 * @param pass The password to set for the SoftAP.
 *
 */
void WifiManager::setAPpassword(String pass)
{
    _APpassword = pass;
}

/**
 * Automatically reconnects to WiFi if disconnected.
 *
 * This function checks the current WiFi status and attempts to reconnect if not already connected.
 * If no previous WiFi connection is available, it starts a new connection using the credentials stored in the `_apscredentials` vector.
 * If a previous connection is available, it attempts to reconnect to that network.
 * If the connection attempt times out, it moves on to the next available network in the `_apscredentials` vector.
 * If a connection is successfully established, it prints the IP address and SSID of the connected network.
 */
void WifiManager::wifiAutoReconnect()
{
    if (WiFi.status() != WL_CONNECTED && !_connecting)
    {
        Serial.println(F("\nNo WiFi connection."));
        if (_apscredentials[_lastconnectednetwork].first != "")
        {
            WiFi.begin(_apscredentials[_lastconnectednetwork].first.c_str(),
                       _apscredentials[_lastconnectednetwork].second.c_str());
        }
        _lastconnectatempt = millis();
        _connecting = true;
    }
    else if (WiFi.status() != WL_CONNECTED && _connecting && (millis() - _lastconnectatempt > WAIT_FOR_WIFI_TIME_OUT))
    {
        if (++_lastconnectednetwork >= MAX_SSID_NUM)
            _lastconnectednetwork = 0;
        WiFi.begin(_apscredentials[_lastconnectednetwork].first.c_str(),
                   _apscredentials[_lastconnectednetwork].second.c_str());
        _lastconnectatempt = millis();
    }
    else if (WiFi.status() == WL_CONNECTED && _connecting)
    {
        _connecting = false;
        Serial.println(F("\nWiFi connected."));
        Serial.println(F("IP address: "));
        Serial.println(WiFi.localIP());
        Serial.print(F("ssid: "));
        Serial.println(WiFi.SSID());
    }
}
