#include<iostream>
#include<string>
#include<unordered_map>
#include<utility>
#include <x86intrin.h>

class TimeMap {
  public:
    TimeMap(unsigned int capacity) : capacity_(capacity) {}
    
    int get(std::string key) {
        mapa_[key].first = __rdtsc();
        return mapa_[key].second;
    }
    void set (std::string key, int val) {
        auto l = mapa_.find(key);
        if (l == mapa_.end() && mapa_.size() == capacity_ ) {
            // erasing the oldest one
            auto ancient_one = mapa_.begin();
            for (auto v = std::next(mapa_.begin()); v != mapa_.end(); ++v) {
                if (v->second.first < ancient_one->second.first) {
                        ancient_one = v;                                                
                }
            }
            mapa_.erase(ancient_one);
        }
        mapa_[key] = std::make_pair(__rdtsc(),val);
    }

    void print_map () {
      std::cout << "Mapa: ";
      for (auto& e : mapa_) {
        std::cout << " " << e.second.second << "(ts:" << e.second.first << ")," ;
      }
      std::cout << std::endl;
    }

  private:
    std::unordered_map<std::string, std::pair<unsigned long long,int> > mapa_;
    unsigned int capacity_;
};

int main()
{
  TimeMap mapa(4); 

  mapa.set("k1", 1);
  mapa.set("k2", 2);
  mapa.set("k3", 3);
  mapa.set("k4", 4);
  auto get1 = mapa.get("k1");
  mapa.set("k5", 5);

  mapa.print_map();
  return 0;
}
