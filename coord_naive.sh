SECONDS_TO_RUN=10
BINARY=./coord_query_naive
DATA_FILE=records.tsv

printf "2 9\n-84.5069411 45.0034049\n" | timeout -s INT $SECONDS_TO_RUN \
    valgrind --leak-check=full --show-leak-kinds=all \
    "$BINARY" "$DATA_FILE"