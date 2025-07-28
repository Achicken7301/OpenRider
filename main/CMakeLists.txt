# file(GLOB_RECURSE SRCS  "*.c")
# idf_component_register(SRCS ${SRCS} 
idf_component_register(SRCS "espnow_driver.c" "main.c"
"intercom_fsm.c"
"peer_manager.c"
"scheduler.c"
"simple_log.c"
"wireless_comm.c"
INCLUDE_DIRS "." "include")