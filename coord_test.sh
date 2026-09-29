DATA_FILE=20000records_final.tsv
N="20000"

filter() {
  grep -Ev '^(Reading records|Building index|Query time|Total query runtime):';
}

./random_coords "$DATA_FILE" | head -n "$N" > coords.txt
./coord_query_naive "$DATA_FILE" < coords.txt | filter > out_naive.txt
./coord_query_kd "$DATA_FILE" < coords.txt | filter > out_kd.txt

echo "naive vs kd"
diff out_naive.txt out_kd.txt && echo "MATCH"