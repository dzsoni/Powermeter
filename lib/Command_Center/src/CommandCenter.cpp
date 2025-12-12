#include "CommandCenter.h"
#include  <algorithm>
#include "SimpleJsonParser.h"



void CommandCenter::processRawCommands()
{
    if(_rawcommands.empty())return;
    command_struct cs;
    rawcommand_struct st = _rawcommands.front();
    _rawcommands.pop();
    String str= st.rawcommand;
    _COMMANDCENT_PL(String(__FUNCTION__)+": rawcommand:"+str)

    int end=0;

    str.trim();
    if(str.length())
    {
       if(_echomessageid)
       {
        end=str.indexOf(" ");
        if(end<0)end=str.length();
        cs.msgid=str.substring(0,end);
        str=str.substring(end);
        str.trim();
       }
        end=str.indexOf(" ");
        if(end<0)end=str.length();
        cs.to=str.substring(0,end);
        str=str.substring(end);
        str.trim();
        if(str.length())
        {
            end=str.indexOf(" ");
            if(end<0)end=str.length();
            cs.command=str.substring(0,end);
            str=str.substring(end);
            str.trim();
            while(str.length())
            {
                end=str.indexOf(" ");
                if(end<0)end=str.length();
                cs.args.emplace_back(str.substring(0,end));
                str=str.substring(end);
                str.trim();
                if(cs.args.size()>MAX_NUMBER_OF_ARGS)
                {
                  _COMMANDCENT_PL(String(__FUNCTION__)+": Too many arguments.")
                    return;
                }
            }
        }

    }
    cs.responsetaker_pt = st.responsetaker_pt;
    _commands.push_back(cs);
}

void CommandCenter::processCommands()
{
    if(_commands.empty())return;
    command_struct cs= _commands.front();
    _COMMANDCENT_PL(String(__FUNCTION__)+": To:"+cs.to+" command:"+cs.command)
    _commands.pop_front();
    comm_tuple commtpl = getCommandTuple(cs.to,cs.command);

    std::shared_ptr<IFunctionHolder>ifh =std::get<ENUM_COMTUPLEORDER_FUNCHOLDER>(commtpl);
    if(!ifh)//false if the shared_ptr is a null pointer std::shared_ptr::operator bool
    {
        _COMMANDCENT_PL(String(__FUNCTION__)+": No such device id or command.")
        return;
    }
    String result = ifh->run(cs.args);
    _responsequeue.push({cs.msgid,result,cs.responsetaker_pt});
}

void CommandCenter::processResponse()
{
    if(_responsequeue.empty())return;
    _COMMANDCENT_PL(String(__FUNCTION__)+": "+_responsequeue.front().response);
    response_struct rs = _responsequeue.front();
    if(_echomessageid)
    {
        rs.response = String(rs.msgid)+" "+rs.response;
    }
    rs.responsetaker_pt->processResponse(rs.response);
    _responsequeue.pop();
}

void CommandCenter::process()
{
    switch (_state)
    {
    case 0:
        processRawCommands();
        break;
    case 1:
        processCommands();
        break;
    case  2:
        processResponse();
        break;
    default:
        break;
    }
  _state=(++_state>=3) ? 0 : _state ;

}


/**
 * Returns the size of the CommandCenter repository.
 *
 * @return The size of the repository as an unsigned integer.
 */
uint CommandCenter::numberOfDevs()
{
    return (uint)_devicerepo.size();
}

uint CommandCenter::numberOfCommandsOfNthDevice(uint nthdevice)
{
    if(nthdevice>=_devicerepo.size())return 0;
    return (uint)_devicerepo[nthdevice].device_commands.size();
}

/**
 * Adds a raw command to the CommandCenter.
 *
 * @param rawcommand The raw command to be added.
 * @param responsetaker_pt The response taker (callback) for the command.
 *
 * @return void
 */
void CommandCenter::addRawCommand(String rawcommand, IResponseTaker *responsetaker_pt)
{
    if(rawcommand.length()==0 || responsetaker_pt==nullptr)return;
    _rawcommands.push({rawcommand,responsetaker_pt});
}



void CommandCenter::shrink_to_fit_DevRepo()
{
    _devicerepo.shrink_to_fit();
}


void CommandCenter::clearCommandsFromDeviceType(String devicetype)
{
    for(device_command_struct element : _devicerepo)
    {
        if(String(element.devicetype) == devicetype)
        {
            element.device_commands.clear();
            return;
        }
    }
}

bool CommandCenter::isDevCommandExist(String devid,comm_tuple newcom)
{
    for(device_command_struct element : _devicerepo)
    {
        if(String(element.devicetype) == devid)
        {
            for( comm_tuple old : element.device_commands)
            {
                if(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(old)) == String(std::get<ENUM_COMTUPLEORDER_COMMAND>(newcom)))
                {
                    return true;     
                }
            }
            return false;
        }
    }
    return false;
}

