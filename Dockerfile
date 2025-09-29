# ===== Build stage =====
FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential make ca-certificates && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY . .
RUN make clean || true && make server

# ===== Runtime stage =====
FROM debian:stable-slim
RUN useradd -r -s /usr/sbin/nologin memoradb
WORKDIR /app
COPY --from=build /src/server /app/server
USER memoradb
EXPOSE 6379
ENTRYPOINT ["/app/server"]
