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

double Dist(double lon1, double lat1, double lon2, double lat2) {
  return sqrt((lon1-lon2)*(lon1-lon2)-(lat1-lat2)*(lat1-lat2));
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
  struct naive_data *closest = data;
  double dist = Dist(data->rs[0].lon, data->rs[0].lat, lon, lat);

  for (int i = 1; i < data->n; i++) {
    if (Dist(data->rs[i].lon, data->rs[i].lat, lon, lat) < dist) {
      closest = data;
    }
  }

  return closest->rs;
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
