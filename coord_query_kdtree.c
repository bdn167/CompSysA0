#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"
#include "../../AppData/Local/Programs/CLion 2025.3/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/float.h"

struct kd_data {
    struct record *rs;
    int n;
    struct node *first;
};

struct node {
    double point;
    int axis;
    struct node *left;
    struct node *right;
    struct record *rec;
};

double get_point(struct record *rs, int i, int axis) {
    return axis == 1 ? rs[i].lat : rs[i].lon;
}

struct node *build(struct kd_data *data, int depth, int l, int r) {
    if (l > r) {
        return NULL;
    }

    int axis = depth % 2;
    int mid = l + (r - l) / 2;
    double median = get_point(data->rs, mid, axis);
    struct node *cur = malloc(sizeof(struct node));
    struct node *left = build(data, depth + 1, l, mid - 1);
    struct node *right = build(data, depth + 1, mid + 1, r);
    cur->point = median;
    cur->axis = axis;
    cur->left = left;
    cur->right = right;
    cur->rec = &data->rs[mid];
    return cur;
}

struct kd_data *mk_kd(struct record *rs, int n) {
    struct kd_data *data = malloc(sizeof(struct kd_data));
    data->rs = rs;
    data->n = n;
    data->first = build(data, 0, 0, n - 1);
    return data;
}

void free_naive(struct kd_data *data) {
    free(data);
}

double get_dist(double lon1, double lat1, double lon2, double lat2) {
    return fabs((lon1 - lon2) * (lon1 - lon2) - (lat1 - lat2) * (lat1 - lat2));
}

const struct record *lookup(double closest, double lon, double lat, struct node *cur) {
    if (!cur) {
        return NULL;
    }

    double query = cur->axis == 1 ? lat : lon;

    if (fabs(cur->point - query) < closest) {
        closest = cur->point;
    }
    double diff = cur->point - query;
    double radius = fabs(query - closest);
    if (diff >= 0 || radius > fabs(diff)) {
        return lookup(closest, lon, lat, cur->left);
    }
    if (diff <= 0 || radius > fabs(diff)) {
        return lookup(closest, lon, lat, cur->right);
    }

    return cur->rec;
}

const struct record *lookup_naive(struct kd_data *data, double lon, double lat) {
    lookup(DBL_MAX, lon, lat, data->first);
}

int main(int argc, char **argv) {
    return coord_query_loop(argc, argv,
                            (mk_index_fn) mk_kd,
                            (free_index_fn) free_naive,
                            (lookup_fn) lookup_naive);
}
