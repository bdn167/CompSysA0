DATA_FILE="20000records_final.tsv"
N="100"

./random_coords "$DATA_FILE" | head -n "$N" > coords.txt
naive_out=$(./coord_query_naive "$DATA_FILE" < coords.txt)
kdtree_out=$(./coord_query_kdtree "$DATA_FILE" < coords.txt)

echo "== naive =="
echo "$naive_out" | grep -E "Reading records|Building index|Total query runtime"
echo
echo "== kd tree =="
echo "$kdtree_out" | grep -E "Reading records|Building index|Total query runtime"
echo