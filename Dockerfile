FROM ubuntu:24.04 AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .
RUN cmake -S . -B build -G Ninja && cmake --build build

FROM ubuntu:24.04 AS runtime
WORKDIR /app
COPY --from=build /src/build/olympus /app/olympus
COPY --from=build /src/config/olympus.toml /app/config/olympus.toml

CMD ["/app/olympus"]
