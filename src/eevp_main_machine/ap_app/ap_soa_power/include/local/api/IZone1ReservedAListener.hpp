#ifndef __IZONE1_RESERVE_A_LISTENER_HPP__
#define __IZONE1_RESERVE_A_LISTENER_HPP__

namespace zone1
{
namespace reserved
{
namespace a
{

class IZone1ReservedAListener {
public:
	virtual ~IZone1ReservedAListener() {};
	virtual void notifyDeviceNormal(const std::uint8_t &fieldValue) = 0;
};

}
}
}

#endif // __IZONE1_RESERVE_A_LISTENER_HPP__