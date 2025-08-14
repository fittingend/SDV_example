#ifndef __IZONE2_RESERVE_A_LISTENER_HPP__
#define __IZONE2_RESERVE_A_LISTENER_HPP__

namespace zone2
{
namespace reserved
{
namespace a
{

class IZone2ReservedAListener {
public:
	virtual ~IZone2ReservedAListener() {};
	virtual void notifyDeviceNormal(const std::uint8_t &fieldValue) = 0;
};

}
}
}

#endif // __IZONE2_RESERVE_A_LISTENER_HPP__