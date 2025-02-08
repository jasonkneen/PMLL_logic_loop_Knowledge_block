#########################################################
# Stage 1: Debian-Based C++ Development Environment
#########################################################
FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12 AS debian
ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Copy and (conditionally) run the CMake reinstall script
COPY ./reinstall-cmake.sh /tmp/
RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi && rm -f /tmp/reinstall-cmake.sh

# Set environment variables
ENV VCPKG_INSTALLATION_ROOT=/usr/local/vcpkg
ENV JAVA_HOME_17_X64=/usr/lib/jvm/java-17-openjdk-amd64

# Install required packages
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
       python3-pip \
       nodejs \
       npm \
       openjdk-17-jdk \
       git && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Set up Python and Node.js
RUN python3 -m pip install --upgrade pip && npm install -g yarn

# Install vcpkg from source
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_INSTALLATION_ROOT} && \
    cd ${VCPKG_INSTALLATION_ROOT} && \
    ./bootstrap-vcpkg.sh

# Copy project files and set working directory
COPY . /workspace
WORKDIR /workspace

# Default command for the Debian-based container
CMD ["bash"]

#########################################################
# Stage 2: Ubuntu-Based C++ Development Environment
#########################################################
FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04 AS ubuntu
ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Copy and (conditionally) run the CMake reinstall script
COPY ./reinstall-cmake.sh /tmp/
RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi && rm -f /tmp/reinstall-cmake.sh

# Install additional packages for Ubuntu
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
       python3-pip \
       nodejs \
       npm \
       openjdk-17-jdk \
       gdb \
       valgrind \
       lsof \
       git \
       clang-18 \
       libstdc++-12-dev \
       glibc-source && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Set up Python and Node.js
RUN python3 -m pip install --upgrade pip && npm install -g yarn

# Install vcpkg for Ubuntu
ENV VCPKG_INSTALLATION_ROOT=/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_INSTALLATION_ROOT} && \
    cd ${VCPKG_INSTALLATION_ROOT} && \
    ./bootstrap-vcpkg.sh

# Copy project files and set working directory
COPY . /workspace
WORKDIR /workspace

# Default command for the Ubuntu-based container
CMD ["bash"]

#########################################################
# Stage 3: Windows-Based C++ Development Environment
#########################################################
FROM mcr.microsoft.com/dotnet/framework/sdk:4.8-windowsservercore-ltsc2022 AS windows
SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# Install Chocolatey and required Windows packages
RUN iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1')); \
    choco install -y msys2 cmake clang python nodejs git jdk17 visualstudio2022buildtools --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"

# Update PATH for tools installed by Chocolatey
ENV PATH="${PATH};C:\msys64\usr\bin;C:\Program Files\Git\cmd"

# Install vcpkg on Windows
RUN git clone https://github.com/microsoft/vcpkg.git C:\vcpkg; \
    cd C:\vcpkg; \
    .\bootstrap-vcpkg.bat

# Copy project files and set working directory
COPY . C:\workspace
WORKDIR C:\workspace

# Default command for the Windows-based container
CMD ["powershell"]
