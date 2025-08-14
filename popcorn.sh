# Docker environment variables
export DOCK_VER=1.2.16
export DOCK_SRC_NAME=salespopcornsar/edu:$DOCK_VER
export DOCK_NAME=edu_$DOCK_VER

# User Info (필요 시 수정)
export USER_NAME=$(whoami)
export USER_HOME=$(eval echo ~$USER_NAME)
export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


# Run Docker (sdv-adaptive-application 폴더 지정)
docker run --privileged -d -p 41003:22 -v /var/run/docker.sock:/var/run/docker.sock \
    -v $SCRIPT_DIR:/home/popcornsar/sdv \
    -v $USER_HOME/appdata:/appdata \
    -v $USER_HOME/bin:/home/popcornsar/bin \
    --shm-size 1g --user popcornsar --name $DOCK_NAME $DOCK_SRC_NAME

# Start SSH Service
docker exec -it $DOCK_NAME sudo service ssh start
docker exec -it $DOCK_NAME mkdir /home/popcornsar/.ssh

# Install essential library
docker exec -it $DOCK_NAME sudo apt-get update
docker exec -it $DOCK_NAME sudo apt-get install -y iptables iputils-ping libevent-dev libssl-dev libjpeg-dev libpng-dev file locales build-essential libcurl4-openssl-dev nlohmann-json3-dev
docker exec -it $DOCK_NAME sudo sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
docker exec -it $DOCK_NAME sudo locale-gen
docker exec -it $DOCK_NAME sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 LANGUAGE=en_US:en

# change passwd 0
echo "popcornsar:0" | docker exec -i --user root $DOCK_NAME chpasswd

