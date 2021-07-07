# Each instruction in this file generates a new layer that gets pushed to your local image cache
# Lines preceeded by # are regarded as comments and ignored
#
#
FROM ubuntu:latest
#
LABEL maintainer="gal.salomon@gmail.com"
#
# Update the image to the latest packages, the image will contain arrow-parquet,boost,and s3select(source).
# this version enables to run s3select queries on parquet-file located on local file-system/

RUN apt-get update
RUN apt-get install -y -V ca-certificates lsb-release wget
RUN wget https://apache.bintray.com/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-archive-keyring-latest-$(lsb_release --codename --short).deb
RUN apt-get install -y -V ./apache-arrow-archive-keyring-latest-$(lsb_release --codename --short).deb
RUN apt-get update
RUN apt-get install -y -V libarrow-dev
RUN apt-get install -y -V libparquet-dev
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends libboost-all-dev  libgtest-dev
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends git cmake g++ make
RUN git clone -b tasks https://github.com/neelgeek/s3select.git
RUN cd /s3select/ && cmake . && make -j4
RUN apt-get update
RUN apt-get install -y -V vim
RUN apt-get install -y -V gdb
RUN apt-get update \
  && apt-get install -y ssh \
    build-essential \
    gcc \
    g++ \
    gdb \
    clang \
    cmake \
    rsync \
    tar \
    python \
  && apt-get clean

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

RUN useradd -m user \
  && yes password | passwd user

RUN usermod -s /bin/bash user

CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]
