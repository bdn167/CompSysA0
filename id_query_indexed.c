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

struct indexed_data *mk_indexed(struct record *rs, int n) {
    struct indexed_data *data = malloc(sizeof(struct indexed_data));

    for (int i = 0; i < n; i++) {
        struct index_record *rec = malloc(sizeof(struct index_record));
        rec->osm_id = rs[i].osm_id;
        rec->record = &rs[i];
        data->irs[i] = *rec;
    }

    return data;
}

void free_indexed(struct indexed_data *data) {
    for (int i = 0; i < data->n; i++) {
        free(&data->irs[i]);
    }

    free(data);
}

const struct record *lookup_indexed(struct indexed_data *data, int64_t needle) {
    for (int i = 0; i < data->n; i++) {
        if (data->irs[i].osm_id == needle) {
            return data->irs[i].record;
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    return id_query_loop(argc, argv,
                      (mk_index_fn)mk_indexed,
                      (free_index_fn)free_indexed,
                      (lookup_fn)lookup_indexed);
}


