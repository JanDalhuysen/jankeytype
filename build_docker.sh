#!/bin/bash

# 1. Build the Docker image
# This creates a "snapshot" of your environment with the built program inside
echo "Building Docker image..."
docker build -t jankeytype-builder .

# 2. Extract the binary
# We create a temporary non-running container to copy files out of it
echo "Extracting binary..."
container_id=$(docker create jankeytype-builder)
docker cp $container_id:/app/build/jankeytype ./jankeytype
docker rm $container_id

echo "Done! The Linux executable is located in ./jankeytype"
