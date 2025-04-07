FROM gcc:latest

WORKDIR /app

# Copy all files
COPY . .

# Install required libraries for socket programming
RUN apt-get update && apt-get install -y \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# Compile the server for Linux
RUN gcc server.c -o server

# Expose port
EXPOSE 8080

# Run the server
CMD ["./server"] 