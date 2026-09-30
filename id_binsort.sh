SECONDS_TO_RUN=10
BINARY=./id_query_binsort
DATA_FILE=records.tsv

printf "9242\n165789\n" | timeout -s INT $SECONDS_TO_RUN \
    valgrind --leak-check=full --show-leak-kinds=all \
    "$BINARY" "$DATA_FILE"