; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude DriveTrainStatusMessage.msg.html

(cl:defclass <DriveTrainStatusMessage> (roslisp-msg-protocol:ros-message)
  ((BLCurrent
    :reader BLCurrent
    :initarg :BLCurrent
    :type cl:integer
    :initform 0)
   (FLCurrent
    :reader FLCurrent
    :initarg :FLCurrent
    :type cl:integer
    :initform 0)
   (BRCurrent
    :reader BRCurrent
    :initarg :BRCurrent
    :type cl:integer
    :initform 0)
   (FRCurrent
    :reader FRCurrent
    :initarg :FRCurrent
    :type cl:integer
    :initform 0)
   (BLVelocity
    :reader BLVelocity
    :initarg :BLVelocity
    :type cl:integer
    :initform 0)
   (FLVelocity
    :reader FLVelocity
    :initarg :FLVelocity
    :type cl:integer
    :initform 0)
   (BRVelocity
    :reader BRVelocity
    :initarg :BRVelocity
    :type cl:integer
    :initform 0)
   (FRVelocity
    :reader FRVelocity
    :initarg :FRVelocity
    :type cl:integer
    :initform 0)
   (Voltage1
    :reader Voltage1
    :initarg :Voltage1
    :type cl:integer
    :initform 0))
)

(cl:defclass DriveTrainStatusMessage (<DriveTrainStatusMessage>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DriveTrainStatusMessage>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DriveTrainStatusMessage)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<DriveTrainStatusMessage> is deprecated: use Messages-msg:DriveTrainStatusMessage instead.")))

(cl:ensure-generic-function 'BLCurrent-val :lambda-list '(m))
(cl:defmethod BLCurrent-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:BLCurrent-val is deprecated.  Use Messages-msg:BLCurrent instead.")
  (BLCurrent m))

(cl:ensure-generic-function 'FLCurrent-val :lambda-list '(m))
(cl:defmethod FLCurrent-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:FLCurrent-val is deprecated.  Use Messages-msg:FLCurrent instead.")
  (FLCurrent m))

(cl:ensure-generic-function 'BRCurrent-val :lambda-list '(m))
(cl:defmethod BRCurrent-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:BRCurrent-val is deprecated.  Use Messages-msg:BRCurrent instead.")
  (BRCurrent m))

(cl:ensure-generic-function 'FRCurrent-val :lambda-list '(m))
(cl:defmethod FRCurrent-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:FRCurrent-val is deprecated.  Use Messages-msg:FRCurrent instead.")
  (FRCurrent m))

(cl:ensure-generic-function 'BLVelocity-val :lambda-list '(m))
(cl:defmethod BLVelocity-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:BLVelocity-val is deprecated.  Use Messages-msg:BLVelocity instead.")
  (BLVelocity m))

(cl:ensure-generic-function 'FLVelocity-val :lambda-list '(m))
(cl:defmethod FLVelocity-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:FLVelocity-val is deprecated.  Use Messages-msg:FLVelocity instead.")
  (FLVelocity m))

(cl:ensure-generic-function 'BRVelocity-val :lambda-list '(m))
(cl:defmethod BRVelocity-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:BRVelocity-val is deprecated.  Use Messages-msg:BRVelocity instead.")
  (BRVelocity m))

(cl:ensure-generic-function 'FRVelocity-val :lambda-list '(m))
(cl:defmethod FRVelocity-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:FRVelocity-val is deprecated.  Use Messages-msg:FRVelocity instead.")
  (FRVelocity m))

(cl:ensure-generic-function 'Voltage1-val :lambda-list '(m))
(cl:defmethod Voltage1-val ((m <DriveTrainStatusMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:Voltage1-val is deprecated.  Use Messages-msg:Voltage1 instead.")
  (Voltage1 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DriveTrainStatusMessage>) ostream)
  "Serializes a message object of type '<DriveTrainStatusMessage>"
  (cl:let* ((signed (cl:slot-value msg 'BLCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'FLCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'BRCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'FRCurrent)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'BLVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'FLVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'BRVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'FRVelocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'Voltage1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DriveTrainStatusMessage>) istream)
  "Deserializes a message object of type '<DriveTrainStatusMessage>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'BLCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'FLCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'BRCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'FRCurrent) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'BLVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'FLVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'BRVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'FRVelocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Voltage1) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DriveTrainStatusMessage>)))
  "Returns string type for a message object of type '<DriveTrainStatusMessage>"
  "Messages/DriveTrainStatusMessage")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DriveTrainStatusMessage)))
  "Returns string type for a message object of type 'DriveTrainStatusMessage"
  "Messages/DriveTrainStatusMessage")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DriveTrainStatusMessage>)))
  "Returns md5sum for a message object of type '<DriveTrainStatusMessage>"
  "e1afd2d5a92d4dbd48c700a5e7e6912c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DriveTrainStatusMessage)))
  "Returns md5sum for a message object of type 'DriveTrainStatusMessage"
  "e1afd2d5a92d4dbd48c700a5e7e6912c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DriveTrainStatusMessage>)))
  "Returns full string definition for message of type '<DriveTrainStatusMessage>"
  (cl:format cl:nil "int32 BLCurrent~%int32 FLCurrent~%int32 BRCurrent~%int32 FRCurrent~%int32 BLVelocity~%int32 FLVelocity~%int32 BRVelocity~%int32 FRVelocity~%int32 Voltage1~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DriveTrainStatusMessage)))
  "Returns full string definition for message of type 'DriveTrainStatusMessage"
  (cl:format cl:nil "int32 BLCurrent~%int32 FLCurrent~%int32 BRCurrent~%int32 FRCurrent~%int32 BLVelocity~%int32 FLVelocity~%int32 BRVelocity~%int32 FRVelocity~%int32 Voltage1~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DriveTrainStatusMessage>))
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
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DriveTrainStatusMessage>))
  "Converts a ROS message object to a list"
  (cl:list 'DriveTrainStatusMessage
    (cl:cons ':BLCurrent (BLCurrent msg))
    (cl:cons ':FLCurrent (FLCurrent msg))
    (cl:cons ':BRCurrent (BRCurrent msg))
    (cl:cons ':FRCurrent (FRCurrent msg))
    (cl:cons ':BLVelocity (BLVelocity msg))
    (cl:cons ':FLVelocity (FLVelocity msg))
    (cl:cons ':BRVelocity (BRVelocity msg))
    (cl:cons ':FRVelocity (FRVelocity msg))
    (cl:cons ':Voltage1 (Voltage1 msg))
))
