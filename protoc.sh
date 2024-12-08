THIRD_PARTY_PROTO_DIR="src/3rdparty/proto"
THIRD_PARTY_OUT_DIR="src/3rdparty/proto/pb"

mkdir -p "$THIRD_PARTY_OUT_DIR"

# Check if the proto directory exists
if [ ! -d "$THIRD_PARTY_PROTO_DIR" ]; then
    echo "Error: Directory $THIRD_PARTY_PROTO_DIR does not exist."
    exit 1
fi

for proto_file in "$THIRD_PARTY_PROTO_DIR"/*.proto; do
    if [ -f "$proto_file" ]; then
        protoc --cpp_out="$THIRD_PARTY_OUT_DIR" --proto_path="$THIRD_PARTY_PROTO_DIR" "$proto_file"
    else
        echo "Warning: No .proto files found in $THIRD_PARTY_PROTO_DIR."
        exit 1
    fi
done