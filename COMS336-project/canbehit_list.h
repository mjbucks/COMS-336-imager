#ifndef CANBEHIT_LIST_H
#define CANBEHIT_LIST_H

#include "canbehit.h"
#include "commons.h"

#include <vector>

class canbehit_list : public canbehit {
  public:
    std::vector<shared_ptr<canbehit>> objects;

    canbehit_list() {}
    canbehit_list(shared_ptr<canbehit> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<canbehit> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif