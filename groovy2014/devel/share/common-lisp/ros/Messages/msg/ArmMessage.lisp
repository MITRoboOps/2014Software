; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude ArmMessage.msg.html

(cl:defclass <ArmMessage> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:integer
    :initform 0)
   (thumb
    :reader thumb
    :initarg :thumb
    :type cl:integer
    :initform 0)
   (claw
    :reader claw
    :initarg :claw
    :type cl:integer
    :initform 0)
   (stick
    :reader stick
    :initarg :stick
    :type cl:integer
    :initform 0)
   (boom
    :reader boom
    :initarg :boom
    :type cl:integer
    :initform 0)
   (armRot
    :reader armRot
    :initarg :armRot
    :type cl:integer
    :initform 0)
   (pan
    :reader pan
    :initarg :pan
    :type cl:integer
    :initform 0)
   (tilt
    :reader tilt
    :initarg :tilt
    :type cl:integer
    :initform 0))
)

(cl:defclass ArmMessage (<ArmMessage>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ArmMessage>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ArmMessage)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<ArmMessage> is deprecated: use Messages-msg:ArmMessage instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:command-val is deprecated.  Use Messages-msg:command instead.")
  (command m))

(cl:ensure-generic-function 'thumb-val :lambda-list '(m))
(cl:defmethod thumb-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:thumb-val is deprecated.  Use Messages-msg:thumb instead.")
  (thumb m))

(cl:ensure-generic-function 'claw-val :lambda-list '(m))
(cl:defmethod claw-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:claw-val is deprecated.  Use Messages-msg:claw instead.")
  (claw m))

(cl:ensure-generic-function 'stick-val :lambda-list '(m))
(cl:defmethod stick-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:stick-val is deprecated.  Use Messages-msg:stick instead.")
  (stick m))

(cl:ensure-generic-function 'boom-val :lambda-list '(m))
(cl:defmethod boom-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:boom-val is deprecated.  Use Messages-msg:boom instead.")
  (boom m))

(cl:ensure-generic-function 'armRot-val :lambda-list '(m))
(cl:defmethod armRot-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:armRot-val is deprecated.  Use Messages-msg:armRot instead.")
  (armRot m))

(cl:ensure-generic-function 'pan-val :lambda-list '(m))
(cl:defmethod pan-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:pan-val is deprecated.  Use Messages-msg:pan instead.")
  (pan m))

(cl:ensure-generic-function 'tilt-val :lambda-list '(m))
(cl:defmethod tilt-val ((m <ArmMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:tilt-val is deprecated.  Use Messages-msg:tilt instead.")
  (tilt m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ArmMessage>) ostream)
  "Serializes a message object of type '<ArmMessage>"
  (cl:let* ((signed (cl:slot-value msg 'command)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'thumb)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'claw)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'stick)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'boom)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'armRot)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'pan)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'tilt)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ArmMessage>) istream)
  "Deserializes a message object of type '<ArmMessage>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'thumb) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'claw) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stick) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'boom) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'armRot) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'pan) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'tilt) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ArmMessage>)))
  "Returns string type for a message object of type '<ArmMessage>"
  "Messages/ArmMessage")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ArmMessage)))
  "Returns string type for a message object of type 'ArmMessage"
  "Messages/ArmMessage")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ArmMessage>)))
  "Returns md5sum for a message object of type '<ArmMessage>"
  "4614ce691507468218fe21642c3bab91")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ArmMessage)))
  "Returns md5sum for a message object of type 'ArmMessage"
  "4614ce691507468218fe21642c3bab91")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ArmMessage>)))
  "Returns full string definition for message of type '<ArmMessage>"
  (cl:format cl:nil "int32 command~%int32 thumb~%int32 claw~%int32 stick~%int32 boom~%int32 armRot~%int32 pan~%int32 tilt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ArmMessage)))
  "Returns full string definition for message of type 'ArmMessage"
  (cl:format cl:nil "int32 command~%int32 thumb~%int32 claw~%int32 stick~%int32 boom~%int32 armRot~%int32 pan~%int32 tilt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ArmMessage>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ArmMessage>))
  "Converts a ROS message object to a list"
  (cl:list 'ArmMessage
    (cl:cons ':command (command msg))
    (cl:cons ':thumb (thumb msg))
    (cl:cons ':claw (claw msg))
    (cl:cons ':stick (stick msg))
    (cl:cons ':boom (boom msg))
    (cl:cons ':armRot (armRot msg))
    (cl:cons ':pan (pan msg))
    (cl:cons ':tilt (tilt msg))
))
