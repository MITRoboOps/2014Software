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
 * Auto-generated by genmsg_cpp from file /home/roboops/2014Software/groovy2014/src/Messages/msg/ArmMessage.msg
 *
 */


#ifndef MESSAGES_MESSAGE_ARMMESSAGE_H
#define MESSAGES_MESSAGE_ARMMESSAGE_H


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
struct ArmMessage_
{
  typedef ArmMessage_<ContainerAllocator> Type;

  ArmMessage_()
    : command(0)
    , thumb(0)
    , claw(0)
    , stick(0)
    , boom(0)
    , armRot(0)
    , pan(0)
    , tilt(0)  {
    }
  ArmMessage_(const ContainerAllocator& _alloc)
    : command(0)
    , thumb(0)
    , claw(0)
    , stick(0)
    , boom(0)
    , armRot(0)
    , pan(0)
    , tilt(0)  {
    }



   typedef int32_t _command_type;
  _command_type command;

   typedef int32_t _thumb_type;
  _thumb_type thumb;

   typedef int32_t _claw_type;
  _claw_type claw;

   typedef int32_t _stick_type;
  _stick_type stick;

   typedef int32_t _boom_type;
  _boom_type boom;

   typedef int32_t _armRot_type;
  _armRot_type armRot;

   typedef int32_t _pan_type;
  _pan_type pan;

   typedef int32_t _tilt_type;
  _tilt_type tilt;




  typedef boost::shared_ptr< ::Messages::ArmMessage_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::Messages::ArmMessage_<ContainerAllocator> const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

}; // struct ArmMessage_

typedef ::Messages::ArmMessage_<std::allocator<void> > ArmMessage;

typedef boost::shared_ptr< ::Messages::ArmMessage > ArmMessagePtr;
typedef boost::shared_ptr< ::Messages::ArmMessage const> ArmMessageConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::Messages::ArmMessage_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::Messages::ArmMessage_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::Messages::ArmMessage_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::Messages::ArmMessage_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::Messages::ArmMessage_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::Messages::ArmMessage_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::Messages::ArmMessage_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::Messages::ArmMessage_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::Messages::ArmMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4614ce691507468218fe21642c3bab91";
  }

  static const char* value(const ::Messages::ArmMessage_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4614ce6915074682ULL;
  static const uint64_t static_value2 = 0x18fe21642c3bab91ULL;
};

template<class ContainerAllocator>
struct DataType< ::Messages::ArmMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Messages/ArmMessage";
  }

  static const char* value(const ::Messages::ArmMessage_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::Messages::ArmMessage_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 command\n\
int32 thumb\n\
int32 claw\n\
int32 stick\n\
int32 boom\n\
int32 armRot\n\
int32 pan\n\
int32 tilt\n\
";
  }

  static const char* value(const ::Messages::ArmMessage_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::Messages::ArmMessage_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.command);
      stream.next(m.thumb);
      stream.next(m.claw);
      stream.next(m.stick);
      stream.next(m.boom);
      stream.next(m.armRot);
      stream.next(m.pan);
      stream.next(m.tilt);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct ArmMessage_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::Messages::ArmMessage_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::Messages::ArmMessage_<ContainerAllocator>& v)
  {
    s << indent << "command: ";
    Printer<int32_t>::stream(s, indent + "  ", v.command);
    s << indent << "thumb: ";
    Printer<int32_t>::stream(s, indent + "  ", v.thumb);
    s << indent << "claw: ";
    Printer<int32_t>::stream(s, indent + "  ", v.claw);
    s << indent << "stick: ";
    Printer<int32_t>::stream(s, indent + "  ", v.stick);
    s << indent << "boom: ";
    Printer<int32_t>::stream(s, indent + "  ", v.boom);
    s << indent << "armRot: ";
    Printer<int32_t>::stream(s, indent + "  ", v.armRot);
    s << indent << "pan: ";
    Printer<int32_t>::stream(s, indent + "  ", v.pan);
    s << indent << "tilt: ";
    Printer<int32_t>::stream(s, indent + "  ", v.tilt);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MESSAGES_MESSAGE_ARMMESSAGE_H
