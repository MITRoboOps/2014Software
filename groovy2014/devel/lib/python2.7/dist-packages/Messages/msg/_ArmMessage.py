"""autogenerated by genpy from Messages/ArmMessage.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class ArmMessage(genpy.Message):
  _md5sum = "4614ce691507468218fe21642c3bab91"
  _type = "Messages/ArmMessage"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 command
int32 thumb
int32 claw
int32 stick
int32 boom
int32 armRot
int32 pan
int32 tilt

"""
  __slots__ = ['command','thumb','claw','stick','boom','armRot','pan','tilt']
  _slot_types = ['int32','int32','int32','int32','int32','int32','int32','int32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       command,thumb,claw,stick,boom,armRot,pan,tilt

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(ArmMessage, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.command is None:
        self.command = 0
      if self.thumb is None:
        self.thumb = 0
      if self.claw is None:
        self.claw = 0
      if self.stick is None:
        self.stick = 0
      if self.boom is None:
        self.boom = 0
      if self.armRot is None:
        self.armRot = 0
      if self.pan is None:
        self.pan = 0
      if self.tilt is None:
        self.tilt = 0
    else:
      self.command = 0
      self.thumb = 0
      self.claw = 0
      self.stick = 0
      self.boom = 0
      self.armRot = 0
      self.pan = 0
      self.tilt = 0

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
      buff.write(_struct_8i.pack(_x.command, _x.thumb, _x.claw, _x.stick, _x.boom, _x.armRot, _x.pan, _x.tilt))
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
      (_x.command, _x.thumb, _x.claw, _x.stick, _x.boom, _x.armRot, _x.pan, _x.tilt,) = _struct_8i.unpack(str[start:end])
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
      buff.write(_struct_8i.pack(_x.command, _x.thumb, _x.claw, _x.stick, _x.boom, _x.armRot, _x.pan, _x.tilt))
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
      (_x.command, _x.thumb, _x.claw, _x.stick, _x.boom, _x.armRot, _x.pan, _x.tilt,) = _struct_8i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_8i = struct.Struct("<8i")
