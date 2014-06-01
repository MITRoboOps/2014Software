/* Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Auto-generated by genmsg_cpp from file /home/roboops/2014Software/groovy2014/src/Messages/msg/NavigationMessage.msg
 *
 */


#ifndef MESSAGES_MESSAGE_NAVIGATIONMESSAGE_H
#define MESSAGES_MESSAGE_NAVIGATIONMESSAGE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace Messages
{
template <class ContainerAllocator>
struct NavigationMessage_
{
  typedef NavigationMessage_<ContainerAllocator> Type;

  NavigationMessage_()
    : timeUp(0)
    , BLcurrent(0)
    , FLcurrent(0)
    , BRcurrent(0)
    , FRcurrent(0)
    , BLvel(0)
    , FLvel(0)
    , BRvel(0)
    , FRvel(0)
    , chargeUsed(0)
    , current(0)
    , voltage(0)
    , latitude(0)
    , longitude(0)
    , roll(0)
    , pitch(0)
    , yaw(0)
    , boomCurrent(0)
    , armRotCurrent(0)
    , thumbPosition(0)
    , clawPosition(0)
    , stickPosition(0)
    , boomPosition(0)
    , armRotPosition(0)
    , panPosition(0)
    , tiltPosition(0)
    , satilites(0)
    , CBoomPosition(0)  {
    }
  NavigationMessage_(const ContainerAllocator& _alloc)
    : timeUp(0)
    , BLcurrent(0)
    , FLcurrent(0)
    , BRcurrent(0)
    , FRcurrent(0)
    , BLvel(0)
    , FLvel(0)
    , BRvel(0)
    , FRvel(0)
    , chargeUsed(0)
    , current(0)
    , voltage(0)
    , latitude(0)
    , longitude(0)
    , roll(0)
    , pitch(0)
    , yaw(0)
    , boomCurrent(0)
    , armRotCurrent(0)
    , thumbPosition(0)
    , clawPosition(0)
    , stickPosition(0)
    , boomPosition(0)
    , armRotPosition(0)
    , panPosition(0)
    , tiltPosition(0)
    , satilites(0)
    , CBoomPosition(0)  {
    }



   typedef int32_t _timeUp_type;
  _timeUp_type timeUp;

   typedef int32_t _BLcurrent_type;
  _BLcurrent_type BLcurrent;

   typedef int32_t _FLcurrent_type;
  _FLcurrent_type FLcurrent;

   typedef int32_t _BRcurrent_type;
  _BRcurrent_type BRcurrent;

   typedef int32_t _FRcurrent_type;
  _FRcurrent_type FRcurrent;

   typedef int32_t _BLvel_type;
  _BLvel_type BLvel;

   typedef int32_t _FLvel_type;
  _FLvel_type FLvel;

   typedef int32_t _BRvel_type;
  _BRvel_type BRvel;

   typedef int32_t _FRvel_type;
  _FRvel_type FRvel;

   typedef int32_t _chargeUsed_type;
  _chargeUsed_type chargeUsed;

   typedef int32_t _current_type;
  _current_type current;

   typedef int32_t _voltage_type;
  _voltage_type voltage;

   typedef int32_t _latitude_type;
  _latitude_type latitude;

   typedef int32_t _longitude_type;
  _longitude_type longitude;

   typedef int32_t _roll_type;
  _roll_type roll;

   typedef int32_t _pitch_type;
  _pitch_type pitch;

   typedef int32_t _yaw_type;
  _yaw_type yaw;

   typedef int32_t _boomCurrent_type;
  _boomCurrent_type boomCurrent;

   typedef int32_t _armRotCurrent_type;
  _armRotCurrent_type armRotCurrent;

   typedef int32_t _thumbPosition_type;
  _thumbPosition_type thumbPosition;

   typedef int32_t _clawPosition_type;
  _clawPosition_type clawPosition;

   typedef int32_t _stickPosition_type;
  _stickPosition_type stickPosition;

   typedef int32_t _boomPosition_type;
  _boomPosition_type boomPosition;

   typedef int32_t _armRotPosition_type;
  _armRotPosition_type armRotPosition;

   typedef int32_t _panPosition_type;
  _panPosition_type panPosition;

   typedef int32_t _tiltPosition_type;
  _tiltPosition_type tiltPosition;

   typedef int32_t _satilites_type;
  _satilites_type satilites;

   typedef int32_t _CBoomPosition_type;
  _CBoomPosition_type CBoomPosition;




  typedef boost::shared_ptr< ::Messages::NavigationMessage_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::Messages::NavigationMessage_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct NavigationMessage_

typedef ::Messages::NavigationMessage_<std::allocator<void> > NavigationMessage;

typedef boost::shared_ptr< ::Messages::NavigationMessage > NavigationMessagePtr;
typedef boost::shared_ptr< ::Messages::NavigationMessage const> NavigationMessageConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::Messages::NavigationMessage_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::Messages::NavigationMessage_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace Messages

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/groovy/share/std_msgs/cmake/../msg'], 'Messages': ['/home/roboops/2014Software/groovy2014/src/Messages/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::Messages::NavigationMessage_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::Messages::NavigationMessage_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::Messages::NavigationMessage_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::Messages::NavigationMessage_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::Messages::NavigationMessage_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::Messages::NavigationMessage_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::Messages::NavigationMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4e2301b8681f5ff9954abd06b7c53650";
  }

  static const char* value(const ::Messages::NavigationMessage_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4e2301b8681f5ff9ULL;
  static const uint64_t static_value2 = 0x954abd06b7c53650ULL;
};

template<class ContainerAllocator>
struct DataType< ::Messages::NavigationMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Messages/NavigationMessage";
  }

  static const char* value(const ::Messages::NavigationMessage_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::Messages::NavigationMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 timeUp\n\
int32 BLcurrent\n\
int32 FLcurrent\n\
int32 BRcurrent\n\
int32 FRcurrent\n\
int32 BLvel\n\
int32 FLvel\n\
int32 BRvel\n\
int32 FRvel\n\
int32 chargeUsed\n\
int32 current\n\
int32 voltage\n\
int32 latitude\n\
int32 longitude\n\
int32 roll\n\
int32 pitch\n\
int32 yaw\n\
int32 boomCurrent\n\
int32 armRotCurrent\n\
int32 thumbPosition\n\
int32 clawPosition\n\
int32 stickPosition\n\
int32 boomPosition\n\
int32 armRotPosition\n\
int32 panPosition\n\
int32 tiltPosition\n\
int32 satilites\n\
int32 CBoomPosition\n\
";
  }

  static const char* value(const ::Messages::NavigationMessage_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::Messages::NavigationMessage_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.timeUp);
      stream.next(m.BLcurrent);
      stream.next(m.FLcurrent);
      stream.next(m.BRcurrent);
      stream.next(m.FRcurrent);
      stream.next(m.BLvel);
      stream.next(m.FLvel);
      stream.next(m.BRvel);
      stream.next(m.FRvel);
      stream.next(m.chargeUsed);
      stream.next(m.current);
      stream.next(m.voltage);
      stream.next(m.latitude);
      stream.next(m.longitude);
      stream.next(m.roll);
      stream.next(m.pitch);
      stream.next(m.yaw);
      stream.next(m.boomCurrent);
      stream.next(m.armRotCurrent);
      stream.next(m.thumbPosition);
      stream.next(m.clawPosition);
      stream.next(m.stickPosition);
      stream.next(m.boomPosition);
      stream.next(m.armRotPosition);
      stream.next(m.panPosition);
      stream.next(m.tiltPosition);
      stream.next(m.satilites);
      stream.next(m.CBoomPosition);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct NavigationMessage_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::Messages::NavigationMessage_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::Messages::NavigationMessage_<ContainerAllocator>& v)
  {
    s << indent << "timeUp: ";
    Printer<int32_t>::stream(s, indent + "  ", v.timeUp);
    s << indent << "BLcurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.BLcurrent);
    s << indent << "FLcurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.FLcurrent);
    s << indent << "BRcurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.BRcurrent);
    s << indent << "FRcurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.FRcurrent);
    s << indent << "BLvel: ";
    Printer<int32_t>::stream(s, indent + "  ", v.BLvel);
    s << indent << "FLvel: ";
    Printer<int32_t>::stream(s, indent + "  ", v.FLvel);
    s << indent << "BRvel: ";
    Printer<int32_t>::stream(s, indent + "  ", v.BRvel);
    s << indent << "FRvel: ";
    Printer<int32_t>::stream(s, indent + "  ", v.FRvel);
    s << indent << "chargeUsed: ";
    Printer<int32_t>::stream(s, indent + "  ", v.chargeUsed);
    s << indent << "current: ";
    Printer<int32_t>::stream(s, indent + "  ", v.current);
    s << indent << "voltage: ";
    Printer<int32_t>::stream(s, indent + "  ", v.voltage);
    s << indent << "latitude: ";
    Printer<int32_t>::stream(s, indent + "  ", v.latitude);
    s << indent << "longitude: ";
    Printer<int32_t>::stream(s, indent + "  ", v.longitude);
    s << indent << "roll: ";
    Printer<int32_t>::stream(s, indent + "  ", v.roll);
    s << indent << "pitch: ";
    Printer<int32_t>::stream(s, indent + "  ", v.pitch);
    s << indent << "yaw: ";
    Printer<int32_t>::stream(s, indent + "  ", v.yaw);
    s << indent << "boomCurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.boomCurrent);
    s << indent << "armRotCurrent: ";
    Printer<int32_t>::stream(s, indent + "  ", v.armRotCurrent);
    s << indent << "thumbPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.thumbPosition);
    s << indent << "clawPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.clawPosition);
    s << indent << "stickPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.stickPosition);
    s << indent << "boomPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.boomPosition);
    s << indent << "armRotPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.armRotPosition);
    s << indent << "panPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.panPosition);
    s << indent << "tiltPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.tiltPosition);
    s << indent << "satilites: ";
    Printer<int32_t>::stream(s, indent + "  ", v.satilites);
    s << indent << "CBoomPosition: ";
    Printer<int32_t>::stream(s, indent + "  ", v.CBoomPosition);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MESSAGES_MESSAGE_NAVIGATIONMESSAGE_H
