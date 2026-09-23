#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "coord_query.h"

struct kd_tree {
    struct record *rs;
    int n;
};

struct kd_tree* mk_kd(struct record* rs, int n) {

}

struct kd_tree* build();

void free_naive(struct naive_data* data) {
    assert(0);
    // TODO
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
    assert(0);
    // TODO
}

int main(int argc, char** argv) {
    return coord_query_loop(argc, argv,
                            (mk_index_fn)mk_kd,
                            (free_index_fn)free_naive,
                            (lookup_fn)lookup_naive);
}
