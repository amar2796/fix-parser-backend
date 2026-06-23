# ── Stage 1: Build ────────────────────────────────────────────────────────────
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ cmake git build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY main.cpp .

RUN git clone --depth 1 https://github.com/CrowCpp/Crow.git /crow
RUN git clone --depth 1 https://github.com/chriskohlhoff/asio.git /asio

RUN g++ -std=c++17 -O2 -I/crow/include -I/asio/asio/include -DASIO_STANDALONE main.cpp -o server -lpthread

# ── Stage 2: Runtime ──────────────────────────────────────────────────────────
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/server .

EXPOSE 18080
CMD ["./server"]
