; Auto-generated. Do not edit!


(cl:in-package Messages-msg)


;//! \htmlinclude CameraBoomMessage.msg.html

(cl:defclass <CameraBoomMessage> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:integer
    :initform 0))
)

(cl:defclass CameraBoomMessage (<CameraBoomMessage>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CameraBoomMessage>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CameraBoomMessage)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Messages-msg:<CameraBoomMessage> is deprecated: use Messages-msg:CameraBoomMessage instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <CameraBoomMessage>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Messages-msg:command-val is deprecated.  Use Messages-msg:command instead.")
  (command m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CameraBoomMessage>) ostream)
  "Serializes a message object of type '<CameraBoomMessage>"
  (cl:let* ((signed (cl:slot-value msg 'command)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CameraBoomMessage>) istream)
  "Deserializes a message object of type '<CameraBoomMessage>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CameraBoomMessage>)))
  "Returns string type for a message object of type '<CameraBoomMessage>"
  "Messages/CameraBoomMessage")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CameraBoomMessage)))
  "Returns string type for a message object of type 'CameraBoomMessage"
  "Messages/CameraBoomMessage")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CameraBoomMessage>)))
  "Returns md5sum for a message object of type '<CameraBoomMessage>"
  "3a54bc0c5f4abe9ad44d29292ec0800d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CameraBoomMessage)))
  "Returns md5sum for a message object of type 'CameraBoomMessage"
  "3a54bc0c5f4abe9ad44d29292ec0800d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CameraBoomMessage>)))
  "Returns full string definition for message of type '<CameraBoomMessage>"
  (cl:format cl:nil "int32 command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CameraBoomMessage)))
  "Returns full string definition for message of type 'CameraBoomMessage"
  (cl:format cl:nil "int32 command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CameraBoomMessage>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CameraBoomMessage>))
  "Converts a ROS message object to a list"
  (cl:list 'CameraBoomMessage
    (cl:cons ':command (command msg))
))
