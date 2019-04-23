#pragma once

#include "communicateLib.hpp"
#include <memory>

class DataCollect
{

};

class Ks34970A_2A
{

public:
	Ks34970A_2A(unsigned int portNo, int baudRate)
	{
		mpCommunicate = static_cast<std::shared_ptr<CommunicateClass::CommunicateInterface>>(std::make_shared<CommunicateClass::ComPortOne>(portNo, baudRate));
		
	}

	Ks34970A_2A()
	{

	}

	~Ks34970A_2A()
	{

	}

	bool init()
	{
		if (mpCommunicate->init() == false)
		{
			return 0;
		}

		QString tRecv;
		mpCommunicate->communicate("*RST", tRecv);
		mpCommunicate->communicate("*CLS", tRecv);
		
		return 0;
	}

private:
	std::shared_ptr<CommunicateClass::CommunicateInterface> mpCommunicate;
};
