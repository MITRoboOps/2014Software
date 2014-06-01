; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude ArmStatusMessage.msg.html

(cl:defclass <ArmStatusMessage> (roslisp-msg-protocol:ros-message)
  ((boomCurrent
    :reader boomCurrent
    :initarg :boomCurrent
    :type cl:integer
    :initform 0)
   (armRotCurrent
    :reader armRotCurrent
    :initarg :armRotCurrent
    :type cl:integer
    :initform 0)
   (thumbPosition
    :reader thumbPosition
    :initarg :thumbPosition
    :type cl:integer
    :initform 0)
   (clawPosition
    :reader clawPosition
    :initarg :clawPosition
    :type cl:integer
    :initform 0)
   (stickPosition
    :reader stickPosition
    :initarg :stickPosition
    :type cl:integer
    :initform 0)
   (boomPosition
    :reader boomPosition
    :initarg :boomPosition
    :type cl:integer
    :initform 0)
   (armRotPosition
    :reader armRotPosition
    :initarg :armRotPosition
    :type cl:integer
    :initform 0)
   (panPosition
    :reader panPosition
    :initarg :panPosition
    :type cl:integer
    :initform 0)
   (tiltPosition
    :reader tiltPosition
    :initarg :tiltPosition
    :type cl:integer
    :initform 0))
)

(cl:defclass ArmStatusMessage (<ArmStatusMessage>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ArmStatusMessage>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ArmStatusMessage)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<ArmStatusMessage> is deprecated: use Messages-msg:ArmStatusMessage instead.")))

(cl:ensure-generic-function 'boomCurrent-val :lambda-list '(m))
(cl:defmethod boomCurrent-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:boomCurrent-val is deprecated.  Use Messages-msg:boomCurrent instead.")
  (boomCurrent m))

(cl:ensure-generic-function 'armRotCurrent-val :lambda-list '(m))
(cl:defmethod armRotCurrent-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:armRotCurrent-val is deprecated.  Use Messages-msg:armRotCurrent instead.")
  (armRotCurrent m))

(cl:ensure-generic-function 'thumbPosition-val :lambda-list '(m))
(cl:defmethod thumbPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:thumbPosition-val is deprecated.  Use Messages-msg:thumbPosition instead.")
  (thumbPosition m))

(cl:ensure-generic-function 'clawPosition-val :lambda-list '(m))
(cl:defmethod clawPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:clawPosition-val is deprecated.  Use Messages-msg:clawPosition instead.")
  (clawPosition m))

(cl:ensure-generic-function 'stickPosition-val :lambda-list '(m))
(cl:defmethod stickPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:stickPosition-val is deprecated.  Use Messages-msg:stickPosition instead.")
  (stickPosition m))

(cl:ensure-generic-function 'boomPosition-val :lambda-list '(m))
(cl:defmethod boomPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:boomPosition-val is deprecated.  Use Messages-msg:boomPosition instead.")
  (boomPosition m))

(cl:ensure-generic-function 'armRotPosition-val :lambda-list '(m))
(cl:defmethod armRotPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:armRotPosition-val is deprecated.  Use Messages-msg:armRotPosition instead.")
  (armRotPosition m))

(cl:ensure-generic-function 'panPosition-val :lambda-list '(m))
(cl:defmethod panPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:panPosition-val is deprecated.  Use Messages-msg:panPosition instead.")
  (panPosition m))

(cl:ensure-generic-function 'tiltPosition-val :lambda-list '(m))
(cl:defmethod tiltPosition-val ((m <ArmStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:tiltPosition-val is deprecated.  Use Messages-msg:tiltPosition instead.")
  (tiltPosition m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ArmStatusMessage>) ostream)
  "Serializes a message object of type '<ArmStatusMessage>"
  (cl:let* ((signed (cl:slot-value msg 'boomCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'armRotCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'thumbPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'clawPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'stickPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'boomPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'armRotPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'panPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'tiltPosition)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ArmStatusMessage>) istream)
  "Deserializes a message object of type '<ArmStatusMessage>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'boomCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'armRotCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'thumbPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'clawPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stickPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'boomPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'armRotPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'panPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'tiltPosition) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ArmStatusMessage>)))
  "Returns string type for a message object of type '<ArmStatusMessage>"
  "Messages/ArmStatusMessage")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ArmStatusMessage)))
  "Returns string type for a message object of type 'ArmStatusMessage"
  "Messages/ArmStatusMessage")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ArmStatusMessage>)))
  "Returns md5sum for a message object of type '<ArmStatusMessage>"
  "39dd7004df49d35f5a38291f8b4a5676")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ArmStatusMessage)))
  "Returns md5sum for a message object of type 'ArmStatusMessage"
  "39dd7004df49d35f5a38291f8b4a5676")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ArmStatusMessage>)))
  "Returns full string definition for message of type '<ArmStatusMessage>"
  (cl:format cl:nil "int32 boomCurrent~%int32 armRotCurrent~%int32 thumbPosition~%int32 clawPosition~%int32 stickPosition~%int32 boomPosition~%int32 armRotPosition~%int32 panPosition~%int32 tiltPosition~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ArmStatusMessage)))
  "Returns full string definition for message of type 'ArmStatusMessage"
  (cl:format cl:nil "int32 boomCurrent~%int32 armRotCurrent~%int32 thumbPosition~%int32 clawPosition~%int32 stickPosition~%int32 boomPosition~%int32 armRotPosition~%int32 panPosition~%int32 tiltPosition~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ArmStatusMessage>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ArmStatusMessage>))
  "Converts a ROS message object to a list"
  (cl:list 'ArmStatusMessage
    (cl:cons ':boomCurrent (boomCurrent msg))
    (cl:cons ':armRotCurrent (armRotCurrent msg))
    (cl:cons ':thumbPosition (thumbPosition msg))
    (cl:cons ':clawPosition (clawPosition msg))
    (cl:cons ':stickPosition (stickPosition msg))
    (cl:cons ':boomPosition (boomPosition msg))
    (cl:cons ':armRotPosition (armRotPosition msg))
    (cl:cons ':panPosition (panPosition msg))
    (cl:cons ':tiltPosition (tiltPosition msg))
))
