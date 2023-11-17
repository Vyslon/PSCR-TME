//
// Created by Thomas SANTONI on 20/10/2023.
//

#ifndef TME5_BARRIER_H
#define TME5_BARRIER_H
#include <cstdlib>
#include <mutex>
#include <condition_variable>

namespace pr {

class Barrier {
public:
  mutable std::recursive_mutex m;
  int cpt;
  int nbJobs;
  std::condition_variable_any cv;

  Barrier(int nbJobs) : cpt(0), nbJobs(nbJobs)  {}

  void done() {
    std::unique_lock<std::recursive_mutex> lg(m);
    ++cpt;
    if (cpt == nbJobs)
    {
        cv.notify_one();
    }
  }

    void waitFor() {
        cv.wait(m);
    }

};


}
#endif //TME5_BARRIER_H