comm_tuple CommandCenter::getCommandTuple(String userdeviceid, String command)
{   
    if(_devicerepo.empty()) return std::make_tuple(nullptr,nullptr);
    userdeviceid.toUpperCase();
    command.toUpperCase();

    for(auto it =_frequencyindextbl.begin();it!=_frequencyindextbl.end();it++)
    {   
        String oid=_devicerepo[*it].id_by_user;
        oid.toUpperCase();
        if(oid==userdeviceid)
        {
            for( comm_tuple func : _devicerepo[*it].device_commands)
            {
                String ocommand=std::get<ENUM_COMTUPLEORDER_COMMAND>(func);
                ocommand.toUpperCase();
                if(ocommand == command )
                {
                    auto i=*it;
                    _frequencyindextbl.remove(i);
                    auto ptr=_frequencyindextbl.before_begin();
                    _frequencyindextbl.insert_after(ptr,i);

                    return func;     
                }
            }
            return std::make_tuple(nullptr,nullptr);
        }
    }
    return std::make_tuple(nullptr,nullptr);
}

/**
 * Returns the device ID at the specified index in the CommandCenter repository.
 *
 * @param index The index of the device ID to retrieve.
 *
 * @return The device ID at the specified index, or an empty string if the index is out of range.
 */
String CommandCenter::getDeviceTypeByIndex(uint index)
{
    if(index>=_devicerepo.size())return String();
    return String(_devicerepo[index].devicetype);
}

/**
 * Returns the device ID at the specified index in the CommandCenter repository.
 *
 * @param index The index of the device ID to retrieve.
 *
 * @return The device ID at the specified index, or an empty string if the index is out of range.
 */
String CommandCenter::getDeviceIdByIndex(uint index)
{
    if(index>=_devicerepo.size())return String();
    return String(_devicerepo[index].id_by_user);
}

std::vector<String> CommandCenter::getDeviceTypes()
{
    std::vector<String> result;
    if(!_devicerepo.empty())
    {
        result.push_back(String(_devicerepo[0].devicetype));
    }

    for(device_command_struct element : _devicerepo)
    {
       bool found = false;
       for(String s : result)
       {
         if(s == String(element.devicetype)) found = true;
       }
       if(!found) result.push_back(String(element.devicetype));
       
    }
    return result; 
}

/**
 * Returns the nth command of the nth device in the CommandCenter repository.
 *
 * @param nthdevice The index of the device.
 * @param nthcommand The index of the command.
 *
 * @return The nth command of the nth device, or an empty string if the indices are out of range.
 *
 */
String CommandCenter::getNthCommandOfNthDevice(uint nthdevice, uint nthcommand)
{
    if(nthdevice>=_devicerepo.size())return String();
    if(nthcommand>=_devicerepo[nthdevice].device_commands.size())return String();
    return String(std::get<ENUM_COMTUPLEORDER_COMMAND>(_devicerepo[nthdevice].device_commands[nthcommand]));
}

/**
 * Filter the commands of a device in the CommandCenter repository based on a list of string pairs.
 *
 * @param devicename The name of the device to filter.
 * @param filter A list of string pairs, where the first string is the command name and the second
 * string is either "true" or "false". If the second string is "true", the command is left in the 
 * repository. If the second string is "false", the command is removed from the repository.
 *
 * The function iterates over the commands of the device and compares each command name with the
 *  first string of each pair in the filter. If a match is found, the second string of the pair is
 *  tested. If it is "true", the command is left in the repository. If it is "false", the command 
 * is removed from the repository. If no match is found, the command is removed from the repository.
 *
 * After filtering the commands, the function shrinks the repository to fit.
 *
 */
