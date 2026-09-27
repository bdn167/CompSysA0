DATA_FILE=20000records_final.tsv
N="20000"

filter() {
  grep -Ev '^(Reading records|Building index|Query time|Total query runtime):';
}

./random_ids "$DATA_FILE" | head -n "$N" > ids.txt
./id_query_naive "$DATA_FILE" < ids.txt | filter > out_naive.txt
./id_query_indexed "$DATA_FILE" < ids.txt | filter > out_indexed.txt
./id_query_binsort "$DATA_FILE" < ids.txt | filter > out_binsort.txt

echo "naive vs indexed"
diff out_naive.txt out_indexed.txt && echo "MATCH"

echo "naive vs binsort"
diff out_naive.txt out_binsort.txt && echo "MATCH"