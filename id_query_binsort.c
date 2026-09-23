#include <stdint.h>
#include <stdlib.h>

#include "id_query.h"
#include "record.h"

struct index_record {
    int64_t osm_id;
    const struct record *record;
};

struct indexed_data {
    struct index_record *irs;
    int n;
};

int comp(const void *a, const void *b) {
    struct index_record recordA = *(const struct index_record *)a;
    struct index_record recordB = *(const struct index_record *)b;

    if (recordA.osm_id < recordB.osm_id) {
        return -1;
    }
    if (recordA.osm_id > recordB.osm_id) {
        return 1;
    }
    return 0;
}

struct indexed_data *mk_indexed(struct record *rs, int n) {
    struct indexed_data *data = malloc(sizeof(struct indexed_data));
    data->irs = malloc(n * sizeof(struct index_record));
    data->n = n;

    for (int i = 0; i < n; i++) {
        const struct index_record rec = {rs[i].osm_id, &rs[i]};
        data->irs[i] = rec;
    }

    qsort(data->irs, n, sizeof(struct index_record), comp);

    return data;
}

void free_indexed(struct indexed_data *data) {
    free(data->irs);
    free(data);
}

const struct record *lookup_bin(struct indexed_data *data, int64_t needle) {
    int l = 0;
    int r = data->n - 1;
    const struct index_record *irs = data->irs;
    while (l <= r) {
        const int m = (l + r) / 2;
        if (irs[m].osm_id == needle) {
            return irs[m].record;
        }
        if (irs[m].osm_id > needle) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    return id_query_loop(argc, argv,
                      (mk_index_fn)mk_indexed,
                      (free_index_fn)free_indexed,
                      (lookup_fn)lookup_bin);
}


