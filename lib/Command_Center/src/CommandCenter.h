#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <queue>
#include <list>
#include <functional>
#include <memory>
#include <vector>
#include "enum_CommandRepo.h"
#include "FunctionHolder.h"
#include <forward_list>


/*
if you don't need any collectFun, please define NO_NEED_XXX_COLLECTFUN in your compile option in platformio.ini

[env]
build_flags = -D NO_NEED_XXX_COLLECTFUN
 NO_NEED_RELAY_COLLECTFUN
 NO_NEED_RELAYARRAY_COLLECTFUN
 NO_NEED_NONBLOCKINGDALLASARRAY_COLLECTFUN
 NO_NEED_COMMANDCENTER_COLLECTFUN
 NO_NEED_MQTTCOMMAND_COLLECTFUN
 NO_NEED_WIFIMANAGER_COLLECTFUN
 ..etc
*/



//#define DEBUG_COMMANDCENTER //comment out or not 

#ifdef DEBUG_COMMANDCENTER
#define _COMMANDCENT_PP(a) Serial.print(a);
#define _COMMANDCENT_PL(a) Serial.println(a);
#else
#define _COMMANDCENT_PP(a)
#define _COMMANDCENT_PL(a)
#endif

#define FILTERFILE_PREFIX "/fltr_"
#define MAX_NUMBER_OF_ARGS 10

/**
 * @class CommandCenter
 * @brief Manages the processing of raw commands, commands, and responses for a device repository.
 *
 * The CommandCenter class is responsible for handling the lifecycle of raw commands, commands, 
 * and responses for a set of devices. It provides methods to add raw commands, process them,
 *  and manage the device repository. The class also supports filtering the device repository based 
 * on device types and command tuples.
 */
class CommandCenter
{
private:
    bool    _echomessageid = true;
    uint8_t _state=0;
    vec_device_command_struct _devicerepo;
    std::queue<rawcommand_struct, std::list<rawcommand_struct>> _rawcommands;
    std::list<command_struct> _commands;
    std::queue<response_struct, std::list<response_struct>> _responsequeue;
    std::forward_list<unsigned int> _frequencyindextbl;

    
    void processRawCommands();
    void processCommands();
    void processResponse();

public:
    CommandCenter(){};
    ~CommandCenter(){};
    void process();//step forward in the state machine 
    void echoMessageIdOn(){_echomessageid=true;};
    void echoMessageIdOff(){_echomessageid=false;};

    uint numberOfDevs();
    uint numberOfCommandsOfNthDevice(uint nthdevice);
    void addRawCommand(String rawcommand,IResponseTaker* responsetaker_pt);
	void shrink_to_fit_DevRepo();
    void clearCommandsFromDeviceType(String devicetype);
	bool isDevCommandExist(String devid,comm_tuple newcom);
    comm_tuple getCommandTuple(String id, String command);
    String getDeviceTypeByIndex(uint index);
    String getDeviceIdByIndex(uint index);
    std::vector<String> getDeviceTypes();
    String getNthCommandOfNthDevice(uint nthdevice, uint nthcommand);
    void filterRepoByStringPair(String devicename, const std::vector<std::pair<String,String>>& filter);
    void filterRepoByFiterFile(String devicename);
    void fillDeviceWithCommands(device_command_struct& newcomm);
    void addDevice(device_command_struct newdev);
    void replaceDevice(String olddevid, device_command_struct newdev);
};
#endif /* COMMANDCENTER_H */
