SKIP_FLASHING_SUPPORT=1 util/docker_build.sh crkbd/rev1:phaberest \
                -e USER_NAME=phaberest \
                -e OLED=yes \
                -j8
                # -e POINTING_DEVICE=trackpoint \
                # -e POINTING_DEVICE_POSITION=right \
