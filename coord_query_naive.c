#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  struct naive_data* data = malloc(sizeof(struct naive_data));
  
  data -> rs = rs;
  data -> n = n;

  return data;
}

void free_naive(struct naive_data* data) {
  free(data);
}

double get_dist(double lon1, double lat1, double lon2, double lat2) {
  double dx = lon1 - lon2;
  double dy = lat1 - lat2;
  return dx * dx + dy * dy;
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
  struct record *closest = NULL;
  double closestDist = DBL_MAX;

  for (int i = 0; i < data->n; i++) {
    struct record *cur = &data->rs[i];
    double dist = get_dist(cur->lon, cur->lat, lon, lat);
    if (dist < closestDist) {
      closest = cur;
      closestDist = dist;
    }
  }

  return closest;
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
