#ifndef __IZONE4_RESERVE_A_LISTENER_HPP__
#define __IZONE4_RESERVE_A_LISTENER_HPP__

namespace zone4
{
namespace reserved
{
namespace a
{

class IZone4ReservedAListener {
public:
	virtual ~IZone4ReservedAListener() {};
	virtual void notifyDeviceNormal(const std::uint8_t &fieldValue) = 0;
};

}
}
}

#endif // __IZONE4_RESERVE_A_LISTENER_HPP__