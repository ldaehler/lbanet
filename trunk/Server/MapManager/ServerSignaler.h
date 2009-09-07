#ifndef __LBANET_SIGNALER_SERVER_H__
#define __LBANET_SIGNALER_SERVER_H__

#include "SignalerBase.h"
#include "MapHandler.h"
/*
************************************************************************************************************************
*                                                  class ServerSignaler
************************************************************************************************************************
*/
class ServerSignaler : public SignalerBase
{

public:
	//! constructor
	ServerSignaler(MapHandler * MH)
		: _MH(MH)
	{}

	//! destructor
	virtual ~ServerSignaler(){}

	//! send signal
	virtual void SendSignal(long signal, const std::vector<long> &targets)
	{
		_MH->SendSignal(signal, targets);
	}

private:
	MapHandler * _MH;
};



#endif