FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    libasound2-dev \
    libfreetype6-dev \
    libx11-dev \
    libxext-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxcomposite-dev \
    libwebkit2gtk-4.1-dev \
    libcurl4-openssl-dev \
    libgtk-3-dev \
    fontconfig \
    unzip \
    && apt-get clean

# Create a non-root user
RUN useradd -m dev
USER dev
WORKDIR /home/dev
