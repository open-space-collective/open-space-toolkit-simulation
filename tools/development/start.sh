#!/bin/bash

# Apache License 2.0

if [[ -z ${project_directory} ]]; then
    echo "Variable [project_directory] is undefined."
    exit 1
fi

options=""
command="/bin/bash"

# Setup linked mode

if [[ ! -z ${1} ]] && [[ ${1} == "--link" ]]; then

    options=""
    command=""

    # Open Space Toolkit ▸ Core

    if [[ -z ${open_space_toolkit_core_directory} ]]; then
        echo "Variable [open_space_toolkit_core_directory] is undefined."
        exit 1
    fi

    if [[ ! -d ${open_space_toolkit_core_directory} ]]; then
        echo "Open Space Toolkit ▸ Core directory [${open_space_toolkit_core_directory}] cannot be found."
        exit 1
    fi

    options="${options} \
    --volume=${open_space_toolkit_core_directory}:/mnt/open-space-toolkit-core:ro"

    command=" \
    rm -rf /usr/local/include/OpenSpaceToolkit/Core; \
    rm -f /usr/local/lib/libopen-space-toolkit-core.so*; \
    cp -as /mnt/open-space-toolkit-core/include/OpenSpaceToolkit/Core /usr/local/include/OpenSpaceToolkit/Core; \
    cp -as /mnt/open-space-toolkit-core/src/OpenSpaceToolkit/Core/* /usr/local/include/OpenSpaceToolkit/Core/; \
    ln -s /mnt/open-space-toolkit-core/lib/libopen-space-toolkit-core.so /usr/local/lib/; \
    ln -s /mnt/open-space-toolkit-core/lib/libopen-space-toolkit-core.so.0 /usr/local/lib/;"

    # Open Space Toolkit ▸ I/O

    # TBI

    # Open Space Toolkit ▸ Mathematics

    # TBI

    # Output

    command="${command} \
    /bin/bash"

fi

# Run Docker container

docker run \
-it \
--rm \
--privileged \
${options} \
--volume="${project_directory}:/app:delegated" \
--volume="${project_directory}/tools/development/helpers:/app/build/helpers:ro,delegated" \
--workdir="/app/build" \
${docker_development_image_repository}:${docker_image_version}-${target} \
/bin/bash -c "${command}"