void CommandCenter::filterRepoByStringPair(String devicetype, const std::vector<std::pair<String, String>>& filter)
{
  devicetype.toUpperCase();
  for(vec_device_command_struct::iterator deviceelement = _devicerepo.begin(); deviceelement != _devicerepo.end(); ++deviceelement)
    {
       if(String((*deviceelement).devicetype) != devicetype)continue;
       for(std::vector<comm_tuple>::iterator commanditer = (*deviceelement).device_commands.begin(); commanditer != (*deviceelement).device_commands.end();)
       {
           bool found=false;
           for(auto filterelement : filter)
           {
            if(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(*commanditer)) == filterelement.first && filterelement.second =="false")
            {
               _COMMANDCENT_PL(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(*commanditer)) + " filtered from " + String((*deviceelement).id_by_user));
               found=true;
               (*deviceelement).device_commands.erase(commanditer);
               break;
            }
            else if(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(*commanditer)) == filterelement.first && filterelement.second =="true")
            {
               found=true;
               commanditer++;
               break;
            }
           }
           if(!found)
           {
            _COMMANDCENT_PL(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(*commanditer)) + " removed from " + String((*deviceelement).id_by_user)+" because not in filter");
            (*deviceelement).device_commands.erase(commanditer);
           }
       }
       (*deviceelement).device_commands.shrink_to_fit();
       _COMMANDCENT_PL("Device " + String((*deviceelement).devicetype) + " " + String((*deviceelement).id_by_user) + " contains " + String((*deviceelement).device_commands.size()) + " commands.");
    }
    _devicerepo.shrink_to_fit();
    
}
/**
 * Filter the commands of a device in the CommandCenter repository based on a filter file.
 *
 * @param devicename The name of the device to filter.
 *
 * The function reads the filter file (FILTERFILE_PREFIX + devicename + ".json") and extracts the
 * first and second strings of each pair in the file. If the second string is "true", the command
 * is left in the repository. If the second string is "false", the command is removed from the
 * repository. If no match is found, the command is removed from the repository.
 *
 * After filtering the commands, the function shrinks the repository to fit.
 *
 */
void CommandCenter::filterRepoByFiterFile(String devicetype)
{
  SimpleJsonParser sjp;
  std::vector<std::pair<String, String>> svec;
  devicetype.toUpperCase();
  svec = sjp.extractKeysandValuesFromFile(FILTERFILE_PREFIX + devicetype + ".json" );
  if(svec.size()==0)
  {
    _COMMANDCENT_PL("No filter file found for " + devicetype + " (" + FILTERFILE_PREFIX + devicetype + ".json)");
    return;
  }
  filterRepoByStringPair(devicetype, svec);
}




/**
 * Fill every same device which has newdew.devicetype in the CommandCenter repository with new commands.
 *
 * This function takes a device_command_struct and checks if a device with the same devicetype already
 * exists in the repository. If a device with the same devicetype exists, the function adds all commands
 * from the device_command_struct that are not already present in the existing device to the existing
 * device. If no device with the same devicetype exists, the function does nothing.
 *
 * @param newdev The device_command_struct that contains the new commands.
 *
 */
void CommandCenter::fillDeviceWithCommands(device_command_struct &newdev)
{
    if(String(newdev.devicetype).length()==0 || newdev.id_by_user.length()==0)return;
    for(vec_device_command_struct::iterator element = _devicerepo.begin(); element!= _devicerepo.end(); ++element)
    {
        if( (*element).devicetype == newdev.devicetype)
        {
            for(comm_tuple nc : newdev.device_commands)
            {
                bool commandexist=false;
                for(comm_tuple old : (*element).device_commands)
                {
                    if (String(std::get<ENUM_COMTUPLEORDER_COMMAND>(nc)) == String(std::get<ENUM_COMTUPLEORDER_COMMAND>(old)))
                    {
                        commandexist = true;
                        break;
                    }
                }
                if (!commandexist)
                {
                    _COMMANDCENT_PL(String(std::get<ENUM_COMTUPLEORDER_COMMAND>(nc)) + " added to " + String((*element).devicetype));
                    (*element).device_commands.push_back(nc);
                }
            }
        }
    }    
}

/**
 * Add a new device to the CommandCenter repository.
 *
 * This function adds a new device to the repository. If a device with the same id_by_user already
 * exists in the repository, the function does nothing.
 *
 * @param newdev The device_command_struct to add to the repository.
 *
 */
void CommandCenter::addDevice(device_command_struct newdev)
{
    if(String(newdev.devicetype).length()==0 || newdev.id_by_user.length()==0)return;
    for(vec_device_command_struct::iterator element = _devicerepo.begin(); element!= _devicerepo.end(); ++element)
    {
        if( (*element).id_by_user == newdev.id_by_user)
        {
        _COMMANDCENT_PL("Device with this name" + String(newdev.id_by_user) + " already exists in repo.");
        return;
        }
    }
    _COMMANDCENT_PL("New device " + String(newdev.devicetype) + " added to repo.");
    _frequencyindextbl.push_front(_devicerepo.size());
    _devicerepo.push_back(newdev);
}

void CommandCenter::replaceDevice(String olddevid, device_command_struct newdev)
{
    for(vec_device_command_struct::iterator element = _devicerepo.begin(); element!= _devicerepo.end(); ++element)
    {
        if( (*element).id_by_user == olddevid)
        {
            _COMMANDCENT_PL("Device " + String(olddevid) + " replaced in repo.");
            (*element) = newdev;
            return;
        }
    }
    _COMMANDCENT_PL("Device " + String(olddevid) + " not found in repo.");
}
