#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "record.h"
#include "coord_query.h"

struct kd_data {
    struct record *rs;
    int n;
    struct node *first;
};

struct node {
    struct record *rec;
    int axis;
    struct node *left;
    struct node *right;
};

struct closest {
    struct node *node;
    double d;
};

int compare_lon(const void *a, const void *b) {
    double x = ((const struct record *)a)->lon;
    double y = ((const struct record *)b)->lon;
    return (x > y) - (x < y);
}
int compare_lat(const void *a, const void *b) {
    double x = ((const struct record *)a)->lat;
    double y = ((const struct record *)b)->lat;
    return (x > y) - (x < y);
}

struct node *build(struct kd_data *data, int depth, int l, int r) {
    if (l > r) return NULL;

    int axis = depth % 2;
    qsort(data->rs + l, r - l + 1, sizeof(struct record), axis == 0 ? compare_lon : compare_lat);

    int mid = l + (r - l) / 2;
    struct node *cur = malloc(sizeof(struct node));
    cur->rec = &data->rs[mid];
    cur->axis = axis;
    cur->left = build(data, depth + 1, l, mid - 1);
    cur->right = build(data, depth + 1, mid + 1, r);
    return cur;
}

struct kd_data *mk_kd(struct record *rs, int n) {
    struct kd_data *data = malloc(sizeof(struct kd_data));
    data->rs = rs;
    data->n = n;
    data->first = build(data, 0, 0, n - 1);
    return data;
}

void free_tree(struct node *n) {
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

void free_kd(struct kd_data *data) {
    free_tree(data->first);
    free(data);
}

double get_dist(double lon1, double lat1, double lon2, double lat2) {
    double dx = lon1 - lon2;
    double dy = lat1 - lat2;
    return dx * dx + dy * dy;
}

void lookup(struct closest *closest, double lon, double lat, struct node *cur) {
    if (!cur) return;

    double cur_lon = cur->rec->lon;
    double cur_lat = cur->rec->lat;
    double d = get_dist(cur_lon, cur_lat, lon, lat);
    if (d < closest->d) {
        closest->node = cur;
        closest->d = d;
    }

    double diff = cur->axis == 0
                  ? cur_lon - lon
                  : cur_lat - lat;

    if (diff >= 0 || closest->d > diff * diff) {
        lookup(closest, lon, lat, cur->left);
    }
    if (diff <= 0 || closest->d > diff * diff) {
        lookup(closest, lon, lat, cur->right);
    }
}

const struct record *lookup_kd(struct kd_data *data, double lon, double lat) {
    struct closest closest = {NULL, DBL_MAX};
    lookup(&closest, lon, lat, data->first);
    return closest.node ? closest.node->rec : NULL;
}

int main(int argc, char **argv) {
    return coord_query_loop(argc, argv,
                            (mk_index_fn) mk_kd,
                            (free_index_fn) free_kd,
                            (lookup_fn) lookup_kd);
}
