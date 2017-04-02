# Minimal docker container to build Nesca

FROM ubuntu:14.04
MAINTAINER Pantyusha <pantene@pere.val>

ENV DEBIAN_FRONTEND noninteractive
ENV QT_PATH /opt/Qt
ENV QT_DESKTOP $QT_PATH/5.4/gcc_64
ENV PATH $QT_DESKTOP/bin:$PATH

# Install updates & requirements:
#  * git, openssh-client, ca-certificates - clone & build
#  * curl, p7zip - to download & unpack Qt bundle
#  * build-essential, pkg-config, libgl1-mesa-dev - basic Qt build requirements
#  * libsm6, libice6, libxext6, libxrender1, libfontconfig1 - dependencies of Qt bundle run-file
RUN apt-get -qq update && apt-get -qq dist-upgrade && apt-get install -qq -y --no-install-recommends \
    git \
    wget \
    make \
    curl \
    openssh-client \
    ca-certificates \
    p7zip \
    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libssh-dev \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    && apt-get -qq clean

# Update curl
RUN apt-get -qq build-dep -y curl \
    && wget http://curl.haxx.se/download/curl-7.50.2.tar.bz2 \
    && tar -xvjf curl-7.50.2.tar.bz2 \
    && cd curl-7.50.2 \
    && ./configure \
    && make install

# Install Qt 5.4.2
RUN mkdir -p /tmp/qt \
    && curl -Lo /tmp/qt/installer.run 'https://download.qt.io/archive/qt/5.4/5.4.2/qt-opensource-linux-x64-5.4.2.run' \
    && chmod 755 /tmp/qt/installer.run && /tmp/qt/installer.run --dump-binary-data -o /tmp/qt/data \
    && mkdir $QT_PATH && cd $QT_PATH \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0qt5_essentials.7z > /dev/null \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0qt5_addons.7z > /dev/null \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0icu_53_1_ubuntu_11_10_64.7z > /dev/null \
    && /tmp/qt/installer.run --runoperation QtPatch linux $QT_DESKTOP qt5 \
    && rm -rf /tmp/qt

# Install multimedia dependencies
RUN apt-get -qq install -y --no-install-recommends qtmultimedia5-dev

# Download Nesca
RUN cd /tmp && git clone https://github.com/pantyusha/nesca.git

# Compile
RUN cd /tmp/nesca && qmake && make

# Add group & user
RUN groupadd -r user && useradd --create-home --gid user user && echo 'user ALL=NOPASSWD: ALL' > /etc/sudoers.d/user

# Move to user dir 
RUN mv /tmp/nesca /home/user/

USER user
WORKDIR /home/user/nesca
ENV HOME /home/user

ENTRYPOINT ["/home/user/nesca/nesca"]