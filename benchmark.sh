DATA_FILE="20000records_final.tsv"
N="10000"

./random_ids "$DATA_FILE" | head -n "$N" > ids.txt
naive_out=$(./id_query_naive "$DATA_FILE" < ids.txt)
indexed_out=$(./id_query_indexed "$DATA_FILE" < ids.txt)
binsort_out=$(./id_query_binsort "$DATA_FILE" < ids.txt)

echo "== naive =="
echo "$naive_out" | grep -E "Reading records|Building index|Total query runtime"
echo
echo "== indexed =="
echo "$indexed_out" | grep -E "Reading records|Building index|Total query runtime"
echo
echo "== binsort =="
echo "$binsort_out" | grep -E "Reading records|Building index|Total query runtime"