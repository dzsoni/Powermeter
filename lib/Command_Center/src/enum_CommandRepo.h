#ifndef ENUM_COMMANDREPO_H
#define ENUM_COMMANDREPO_H

#include <pgmspace.h>
#include <functional>
#include <memory>
#include <vector>
#include "IFunctionHolder.h"
#include "IResponseTaker.h"

struct rawcommand_struct
{
	String rawcommand;
	IResponseTaker *responsetaker_pt;
};

struct command_struct
{
	String msgid;
	String to;
	String command;
	std::vector<String> args;
	IResponseTaker *responsetaker_pt;
};
struct response_struct
{
	String msgid;
	String response;
	IResponseTaker *responsetaker_pt;
};

enum comm_tuple_order : uint8_t
{
	ENUM_COMTUPLEORDER_COMMAND,
	ENUM_COMTUPLEORDER_FUNCHOLDER
};

typedef std::tuple<const __FlashStringHelper *, std::shared_ptr<IFunctionHolder>> comm_tuple;

static const char device_No[] PROGMEM = "";
struct device_command_struct
{
	const __FlashStringHelper *devicetype = FPSTR(device_No);
	String id_by_user;
	std::vector<comm_tuple> device_commands;
	
	~device_command_struct() { device_commands.clear(); };
};

typedef std::vector<device_command_struct> vec_device_command_struct;

typedef std::function<void(void)> ft_V_V;
typedef std::function<void(int)> ft_V_I;
typedef std::function<void(unsigned int)> ft_V_UI;
typedef std::function<void(unsigned char)> ft_V_UC;
typedef std::function<void(bool)> ft_V_B;
typedef std::function<void(unsigned char, bool)> ft_V_UC_B;
typedef std::function<void(unsigned char, bool, bool)> ft_V_UC_B_B;
typedef std::function<void(String, bool)> ft_V_S_B;
typedef std::function<void(String, String, unsigned char)> ft_V_S_S_UC;
typedef std::function<void(String)> ft_V_S;
typedef std::function<void(String,String)> ft_V_S_S;

typedef std::function<bool(void)> ft_B_V;
typedef std::function<bool(unsigned char)> ft_B_UC;
typedef std::function<bool(String)> ft_B_S;
typedef std::function<bool(String, unsigned char)> ft_B_S_UC;
typedef std::function<bool(unsigned char,String )> ft_B_UC_S;
typedef std::function<bool(String, String)> ft_B_S_S;
typedef std::function<bool(String, unsigned char, bool, bool)>  ft_B_S_UC_B_B;
typedef std::function<bool(unsigned char, bool, bool)> ft_B_UC_B_B;
typedef std::function<bool(String, bool, bool)> ft_B_S_B_B;

typedef std::function<unsigned char(void)> ft_UC_V;
typedef std::function<unsigned char(String)> ft_UC_S;
typedef std::function<unsigned char(unsigned char)> ft_UC_UC;

typedef std::function<char(unsigned char)> ft_C_UC;
typedef std::function<char(String)> ft_C_S;

typedef std::function<float(String)> ft_F_S;

typedef std::function<int(void)> ft_I_V;
typedef std::function<unsigned int(void)> ft_UI_V;
typedef std::function<int(unsigned char)> ft_I_UC;
typedef std::function<int(String)> ft_I_S;

typedef std::function<String(void)> ft_S_V;
typedef std::function<String(unsigned char)> ft_S_UC;
typedef std::function<String(unsigned int, unsigned int)> ft_S_UI_UI;

#endif /* ENUM_COMMANDREPO_H */
