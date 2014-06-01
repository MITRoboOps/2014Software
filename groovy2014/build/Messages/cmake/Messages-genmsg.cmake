# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "Messages: 9 messages, 0 services")

set(MSG_I_FLAGS "-IMessages:/home/roboops/2014Software/groovy2014/src/Messages/msg;-Istd_msgs:/opt/ros/groovy/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(Messages_generate_messages ALL)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/PilotMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/GPSMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomStatus.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)
_generate_msg_cpp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/NavigationMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
)

### Generating Services

### Generating Module File
_generate_module_cpp(Messages
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(Messages_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(Messages_generate_messages Messages_generate_messages_cpp)

# target for backward compatibility
add_custom_target(Messages_gencpp)
add_dependencies(Messages_gencpp Messages_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS Messages_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/PilotMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/GPSMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomStatus.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)
_generate_msg_lisp(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/NavigationMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
)

### Generating Services

### Generating Module File
_generate_module_lisp(Messages
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(Messages_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(Messages_generate_messages Messages_generate_messages_lisp)

# target for backward compatibility
add_custom_target(Messages_genlisp)
add_dependencies(Messages_genlisp Messages_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS Messages_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/ArmStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/PilotMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainStatusMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/GPSMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/DriveTrainMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/CameraBoomStatus.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)
_generate_msg_py(Messages
  "/home/roboops/2014Software/groovy2014/src/Messages/msg/NavigationMessage.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
)

### Generating Services

### Generating Module File
_generate_module_py(Messages
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(Messages_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(Messages_generate_messages Messages_generate_messages_py)

# target for backward compatibility
add_custom_target(Messages_genpy)
add_dependencies(Messages_genpy Messages_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS Messages_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/Messages
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(Messages_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/Messages
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(Messages_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/Messages
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(Messages_generate_messages_py std_msgs_generate_messages_py)
