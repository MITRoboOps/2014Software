"""autogenerated by genpy from Messages/DriveTrainStatusMessage.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class DriveTrainStatusMessage(genpy.Message):
  _md5sum = "e1afd2d5a92d4dbd48c700a5e7e6912c"
  _type = "Messages/DriveTrainStatusMessage"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 BLCurrent
int32 FLCurrent
int32 BRCurrent
int32 FRCurrent
int32 BLVelocity
int32 FLVelocity
int32 BRVelocity
int32 FRVelocity
int32 Voltage1

"""
  __slots__ = ['BLCurrent','FLCurrent','BRCurrent','FRCurrent','BLVelocity','FLVelocity','BRVelocity','FRVelocity','Voltage1']
  _slot_types = ['int32','int32','int32','int32','int32','int32','int32','int32','int32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       BLCurrent,FLCurrent,BRCurrent,FRCurrent,BLVelocity,FLVelocity,BRVelocity,FRVelocity,Voltage1

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(DriveTrainStatusMessage, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.BLCurrent is None:
        self.BLCurrent = 0
      if self.FLCurrent is None:
        self.FLCurrent = 0
      if self.BRCurrent is None:
        self.BRCurrent = 0
      if self.FRCurrent is None:
        self.FRCurrent = 0
      if self.BLVelocity is None:
        self.BLVelocity = 0
      if self.FLVelocity is None:
        self.FLVelocity = 0
      if self.BRVelocity is None:
        self.BRVelocity = 0
      if self.FRVelocity is None:
        self.FRVelocity = 0
      if self.Voltage1 is None:
        self.Voltage1 = 0
    else:
      self.BLCurrent = 0
      self.FLCurrent = 0
      self.BRCurrent = 0
      self.FRCurrent = 0
      self.BLVelocity = 0
      self.FLVelocity = 0
      self.BRVelocity = 0
      self.FRVelocity = 0
      self.Voltage1 = 0

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
      buff.write(_struct_9i.pack(_x.BLCurrent, _x.FLCurrent, _x.BRCurrent, _x.FRCurrent, _x.BLVelocity, _x.FLVelocity, _x.BRVelocity, _x.FRVelocity, _x.Voltage1))
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
      end += 36
      (_x.BLCurrent, _x.FLCurrent, _x.BRCurrent, _x.FRCurrent, _x.BLVelocity, _x.FLVelocity, _x.BRVelocity, _x.FRVelocity, _x.Voltage1,) = _struct_9i.unpack(str[start:end])
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
      buff.write(_struct_9i.pack(_x.BLCurrent, _x.FLCurrent, _x.BRCurrent, _x.FRCurrent, _x.BLVelocity, _x.FLVelocity, _x.BRVelocity, _x.FRVelocity, _x.Voltage1))
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
      end += 36
      (_x.BLCurrent, _x.FLCurrent, _x.BRCurrent, _x.FRCurrent, _x.BLVelocity, _x.FLVelocity, _x.BRVelocity, _x.FRVelocity, _x.Voltage1,) = _struct_9i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_9i = struct.Struct("<9i")