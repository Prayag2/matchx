# Stage 1: build
FROM ubuntu:24.04 AS builder

# installing packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates build-essential cmake git lsb-release curl gpg \
    libssl-dev python3 libhiredis-dev doxygen \
    && rm -rf /var/lib/apt/lists/*

# installing redis
RUN curl -fsSL https://packages.redis.io/gpg | gpg --dearmor -o /usr/share/keyrings/redis-archive-keyring.gpg \
    && chmod 644 /usr/share/keyrings/redis-archive-keyring.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/redis-archive-keyring.gpg] https://packages.redis.io/deb $(lsb_release -cs) main" | tee /etc/apt/sources.list.d/redis.list \
    && apt-get update \
    && apt-get install -y redis

WORKDIR /tmp

# Build redis++ from source
RUN git clone https://github.com/sewenew/redis-plus-plus.git redispp \
    && cd redispp && mkdir build && cd build \
    && cmake .. && make && make install

WORKDIR /app
COPY . /app
RUN mkdir -p build && cd build && cmake .. && make
CMD ["./build/matchx"]
