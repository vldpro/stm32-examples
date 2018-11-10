BUILD_IMG_NAME=stm32build_env
DOCKER_PROJECT_DIR=/usr/src/stm32_app
DOCKER_BUILD_LOG=/tmp/${BUILD_IMG_NAME}_docker-build.log
BUILD_LOG=/tmp/${BUILD_IMG_NAME}_gcc-build.log


echo "INFO: docker-image: building..."
docker build -t ${BUILD_IMG_NAME} . >${DOCKER_BUILD_LOG} 2>&1 \
|| { echo "ERR: docker-image: Build failed. See ${DOCKER_BUILD_LOG}" >&2 && exit 1; }
echo "INFO: docker-image: done."


echo "INFO: project: building..."
docker run --rm -v ${PWD}:${DOCKER_PROJECT_DIR} \
                -w ${DOCKER_PROJECT_DIR} ${BUILD_IMG_NAME} make \
                >${BUILD_LOG} 2>&1 \
|| { echo "ERR: project: Build failed.See ${BUILD_LOG}" >&2 && exit 1; }
echo "INFO: project: done"
