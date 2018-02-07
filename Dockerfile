FROM ubuntu:16.04

LABEL description="A linux C++ build environment."

RUN apt-get update && apt-get install -y \
  build-essential \
  pkg-config \
  gcc \
  clang \
  cmake \
  libboost-all-dev \
  g++ `#Fb folly deps` \
  automake \
  autoconf \
  autoconf-archive \
  libtool \
  libboost-all-dev \
  libevent-dev \
  libdouble-conversion-dev \
  libgoogle-glog-dev \
  libgflags-dev \
  liblz4-dev \
  liblzma-dev \
  libsnappy-dev \
  make \
  zlib1g-dev \
  binutils-dev \
  libjemalloc-dev \
  libssl-dev \
  uuid-dev \
  libiberty-dev \
  libnl-3-200 \
  libnl-route-3-dev \
  libnl-3-dev \
  autotools-dev \
  autoconf \
  vim \
  libnl-genl-3-dev


RUN mkdir /jenkins
WORKDIR /jenkins

#ADD . /zenoutils

CMD ["/bin/bash"]
#CMD ["./autogen.sh"]
#CMD ["./configure"]
