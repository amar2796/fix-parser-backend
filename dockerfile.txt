FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ cmake git build-essential

WORKDIR /app
COPY . .

# Crow is header-only; clone it
RUN git clone --depth 1 https://github.com/CrowCpp/Crow.git /crow

RUN g++ -std=c++17 -O2 -I/crow/include main.cpp -o server -lpthread

EXPOSE 18080
CMD ["./server"]