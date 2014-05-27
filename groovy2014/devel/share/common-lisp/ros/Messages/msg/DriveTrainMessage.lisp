; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude DriveTrainMessage.msg.html

(cl:defclass <DriveTrainMessage> (roslisp-msg-protocol:ros-message)
  ((leftVelocity
    :reader leftVelocity
    :initarg :leftVelocity
    :type cl:integer
    :initform 0)
   (rightVelocity
    :reader rightVelocity
    :initarg :rightVelocity
    :type cl:integer
    :initform 0))
)

(cl:defclass DriveTrainMessage (<DriveTrainMessage>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DriveTrainMessage>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DriveTrainMessage)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<DriveTrainMessage> is deprecated: use Messages-msg:DriveTrainMessage instead.")))

(cl:ensure-generic-function 'leftVelocity-val :lambda-list '(m))
(cl:defmethod leftVelocity-val ((m <DriveTrainMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:leftVelocity-val is deprecated.  Use Messages-msg:leftVelocity instead.")
  (leftVelocity m))

(cl:ensure-generic-function 'rightVelocity-val :lambda-list '(m))
(cl:defmethod rightVelocity-val ((m <DriveTrainMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:rightVelocity-val is deprecated.  Use Messages-msg:rightVelocity instead.")
  (rightVelocity m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DriveTrainMessage>) ostream)
  "Serializes a message object of type '<DriveTrainMessage>"
  (cl:let* ((signed (cl:slot-value msg 'leftVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'rightVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DriveTrainMessage>) istream)
  "Deserializes a message object of type '<DriveTrainMessage>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'leftVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'rightVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DriveTrainMessage>)))
  "Returns string type for a message object of type '<DriveTrainMessage>"
  "Messages/DriveTrainMessage")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DriveTrainMessage)))
  "Returns string type for a message object of type 'DriveTrainMessage"
  "Messages/DriveTrainMessage")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DriveTrainMessage>)))
  "Returns md5sum for a message object of type '<DriveTrainMessage>"
  "34da087c62253d83cd6ecbc7c1f2d16e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DriveTrainMessage)))
  "Returns md5sum for a message object of type 'DriveTrainMessage"
  "34da087c62253d83cd6ecbc7c1f2d16e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DriveTrainMessage>)))
  "Returns full string definition for message of type '<DriveTrainMessage>"
  (cl:format cl:nil "int32 leftVelocity~%int32 rightVelocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DriveTrainMessage)))
  "Returns full string definition for message of type 'DriveTrainMessage"
  (cl:format cl:nil "int32 leftVelocity~%int32 rightVelocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DriveTrainMessage>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DriveTrainMessage>))
  "Converts a ROS message object to a list"
  (cl:list 'DriveTrainMessage
    (cl:cons ':leftVelocity (leftVelocity msg))
    (cl:cons ':rightVelocity (rightVelocity msg))
))
