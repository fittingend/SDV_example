#ifndef __IZONE3_RESERVE_A_LISTENER_HPP__
#define __IZONE3_RESERVE_A_LISTENER_HPP__

namespace zone3
{
namespace reserved
{
namespace a
{

class IZone3ReservedAListener {
public:
	virtual ~IZone3ReservedAListener() {};
	virtual void notifyDeviceNormal(const std::uint8_t &fieldValue) = 0;
};

}
}
}

#endif // __IZONE3_RESERVE_A_LISTENER_HPP__