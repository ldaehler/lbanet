#ifndef _LBA_NET_DATA_FILE_HANDLER_H
#define _LBA_NET_DATA_FILE_HANDLER_H


/***********************************
*	handle the path of the data file loaded by the game
*************************************/
class DataFileHandler
{
public:

	static std::string GetPath(const std::string Filename)
	{
		std::string str("Data/");
		str += Filename;
		str += ".HQR";
		return str;
	}

	static std::string GetMidiPath(const std::string Filename)
	{
		std::string str("Data/");
		str += Filename;
		str += ".midi";
		return str;
	}
};


#endif
