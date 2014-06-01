"""autogenerated by genpy from Messages/PilotMessage.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class PilotMessage(genpy.Message):
  _md5sum = "e685780fe831c819df93ab3b7f4dee3d"
  _type = "Messages/PilotMessage"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 timeUp
int32 batteryVoltage
int32 chargeUsed
int32 latitude
int32 longitude
int32 roll
int32 pitch
int32 yaw

"""
  __slots__ = ['timeUp','batteryVoltage','chargeUsed','latitude','longitude','roll','pitch','yaw']
  _slot_types = ['int32','int32','int32','int32','int32','int32','int32','int32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       timeUp,batteryVoltage,chargeUsed,latitude,longitude,roll,pitch,yaw

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(PilotMessage, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.timeUp is None:
        self.timeUp = 0
      if self.batteryVoltage is None:
        self.batteryVoltage = 0
      if self.chargeUsed is None:
        self.chargeUsed = 0
      if self.latitude is None:
        self.latitude = 0
      if self.longitude is None:
        self.longitude = 0
      if self.roll is None:
        self.roll = 0
      if self.pitch is None:
        self.pitch = 0
      if self.yaw is None:
        self.yaw = 0
    else:
      self.timeUp = 0
      self.batteryVoltage = 0
      self.chargeUsed = 0
      self.latitude = 0
      self.longitude = 0
      self.roll = 0
      self.pitch = 0
      self.yaw = 0

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_8i.pack(_x.timeUp, _x.batteryVoltage, _x.chargeUsed, _x.latitude, _x.longitude, _x.roll, _x.pitch, _x.yaw))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 32
      (_x.timeUp, _x.batteryVoltage, _x.chargeUsed, _x.latitude, _x.longitude, _x.roll, _x.pitch, _x.yaw,) = _struct_8i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_8i.pack(_x.timeUp, _x.batteryVoltage, _x.chargeUsed, _x.latitude, _x.longitude, _x.roll, _x.pitch, _x.yaw))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 32
      (_x.timeUp, _x.batteryVoltage, _x.chargeUsed, _x.latitude, _x.longitude, _x.roll, _x.pitch, _x.yaw,) = _struct_8i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_8i = struct.Struct("<8i")