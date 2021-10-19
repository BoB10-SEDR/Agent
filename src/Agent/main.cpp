#include <iostream>
#include "Device.h"

int main(int argc, char* argv[])
{
	Device dev = Device();
	dev.DeviceInit();
	dev.DeviceInfoPrint();

	return 0;
}