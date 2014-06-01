; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude CameraBoomStatus.msg.html

(cl:defclass <CameraBoomStatus> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type cl:integer
    :initform 0))
)

(cl:defclass CameraBoomStatus (<CameraBoomStatus>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CameraBoomStatus>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CameraBoomStatus)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<CameraBoomStatus> is deprecated: use Messages-msg:CameraBoomStatus instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <CameraBoomStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:position-val is deprecated.  Use Messages-msg:position instead.")
  (position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CameraBoomStatus>) ostream)
  "Serializes a message object of type '<CameraBoomStatus>"
  (cl:let* ((signed (cl:slot-value msg 'position)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CameraBoomStatus>) istream)
  "Deserializes a message object of type '<CameraBoomStatus>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'position) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CameraBoomStatus>)))
  "Returns string type for a message object of type '<CameraBoomStatus>"
  "Messages/CameraBoomStatus")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CameraBoomStatus)))
  "Returns string type for a message object of type 'CameraBoomStatus"
  "Messages/CameraBoomStatus")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CameraBoomStatus>)))
  "Returns md5sum for a message object of type '<CameraBoomStatus>"
  "ada70156e12e6e31948c64c60d8bb212")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CameraBoomStatus)))
  "Returns md5sum for a message object of type 'CameraBoomStatus"
  "ada70156e12e6e31948c64c60d8bb212")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CameraBoomStatus>)))
  "Returns full string definition for message of type '<CameraBoomStatus>"
  (cl:format cl:nil "int32 position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CameraBoomStatus)))
  "Returns full string definition for message of type 'CameraBoomStatus"
  (cl:format cl:nil "int32 position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CameraBoomStatus>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CameraBoomStatus>))
  "Converts a ROS message object to a list"
  (cl:list 'CameraBoomStatus
    (cl:cons ':position (position msg))
))
