FROM jalospinoso/libpoco:latest
MAINTAINER "Josh Lospinoso" josh@lospi.net
RUN apt-get install software-properties-common -y && add-apt-repository ppa:george-edison55/cmake-3.x
RUN apt-get update && apt-get upgrade -y
RUN apt-get install cmake -y
COPY *.cpp *.h CMakeLists.txt /twitter-subliminal/
COPY tests/ /twitter-subliminal/tests/
COPY googletest/ /twitter-subliminal/googletest/
COPY cmake/ /twitter-subliminal/cmake/
RUN mkdir -p /twitter-subliminal/build
WORKDIR /twitter-subliminal/build
RUN cmake ..
RUN make
COPY twitter-subliminal.properties.example twitter-subliminal.properties
