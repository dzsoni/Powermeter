#ifndef SERIAL_COMMAND_H
#define SERIAL_COMMAND_H
#include <WString.h>
#include <Stream.h>
#include <HardwareSerial.h>
#include <IResponseTaker.h>
#include <CommandCenter.h>

class Serial_Command : public IResponseTaker
{
    private:
        CommandCenter& _mycommandcenter;
        String  _rawcommand;
        Stream& _serial;
        
    public:
        Serial_Command(CommandCenter& mycommandcenter);     // Constructor with default Stream object
        ~Serial_Command(){};   // Destructor

        void processResponse(String response) override; //do something with the response
        void processSerialInput();                      //process the serial input in the loop
};
#endif /* SERIAL_COMMAND_H */
