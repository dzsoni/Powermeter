#include "Serial_Command.h"

Serial_Command::Serial_Command(CommandCenter& mycommandcenter):_mycommandcenter(mycommandcenter),_serial(Serial)
{
  _rawcommand.clear();
}
void Serial_Command::processResponse(String response)
{
    _serial.println(response);
}
/**
 * Processes the serial input and adds the raw command to the CommandCenter.
 *
 * @throws None
 */
void Serial_Command::processSerialInput()
{
    //process the serial input
    while (_serial.available())
    {
        char c = _serial.read();
        if (c == '\r' || c == '\n')
        { 
            _mycommandcenter.addRawCommand(_rawcommand,this);
            _rawcommand = "";
        }
        else if(isprint(c)  )
        {
            //add the character to the command
                _rawcommand += c;
        }
    }
};