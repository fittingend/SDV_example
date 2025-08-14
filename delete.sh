# Docker environment variables
export DOCK_VER=1.2.16
export DOCK_SRC_NAME=salespopcornsar/edu:$DOCK_VER
export DOCK_NAME=edu_$DOCK_VER

docker rm -f $DOCK_NAME
docker ps -a
