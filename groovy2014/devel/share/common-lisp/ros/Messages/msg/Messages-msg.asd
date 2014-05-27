
(cl:in-package :asdf)

(defsystem "Messages-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "DriveTrainMessage" :depends-on ("_package_DriveTrainMessage"))
    (:file "_package_DriveTrainMessage" :depends-on ("_package"))
    (:file "ArmMessage" :depends-on ("_package_ArmMessage"))
    (:file "_package_ArmMessage" :depends-on ("_package"))
  ))