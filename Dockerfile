FROM gcc:latest

WORKDIR /app

# Copy all files
COPY . .

# Install required libraries for Windows socket programming
RUN apt-get update && apt-get install -y \
    libws2-32-dev \
    && rm -rf /var/lib/apt/lists/*

# Compile the server
RUN gcc server.c -o server -lwsock32 -lws2_32

# Expose port
EXPOSE 8080

# Run the server
CMD ["./server"] 