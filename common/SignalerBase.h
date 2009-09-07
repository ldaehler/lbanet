#ifndef __LBANET_SIGNALER_BASE_H__
#define __LBANET_SIGNALER_BASE_H__


/*
************************************************************************************************************************
*                                                  class SignalerBase
************************************************************************************************************************
*/
class SignalerBase
{

public:
	//! constructor
	SignalerBase(){}

	//! destructor
	virtual ~SignalerBase(){}

	//! send signal
	virtual void SendSignal(long signal, const std::vector<long> &targets) = 0;

};



#endif